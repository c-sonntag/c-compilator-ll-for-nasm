/**
 * @brief Outils pour le parcours
 *
 * @author Yannick Robin
 * @author Christophe Sonntag (http://u4a.at)
 *
 * @defgroup ParcoursTools Outils pour le parcours de l'arbres abstrait
 * @{
 */

#include "parcoursTools.h"
#include "parcours.h"
#include "util/message.h"
#include "tabsymboles.h"
#include "assembleur.h"

#include <assert.h>
#include <stdlib.h>

/**
 * @defgroup ParcoursToolsExpr Expression
 * @{
 */

/**
 * @brief calculer_exp_const permet de calculer les opérations constantes
 * @param computeComparaison : indique si le resultat est une comparaison
 */
int parcoursTools_constexpr_operation( const operation op, const int op1, const int op2, bool * computeComparaison )
{
  *computeComparaison = false;
  switch( op )
  {
  case plus:
    return op1 + op2;
  case moins:
    return op1 - op2;
  case fois:
    return op1 * op2;
  case divise:
    return op1 / op2;
  case modulo:
    return op1 % op2;

    #if HAVE_LOGIQUE_COMPARAISON==1
  case et:
    return op1 & op2;
  case ou:
    return op1 | op2;
    #else
  case et:
    return op1 && op2;
  case ou:
    return op1 || op2;
    #endif

  case non:
    return !op1;
  case negatif:
    return -op1;

  case diff:
    *computeComparaison = true;
    return op1 != op2;
  case inf:
    *computeComparaison = true;
    return op1 < op2;
  case sup:
    *computeComparaison = true;
    return op1 > op2;
  case infeg:
    *computeComparaison = true;
    return op1 <= op2;
  case supeg:
    *computeComparaison = true;
    return op1 >= op2;

  default : /*  -Wswitch */
    break;
  }
  erreurArgs( "'op' valide necessaire pour parcoursTools_constexpr_operation" );
  return 0;
}

/**
 * @brief parcoursTools_constexpr : créer la valeur constante, si l'operation (recursive) ne contient que des entiers
 * @param compute : le resultat constant, si l'operation est valide
 * @param computeComparaison : indique si le resultat est une comparaison
 * @return indique si l'operation est valide
 */
bool parcoursTools_constexpr( const n_exp * n, int * compute, bool * computeComparaison )
{
  if( n == NULL )
  {
    *compute = 0;
    return true;
  }
  else if( n->type == opExp )
  {
    int op1, op2;

    //
    if( !parcoursTools_constexpr( n->u.opExp_.op1, &op1, computeComparaison  ) )
      return false;
    if( !parcoursTools_constexpr( n->u.opExp_.op2, &op2, computeComparaison  ) )
      return false;

    //
    *compute = parcoursTools_constexpr_operation( n->u.opExp_.op, op1, op2, computeComparaison );
    return true;
  }
  else if ( n->type == intExp )
  {
    *compute =  n->u.entier;
    return true;
  }
  else
    return false;
}


/**
 * @brief parcoursTools_computeOperation
 * @param reg : le registre pour recuperer le resultat
 * @param oneOperandDestination : ptr sur boolean, indique si l'opération ne prend pas le registre 'source' et donc seulement le registre 'destination'
 * @param isCall : ptr sur boolean, indique si l'opération doit être appelée
 * @return la chaine de caractère corespondant à l'instruction de l'opération 'op'
 */
const char * parcoursTools_computeOperation( const operation op, const char ** reg, bool * oneOperandDestination, bool * isCall )
{
  *reg = "eax";
  *oneOperandDestination = false;
  *isCall = false;
  switch( op )
  {
  case plus:
    return "add";
  case moins:
    return "sub";
  case fois:
    *oneOperandDestination = true;
    return "mul";
  case divise:
    *oneOperandDestination = true;
    return "div";
  case modulo:
    /** @todo Voir pour ajouter l'instruction initialisation de edx :
     *     mov  edx, 0     ; initialise edx à zéro */
    *oneOperandDestination = true;
    *reg = "edx";
    return "div";

    #if HAVE_LOGIQUE_COMPARAISON==1
  case et:
    return "and";
  case ou:
    return "or";
  case non:
    return "not";
    #else
  case et:
    *isCall = true;
    return parcoursTools_lib_comparaisonBool( et );
  case ou:
    *isCall = true;
    return parcoursTools_lib_comparaisonBool( ou );
  case non:
    *isCall = true;
    return parcoursTools_lib_comparaisonBool( non );
    #endif

  //case negatif:
  //  return "not";
  default :
    return NULL;
  }
}

const char * parcoursTools_comparaisonOperation( const operation op )
{
  switch( op )
  {
  case egal:
    return "je";
  case diff:
    return "jne";
  case inf:
    return "jl";
  case sup:
    return "jg";
  case infeg:
    return "jle";
  case supeg:
    return "jge";
  default :
    return NULL;
  }
}

/**
 * @}
 * @defgroup ParcoursToolsListe Liste
 * @{
 */

/**
 * @brief parcoursTools_taille_n_l : renvoi la longeur d'une liste chainée
 */
size_t parcoursTools_taille_n_l( n_l_dec * n )
{
  return n ? ( 1 + parcoursTools_taille_n_l( n->queue ) ) : 0;
}

/**
 * @}
 * @defgroup ParcoursToolsAsmTranscode Aquisition
 * @{
 */

extern assembleur_entry * ce;
void parcours_exp( const n_exp * n );

/**
 * @brief parcoursTools_accesVar
 * @warning AccesVar peut ecrire dans l'entrée courante 'ce'
 * @warning le registre 'eax' peut etre utiliser durant l'operation (cas tableau)
 */
const char * parcoursTools_accesVar( const n_var * variable, const char ** comment )
{
  const desc_identif * variableSymb = tabsymb_getSymbole( variable->nom );

  // Check...
  if( variableSymb == NULL )
    erreurArgs( "Variable '%s' non déclarée !", variable->nom );
  else if( ( variableSymb->type != T_ENTIER ) && ( variableSymb->type != T_TABLEAU_ENTIER ) )
    erreurArgs( "'%s' n'est pas déclarée en tant que variable", variable->nom );
  else if( ( variable->type == indicee ) &&  ( variableSymb->type == T_ENTIER ) )
    erreurArgs( "'%s' est déclarée en tant que simple variable", variable->nom );
  else if( ( variable->type == simple ) &&  ( variableSymb->type == T_TABLEAU_ENTIER ) )
    erreurArgs( "'%s' est déclarée en tant que tableau de variable", variable->nom );

  //
  *comment = NULL;

  //
  if( ( variableSymb->portee == P_VARIABLE_GLOBALE ) && ( variableSymb->type == T_ENTIER ) )
  {
    *comment = "variable global";
    return assembleur_make_globalVar( variable->nom );
  }
  else if( ( variableSymb->portee == P_VARIABLE_GLOBALE ) && ( variableSymb->type == T_TABLEAU_ENTIER ) )
  {
    parcours_exp( variable->u.indicee_.indice );
    assembleur_entry_1s( ce, "pop", "eax" );
    assembleur_entry_2s_andComment( ce, "add", "eax", "eax", "(multiplie par 2)" );
    assembleur_entry_2s_andComment( ce, "add", "eax", "eax", "(multiplie par 2)" );

    *comment = "tableau entier indicée à partir de l'indice stocké dans 'eax'";
    return assembleur_make_tabVar( variable->nom, "eax" );
  }
  else if( ( variableSymb->portee == P_ARGUMENT ) && ( variableSymb->type == T_ENTIER )  )
  {
    const desc_identif * fonctionCouranteSymb = tabsymb_getSymboleFonctionCourante();
    if( fonctionCouranteSymb == NULL )
      erreurArgs( "Incroyable, vous utilisez un argument de fonction sans être dans une fonction !" );
    //
    *comment = "argument";
    return assembleur_make_localVar(
             "ebp", ( 8 +
                      ( ( fonctionCouranteSymb->complement == 0 ) ? 0 : ( ( fonctionCouranteSymb->complement - 1 ) * 4 ) )
                    ) - ( variableSymb->adresse )
           );
    //return assembleur_make_localVar( "ebp", ( 8 + fonctionCouranteSymb->complement * 4 ) - ( variableSymb->adresse ) );
    // +8 : (push de la fonction local) et (sub esp, 4 : pour la valeur renvoyé)
    // (fonctionCouranteSymb->complement * 4) :  le premier argument se trouve au sommet de pile (ebp + 12 par exemple),
    //                                           alors que le dernier se trouve au debut (ebp + 8 par exemple)
  }
  else if( ( variableSymb->portee == P_VARIABLE_LOCALE ) && ( variableSymb->type == T_ENTIER ) )
  {
    *comment = "variable local";
    return assembleur_make_localVar( "ebp", ( -4 ) - variableSymb->adresse );
    // -4 : (push de la fonction local)
  }

  //
  return NULL;
}


/**
 * @}
 * @defgroup ParcoursToolsLib Bibliothèque
 * @{
 */

/**
 * @defgroup ParcoursToolsLib_comparatorExpr Bibliothèque comparatorExpr
 * @{
 */


#define LIB_COMPARATOREXPR_PREFFIX "_cmpBin_"
#define LIB_COMPARATOREXPR_SIZE 6
static bool lib_is_comparatorExpr[LIB_COMPARATOREXPR_SIZE] = {false};
static operation lib_comparatorExpr[LIB_COMPARATOREXPR_SIZE] = {egal, diff, inf, sup, infeg, supeg};

const char * parcoursTools_lib_comparatorExpr( operation op )
{
  for( size_t i = 0; i < LIB_COMPARATOREXPR_SIZE; i++ )
  {
    if( op == lib_comparatorExpr[i] )
    {
      lib_is_comparatorExpr[i] = true;
      const char * comparator = parcoursTools_comparaisonOperation( lib_comparatorExpr[i] );
      return assembleur_make_concat( LIB_COMPARATOREXPR_PREFFIX , comparator ) ;
    }
  }
  return NULL;
}

void parcoursTools_genLib_comparatorExpr()
{
  assembleur_entry * cmpBin_1_entry = NULL;
  assembleur_entry * cmpBin_0_entry = NULL;

  for( size_t i = 0; i < LIB_COMPARATOREXPR_SIZE; i++ )
  {
    if( !lib_is_comparatorExpr[i] )
      continue;

    //
    if( ( cmpBin_0_entry == NULL ) || ( cmpBin_1_entry == NULL ) )
    {
      cmpBin_1_entry = assembleur_new_entryName( LIB_COMPARATOREXPR_PREFFIX "_1" );
      assembleur_entry_2s( cmpBin_1_entry, "mov", "eax", "1" );
      assembleur_entry_0s( cmpBin_1_entry, "ret" );

      cmpBin_0_entry = assembleur_new_entryName( LIB_COMPARATOREXPR_PREFFIX "_0" );
      assembleur_entry_2s( cmpBin_0_entry, "mov", "eax", "0" );
      assembleur_entry_0s( cmpBin_0_entry, "ret" );

      assembleur_add_entry( cmpBin_1_entry );
      assembleur_add_entry( cmpBin_0_entry );
    }

    //
    const char * comparator = parcoursTools_comparaisonOperation( lib_comparatorExpr[i] );
    assembleur_entry * cmpBin_entry = assembleur_add_entryName( assembleur_make_concat( LIB_COMPARATOREXPR_PREFFIX , comparator ) );

    //
    assembleur_entry_2s( cmpBin_entry, "cmp", "eax", "ebx" );
    assembleur_entry_1s( cmpBin_entry, comparator, assembleur_entry_getName( cmpBin_1_entry ) );
    assembleur_entry_1s( cmpBin_entry, "jmp", assembleur_entry_getName( cmpBin_0_entry ) );
  }
}

/**
 * @}
 * @defgroup ParcoursToolsLib_comparaisonBool Bibliothèque comparaisonBool
 * @{
 */

#define LIB_COMPARAISONBOOL_PREFFIX "_cmpBool_"
#define LIB_COMPARAISONBOOL_SIZE 3
static bool lib_is_comparaisonBool[LIB_COMPARAISONBOOL_SIZE] = {false};
static operation lib_comparaisonBool[LIB_COMPARAISONBOOL_SIZE] = {et, ou, non};

const char * parcoursTools_lib_comparaisonBool_getName( operation op )
{
  if( op == et )
    return LIB_COMPARAISONBOOL_PREFFIX "et";
  else if( op == ou )
    return LIB_COMPARAISONBOOL_PREFFIX "ou";
  else if( op == non )
    return LIB_COMPARAISONBOOL_PREFFIX "non";
  else
    erreurArgs( "La bibliothèque comparaisonBool ne dispose que des opérations : et,ou,non" );
  return NULL; // -Wall
}

const char * parcoursTools_lib_comparaisonBool( operation op )
{
  for( size_t i = 0; i < LIB_COMPARAISONBOOL_SIZE; i++ )
  {
    if( op == lib_comparaisonBool[i] )
    {
      lib_is_comparaisonBool[i] = true;
      return parcoursTools_lib_comparaisonBool_getName( op ) ;
    }
  }
  return NULL;
}

void parcoursTools_genLib_comparaisonBool()
{
  assembleur_entry * cmpBool_1_entry = NULL;
  assembleur_entry * cmpBool_0_entry = NULL;

  for( size_t i = 0; i < LIB_COMPARAISONBOOL_SIZE; i++ )
  {
    if( !lib_is_comparaisonBool[i] )
      continue;

    if( ( cmpBool_0_entry == NULL ) || ( cmpBool_1_entry == NULL ) )
    {
      cmpBool_1_entry = assembleur_new_entryName( LIB_COMPARAISONBOOL_PREFFIX "_1" );
      assembleur_entry_2s( cmpBool_1_entry, "mov", "eax", "1" );
      assembleur_entry_0s( cmpBool_1_entry, "ret" );

      cmpBool_0_entry = assembleur_new_entryName( LIB_COMPARAISONBOOL_PREFFIX "_0" );
      assembleur_entry_2s( cmpBool_0_entry, "mov", "eax", "0" );
      assembleur_entry_0s( cmpBool_0_entry, "ret" );

      assembleur_add_entry( cmpBool_1_entry );
      assembleur_add_entry( cmpBool_0_entry );
    }


    if( lib_comparaisonBool[i] == et )
    {
      assembleur_entry * cmpBoolEt_entry = assembleur_add_entryName( parcoursTools_lib_comparaisonBool_getName( et ) );

      //
      assembleur_entry * cmpBoolEt_2ndOp_entry = assembleur_new_entryName( assembleur_entry_make_preffix( cmpBoolEt_entry, "2ndOp" ) );
      assembleur_entry_2s( cmpBoolEt_2ndOp_entry, "cmp", "ebx", "0" );
      assembleur_entry_1s( cmpBoolEt_2ndOp_entry, "jne", assembleur_entry_getName( cmpBool_1_entry ) );
      assembleur_entry_1s( cmpBoolEt_2ndOp_entry, "jmp", assembleur_entry_getName( cmpBool_0_entry ) );

      //
      assembleur_entry_2s( cmpBoolEt_entry, "cmp", "eax", "0" );
      assembleur_entry_1s( cmpBoolEt_entry, "jne", assembleur_entry_getName( cmpBoolEt_2ndOp_entry ) );
      assembleur_entry_1s( cmpBoolEt_entry, "jmp", assembleur_entry_getName( cmpBool_0_entry ) );
      assembleur_entry_add_subEntry( cmpBoolEt_entry, cmpBoolEt_2ndOp_entry );
    }

    else if( lib_comparaisonBool[i] == ou )
    {
      assembleur_entry * cmpBoolOu_entry = assembleur_add_entryName( parcoursTools_lib_comparaisonBool_getName( ou ) );

      //
      assembleur_entry * cmpBoolOu_2ndOp_entry = assembleur_new_entryName( assembleur_entry_make_preffix( cmpBoolOu_entry, "2ndOp" ) );
      assembleur_entry_2s( cmpBoolOu_2ndOp_entry, "cmp", "ebx", "0" );
      assembleur_entry_1s( cmpBoolOu_2ndOp_entry, "jne", assembleur_entry_getName( cmpBool_1_entry ) );
      assembleur_entry_1s( cmpBoolOu_2ndOp_entry, "jmp", assembleur_entry_getName( cmpBool_0_entry ) );

      //
      assembleur_entry_2s( cmpBoolOu_entry, "cmp", "eax", "0" );
      assembleur_entry_1s( cmpBoolOu_entry, "jne", assembleur_entry_getName( cmpBool_1_entry ) );
      assembleur_entry_1s( cmpBoolOu_entry, "jmp", assembleur_entry_getName( cmpBoolOu_2ndOp_entry ) );
      assembleur_entry_add_subEntry( cmpBoolOu_entry, cmpBoolOu_2ndOp_entry );
    }

    else if( lib_comparaisonBool[i] == non )
    {
      assembleur_entry * cmpBoolOu_entry = assembleur_add_entryName( parcoursTools_lib_comparaisonBool_getName( non ) );
      assembleur_entry_2s( cmpBoolOu_entry, "cmp", "eax", "0" );
      assembleur_entry_1s( cmpBoolOu_entry, "jne", assembleur_entry_getName( cmpBool_0_entry ) );
      assembleur_entry_1s( cmpBoolOu_entry, "jmp", assembleur_entry_getName( cmpBool_1_entry ) );
    }

  }
}

/**
* @}
*/


void parcoursTools_dump_asm_lib()
{
  parcoursTools_genLib_comparatorExpr();
  parcoursTools_genLib_comparaisonBool();


}


/**
* @}
*/

/**
* @}
*/
