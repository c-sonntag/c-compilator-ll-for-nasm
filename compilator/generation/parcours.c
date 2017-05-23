/**
 * @brief Partie création de la Table des symboles et de l'arbre abstrait
 *
 * @author Alexis Nasr (http://pageperso.lif.univ-mrs.fr/~alexis.nasr/)
 * @author released by Yannick Robin
 * @author released by Christophe Sonntag (http://u4a.at)
 *
 * @defgroup Parcours Parcours de l'arbres abstrait pour la génération
 * @{
 */

#include "parcours.h"
#include "parcoursTools.h"

#include "tabsymboles.h"
#include "assembleur.h"

#include "util/message.h"

#include <stdio.h>
#include <string.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS

  void parcours_prog( const n_prog * n );
  void parcours_init_asm();
  const char * parcours_get_endEntryName();

  unsigned int parcours_prefixe_l_dec( const n_l_dec * n, int porte );
  unsigned int parcours_suffixe_l_dec( const n_l_dec * n, int porte );
  void parcours_dec( const n_dec * n, int porte );
  void parcours_dec_fonctions( const n_dec * n );

  unsigned int parcours_prefixe_l_instr( const n_l_instr * n );
  unsigned int parcours_suffixe_l_instr( const n_l_instr * n );
  void parcours_instr( const n_instr * n );
  void parcours_instr_ecrire( const n_exp * n );
  void parcours_instr_affect( const n_var * var, const n_exp * exp );
  void parcours_instr_incr( const n_var * var );
  void parcours_instr_tantque( const n_exp * test, const n_instr * faire );
  void parcours_instr_appel( const n_appel * appel );
  void parcours_instr_si( const n_exp * test, const n_instr * alors, const n_instr * sinon );
  void parcours_instr_retour( const n_exp * expression );

  unsigned int parcours_prefixe_l_exp( const n_l_exp * n );
  unsigned int parcours_suffixe_l_exp( const n_l_exp * n );
  void parcours_exp( const n_exp * n );
  int parcours_exp_comparaison( const n_exp * n, const char ** comparaisonOperationPtr );
  void parcours_exp_lire();
  void parcours_exp_var( const n_var * variable );
  void parcours_exp_int( const int entier );
  void parcours_exp_appel( const n_appel * appel, const bool desallocReturn );
  int parcours_exp_op( const n_exp * n, const bool isComparaison );

#endif /* DOXYGEN_SHOULD_SKIP_THIS */


/**
 * @defgroup ParcoursLocal Variable de gestion de la table des symboles
 * @{
 */

int adresseGlobalCourant;
int adresseLocaleCourante;
int adresseArgumentCourant;

/** c'est l'entrée courante */
assembleur_entry * ce = NULL;

/** entrée d'une fonction */
assembleur_entry * topEntry = NULL;

/** entrée de fin si un ou, plusieurs 'retour' ont eu lieu */
assembleur_entry * endEntry = NULL;

/**
 * @}
 * @defgroup ParcoursDef Definition pour la gestion du parcours
 * @{
 */

#define ErreurSiNulle(var) \
  if(var==NULL) { erreurArgs("La fonction '%s' n'accepte pas de valeur nulle", __FUNCTION__); }

#define ErreurSiMauvaisType(var,leTypeVoulut) \
  if(var->type!=leTypeVoulut) { erreurArgs("La fonction '%s' necessite le type '%s'", __FUNCTION__, #leTypeVoulut ); }


/**
 * @}
 * @defgroup ParcoursFonction Fonction de parcours pour la génération
 * @{
 */


/**
 * @defgroup ParcoursFonctionPrincipal Entrées principales
 * @{
 */

/**
 * @brief parcours_prog : Génère la table des symbole et le code assembleur
 * @param n : un arbre abstrait
 */
void parcours_prog( const n_prog * n )
{
  ErreurSiNulle( n )

  //
  adresseGlobalCourant = 0;

  //
  parcours_init_asm();
  parcours_prefixe_l_dec( n->variables, P_VARIABLE_GLOBALE );
  parcours_prefixe_l_dec( n->fonctions, P_VARIABLE_GLOBALE );
  parcoursTools_dump_asm_lib();

  //
  const desc_identif * main_symb = tabsymb_getSymbole( "main" );
  if( main_symb == NULL )
    erreurArgs( "La fonction 'main' est requise !!!" );
  else if( main_symb->type != T_FONCTION )
    erreurArgs( "L'identificateur 'main' doit etre une fonction !" );

}


void parcours_init_asm()
{
  assembleur_entry * start_entry = assembleur_add_entryName_andComment( "_start", "entrée start" );

  //
  #if PARCOURS_STARTENTRY_ACCEPT_RETVALUE == 1  //  ajoute l'entrée _start avec valeur de retour
  assembleur_entry_1s_andComment( start_entry, "push", "0", "valeur de retour initiale" );
  assembleur_entry_1s( start_entry, "call", "main" );
  assembleur_entry_2s_andComment( start_entry, "mov", "eax", "1" , "1 est le code de SYS_EXIT" );
  assembleur_entry_1s_andComment( start_entry, "pop", "ebx", "recup valeur de retour, pour 'exit'" );
  assembleur_entry_1s_andComment( start_entry, "int", "0x80", "exit" );

  #else //  ajoute l'entrée _start sans valeur de retour
  assembleur_entry_1s( start_entry, "call", "main" );
  assembleur_entry_2s_andComment( start_entry, "mov", "eax", "1" , "1 est le code de SYS_EXIT" );
  assembleur_entry_2s_andComment( start_entry, "mov", "ebx", "0" , "retourne 0" );
  assembleur_entry_1s_andComment( start_entry, "int", "0x80", "exit" );

  #endif


  // ajoute la variable global sinput
  assembleur_add_bss_andComment( "sinput", resb, 255, "reserve a 255 byte space in memory for the users input string" );
}


/**
 * @}
 * @defgroup ParcoursFonctionInternal Fonction de gestion interne
 * @{
 */

/**
 * @brief parcours_get_endEntryName
 *  genère une entrée ('assembleur_entry') de type 'entryPoint'
 *  conçue pour être placée en fin de fonction.
 * @note Necessaire à l'implémantation de l'instruction 'retour' du langage L
 * @return retourne le nom du 'entryPoint' disponnible en fin de fonction.
 */
const char * parcours_get_endEntryName()
{
  if( endEntry == NULL )
  {
    ErreurSiNulle( topEntry )
    endEntry = assembleur_new_entryName(
                 assembleur_entry_make_preffix( topEntry, "END" )
               );
  }
  return assembleur_entry_getName( endEntry );
}

/**
 * @}
 * @defgroup ParcoursFonctionDeclaration Declaration
 * @{
 */

unsigned int parcours_prefixe_l_dec( const n_l_dec * n, int porte )
{
  if( n )
  {
    parcours_dec( n->tete, porte );
    return parcours_prefixe_l_dec( n->queue, porte ) + 1;
  }
  return 0;
}


unsigned int parcours_suffixe_l_dec( const n_l_dec * n, int porte )
{
  if( n )
  {
    const unsigned int p = parcours_suffixe_l_dec( n->queue, porte ) + 1;
    parcours_dec( n->tete, porte );
    return p;
  }
  return 0;
}



/**
* @brief parcours_dec
*  Pour le moment, les seules déclarations prises en compte sont les variables
*  globales. Lors du parcours d’un noeud correspondant à une variable globale, il faut allouer
*  de la place pour cette variable dans la région .bss du code machine, à l’aide des pseudo
*  instructions resb, resw . .
*/
void parcours_dec( const n_dec * n, int porte )
{
  ErreurSiNulle( n )

  int type = 0;
  unsigned int complement = 1;
  unsigned int tailleDeclaration = 0;

  // Recherche type, complement et tailleDeclaration
  switch( n->type )
  {
  case foncDec:
    type = T_FONCTION;
    complement = parcoursTools_taille_n_l( n->u.foncDec_.param );
    break;
  case varDec:
    type = T_ENTIER;
    tailleDeclaration = 4;
    break;
  case tabDec:
    if( porte != P_VARIABLE_GLOBALE )
      erreurArgs( "Les tableaux ne sont pas pris en chage, sauf en tant que variable globale" );
    //
    type = T_TABLEAU_ENTIER;
    complement = n->u.tabDec_.taille;
    break;
  }

  // Recherche Adresse
  int adresse = 0;
  switch( porte )
  {

  case P_VARIABLE_LOCALE:
    adresse = adresseLocaleCourante;
    adresseLocaleCourante += tailleDeclaration;
    break;

  case P_VARIABLE_GLOBALE:
    if( type == T_ENTIER )
    {
      adresse = adresseGlobalCourant;
      adresseGlobalCourant += tailleDeclaration;
    }
    break;

  case P_ARGUMENT:
    adresse = adresseArgumentCourant;
    adresseArgumentCourant += tailleDeclaration;
    break;

  }

  // Checks ...
  if( tabsymb_rechercheDeclarative( n->nom ) >= 0 )
    erreurArgs( "Identificateur '%s' déja existant !", n->nom );

  //
  tabsymb_ajouteIdentificateur( n->nom, porte, type, adresse, complement );

  //
  if( ( type != T_FONCTION ) && ( porte == P_VARIABLE_GLOBALE ) )
    assembleur_add_bss( n->nom, rest, complement );

  //
  if( n->type == foncDec )
    parcours_dec_fonctions( n );
}


void parcours_dec_fonctions( const n_dec * n )
{
  ErreurSiNulle( n )
  ErreurSiMauvaisType( n, foncDec )

  //
  n_instr * corps = n->u.foncDec_.corps;
  ErreurSiMauvaisType( corps, blocInst )

  //
  tabsymb_entreeFonction();

  //
  topEntry = assembleur_add_entryName( n->nom );
  endEntry = NULL;
  ce = topEntry;

  //
  assembleur_entry_1s_andComment( ce, "push", "ebp", "sauvegarde la valeur de ebp" );
  assembleur_entry_2s_andComment( ce, "mov", "ebp", "esp", "nouvelle valeur de ebp" );

  // P_ARGUMENT
  parcours_prefixe_l_dec(  n->u.foncDec_.param, P_ARGUMENT );

  // ALLOC P_VARIABLE_LOCALE
  const unsigned int nbVariablesLocal = parcours_prefixe_l_dec(  n->u.foncDec_.variables, P_VARIABLE_LOCALE );
  if( nbVariablesLocal > 0 )
    assembleur_entry_2s_andComment( ce, "sub", "esp", assembleur_make_constant( nbVariablesLocal * 4 ), "allocation variables locales type 'entier'" );

  //
  parcours_prefixe_l_instr( corps->u.liste );

  //
  tabsymb_dump();

  // où est inserée la fin
  assembleur_entry * addEnd = ( endEntry == NULL ) ? ce : endEntry;

  // DESALLOC P_VARIABLE_LOCALE
  if( nbVariablesLocal > 0 )
    assembleur_entry_2s_andComment( addEnd, "add", "esp", assembleur_make_constant( nbVariablesLocal * 4 ), "desallocation variables locales type 'entier' " );

  //
  assembleur_entry_1s_andComment( addEnd, "pop", "ebp", "restaure la valeur de ebp" );
  assembleur_entry_0s( addEnd, "ret" );

  /** @todo tester si cela ne fait pas de bug avec 'topEntry' plutot que 'ce' */
  // si la fin est séparé
  if( endEntry != NULL )
    assembleur_entry_add_entryPoint_andComment( topEntry, endEntry , "appelé par un retour" );
  //assembleur_entry_add_subEntry( ce, endEntry );


  //
  ce = NULL;
  topEntry = NULL;
  endEntry = NULL;

  //
  tabsymb_sortieFonction();
}


/**
 * @}
 * @defgroup ParcoursFonctionInstruction Instruction
 * @{
 */


unsigned int parcours_prefixe_l_instr( const n_l_instr * n )
{
  if( n )
  {
    parcours_instr( n->tete );
    return parcours_prefixe_l_instr( n->queue ) + 1;
  }
  return 0;
}

unsigned int parcours_suffixe_l_instr( const n_l_instr * n )
{
  if( n )
  {
    const unsigned int p = parcours_suffixe_l_instr( n->queue ) + 1;
    parcours_instr( n->tete );
    return p;
  }
  return 0;
}


/**
* @brief parcours_instr :
*   Seules les instructions de type ecrire et affect seront traitées pendant
*   ce TP. Une instruction ecrire doit être traduite par un appel système int 0x80, une
*   affectation stocke la valeur d’un registre (qui contient le résultat de parcours_exp) dans
*   une adresse mémoire. Les adresses des variables globales simples peuvent être représentées
*   sous forme d’étiquette, tandis que les adresses des tableaux sont aussi des expressions dont
*   la valeur doit être calculée.
*/
void parcours_instr( const n_instr * n )
{
  ErreurSiNulle( n )

  switch( n->type )
  {
  case ecrireInst :
    return parcours_instr_ecrire( n->u.ecrire_.expression );

  case affecteInst :
    return parcours_instr_affect( n->u.affecte_.var, n->u.affecte_.exp );

  case incrInst :
    return parcours_instr_incr( n->u.incr );

  case tantqueInst :
    return parcours_instr_tantque( n->u.tantque_.test, n->u.tantque_.faire );

  case blocInst :
    parcours_prefixe_l_instr( n->u.liste );
    return;

  case appelInst :
    return parcours_instr_appel( n->u.appel );

  case siInst :
    return parcours_instr_si( n->u.si_.test, n->u.si_.alors, n->u.si_.sinon );

  case retourInst :
    return parcours_instr_retour( n->u.retour_.expression );

  case videInst :
    return;

  /** @todo faire 'instruction faire' */
  /** @todo voir pour 'instruction incrInst' */

  default :
    erreurArgs( "Instruction inconnue" );
  }

}


void parcours_instr_ecrire( const n_exp * n )
{
  ErreurSiNulle( n )

  parcours_exp( n );

  assembleur_entry_1s( ce, "pop", "eax" );
  assembleur_entry_1s( ce, "call", "iprintLF" );
}

void parcours_instr_affect( const n_var * var, const n_exp * exp )
{
  ErreurSiNulle( var )
  ErreurSiNulle( exp )

  //
  parcours_exp( exp );

  //
  const char * comment = NULL;
  const char * varAccess = parcoursTools_accesVar( var, &comment ); // 'eax' reservé voir @warning
  if( varAccess == NULL )
    erreurArgs( "Type de Variable inconnue" );

  //
  assembleur_entry_1s( ce, "pop", "ebx" );

  //
  assembleur_entry_2s_andComment(
    ce, "mov", varAccess, "ebx", comment
  );
}


void parcours_instr_incr( const n_var * var )
{
  ErreurSiNulle( var )

  //
  const char * comment = NULL;
  const char * varAccess = parcoursTools_accesVar( var, &comment ); // 'eax' reservé voir @warning
  if( varAccess == NULL )
    erreurArgs( "Type de Variable inconnue" );


  #if PARCOURS_VAR_USE_DWORD==1

  assembleur_entry_2s_andComment(
    ce, "add dword", varAccess, "1", assembleur_make_concat( "incremente : ", comment )
  );

  #else

  //
  parcours_exp_var( var );

  //
  assembleur_entry_1s( ce, "pop", "ebx" );

  //
  assembleur_entry_2s_andComment(
    ce, "add", "ebx", "1", "incremente"
  );

  //
  assembleur_entry_2s_andComment(
    ce, "mov", varAccess, "ebx", comment
  );

  #endif

}


void parcours_instr_tantque( const n_exp * test, const n_instr * faire  )
{
  ErreurSiNulle( test )
  ErreurSiNulle( faire )

  // save parentEntry
  assembleur_entry * parentEntry = ce;

  // new sub-entries
  assembleur_entry * test_subEntry = assembleur_entry_subEntryName_andComment( parentEntry, assembleur_entry_make_preffix( parentEntry , "TqT" ), "tantque : test" );
  assembleur_entry * pushContinue_subEntry = assembleur_entry_subEntryName_andComment( parentEntry, assembleur_entry_make_preffix( parentEntry , "TqV" ), "tantque : valide état"  );
  assembleur_entry * instr_subEntry = assembleur_entry_subEntryName_andComment( parentEntry, assembleur_entry_make_preffix( parentEntry , "TqI" ), "tantque : bloc d'instruction" );

  // continue point
  assembleur_entry * continue_entryPoint = assembleur_new_entryName( assembleur_entry_make_preffix( parentEntry , "R" ) );

  // Gen test_subEntry
  ce = test_subEntry;

  // Gen test
  const char * comparaisonOperation;
  int codeExpr = parcours_exp_comparaison( test, &comparaisonOperation );

  if( codeExpr != 4 ) // si 'constexpr faux' (4) alors ne fait pas le test
    assembleur_entry_1s(
      test_subEntry,
      ( codeExpr == 3 ) ?
      "jmp" : comparaisonOperation,   // si 'constexpr vrai' (3) alors passe directement en pushContinue_subEntry
      assembleur_entry_getName( pushContinue_subEntry )
    );
  assembleur_entry_1s_andComment( test_subEntry, "push", "0", "état 'non-validé'" );
  assembleur_entry_1s( test_subEntry, "jmp", assembleur_entry_getName( instr_subEntry ) );

  // Gen pushContinue_subEntry
  assembleur_entry_1s_andComment( pushContinue_subEntry, "push", "1", "état 'validé'" );

  // Gen instr_subEntry
  assembleur_entry_1s( instr_subEntry, "pop", "eax" );
  assembleur_entry_2s_andComment( instr_subEntry, "cmp", "eax", "00", "check état" );
  assembleur_entry_1s_andComment( instr_subEntry, "jz", assembleur_entry_getName( continue_entryPoint ), "si état non valide, quitte instruction tantque" );
  assembleur_entry_0s( instr_subEntry, "" );
  ce = instr_subEntry;
  parcours_instr( faire );
  assembleur_entry_0s( instr_subEntry, "" );
  assembleur_entry_1s( instr_subEntry, "jmp", assembleur_entry_getName( test_subEntry ) );

  // continue
  assembleur_entry_add_entryPoint_andComment( parentEntry, continue_entryPoint , "retour à la fonction" );
  ce = parentEntry;

}


void parcours_instr_appel( const n_appel * appel  )
{
  ErreurSiNulle( appel )

  //
  parcours_exp_appel( appel, true );
}

void parcours_instr_si( const n_exp * test, const n_instr * alors, const n_instr * sinon )
{
  ErreurSiNulle( test )
  ErreurSiNulle( alors )

  // save parentEntry
  assembleur_entry * parentEntry = ce;

  /** @todo voir si utile de generer le bloc inutilisé dans le cas d'un constexpr
    * par exemple, si la condition ne fait que valider, alors ne pas generer le sinon s'il existe ... */

  // sub-entries (not-implanted into parentEntry)
  assembleur_entry * instrSi_subEntry = assembleur_new_entryName( assembleur_entry_make_preffix( parentEntry , "Si" ) );
  assembleur_entry * instrSinon_subEntry =
    ( sinon == NULL ) ? NULL : assembleur_new_entryName(  assembleur_entry_make_preffix( parentEntry , "Sinon" ) );

  // continue point
  assembleur_entry * continue_entryPoint = assembleur_new_entryName( assembleur_entry_make_preffix( parentEntry , "R" ) );


  //
  const char * elseEntry = ( instrSinon_subEntry == NULL ) ?
                           assembleur_entry_getName( continue_entryPoint ) :
                           assembleur_entry_getName( instrSinon_subEntry );

  // Gen test
  const char * comparaisonOperation;
  int codeExpr = parcours_exp_comparaison( test, &comparaisonOperation );

  if( ( codeExpr == 3 ) || ( codeExpr == 4 ) )   // cas constexpr : 'si' ou ('sinon' ou 'continue')
  {
    //  comme 'si' est tout de suite après, pas besoin de 'jmp'
    if( codeExpr == 4 )
      assembleur_entry_1s( parentEntry, "jmp", elseEntry );
  }
  else
  {
    assembleur_entry_1s( parentEntry, comparaisonOperation, assembleur_entry_getName( instrSi_subEntry ) );
    assembleur_entry_1s( parentEntry, "jmp", elseEntry );
  }

  // Implant sub-entries
  assembleur_entry_add_subEntry_andComment( parentEntry, instrSi_subEntry , "si" );
  if( instrSinon_subEntry != NULL )
    assembleur_entry_add_subEntry_andComment( parentEntry, instrSinon_subEntry , "sinon" );

  // Gen instrSi_subEntry
  ce = instrSi_subEntry;
  parcours_instr( alors );
  if( instrSinon_subEntry != NULL )
  {
    assembleur_entry_0s( instrSi_subEntry, "" );
    assembleur_entry_1s( instrSi_subEntry, "jmp", assembleur_entry_getName( continue_entryPoint ) );
  }

  // Gen instrSinon_subEntry
  if( instrSinon_subEntry != NULL )
  {
    ce = instrSinon_subEntry;
    parcours_instr( sinon );
  }

  // continue
  assembleur_entry_add_entryPoint_andComment( parentEntry, continue_entryPoint , "retour à la fonction" );
  ce = parentEntry;
}



void parcours_instr_retour( const n_exp * expression )
{
  ErreurSiNulle( expression )

  const desc_identif * fonctionCouranteSymb = tabsymb_getSymboleFonctionCourante();
  if( fonctionCouranteSymb == NULL )
    erreurArgs( "Incroyable, vous souhaitez faire un 'retoure' sans être dans une fonction !" );
  //
  parcours_exp( expression );

  //
  assembleur_entry_1s( ce, "pop", "eax" );
  assembleur_entry_2s_andComment(
    ce, "mov",
    assembleur_make_localVar( "ebp", ( 8 + ( fonctionCouranteSymb->complement ) * 4 ) ),
    "eax", "ecriture de la valeur de retour"
  );
  // +8 : ...
  assembleur_entry_1s( ce, "jmp", parcours_get_endEntryName() ); ;
}


/**
 * @}
 * @defgroup ParcoursFonctionExpression Expression
 * @{
 */


unsigned int parcours_prefixe_l_exp( const n_l_exp * n )
{
  if( n )
  {
    parcours_exp( n->tete );
    return parcours_prefixe_l_exp( n->queue ) + 1;
  }
  return 0;
}

unsigned int parcours_suffixe_l_exp( const n_l_exp * n )
{
  if( n )
  {
    const unsigned int p = parcours_suffixe_l_exp( n->queue ) + 1;
    parcours_exp( n->tete );
    return p;
  }
  return 0;
}



/**
* @brief parcours_exp :
*   Cette fonction génère les instructions correspondant au calcul de la valeur
*   de l’expression et dépose le résultat du calcul dans la pile.
*   La génération de code pour une opération de type arg1 op arg2 consiste à affecter à des
*   registres les deux opérandes arg1 et arg2 puis à réaliser l’opération à l’aide de l’instruction
*   correspondante et enfin à empiler le résultat de l’opération.
*   Pour les instructions complexes, les valeurs des opérandes (sous expressions) seront prises
*   dans la pile, c’est là qu’aura été déposé le résultat de ces sous expressions. Il faut aussi traiter
*   les lectures de variables lire avec un appel système.
*/
void parcours_exp( const n_exp * n )
{
  ErreurSiNulle( n )

  //
  switch( n->type )
  {
  case varExp :
    parcours_exp_var( n->u.var );
    break;
  case opExp :
    parcours_exp_op( n, false );
    break;
  case intExp :
    parcours_exp_int( n->u.entier );
    break;
  case appelExp :
    parcours_exp_appel( n->u.appel, false );
    break;
  case lireExp :
    parcours_exp_lire( );
    break;
  default:
    erreurArgs( "Expression inconnue" );
  }

}


/**
 * @brief parcours_exp_comparaison : est executer a partir d'une fonction de comparaison (pas d'opération)
 *          permet de traduire n'import quelle expression en comparaison (comparaison, test vrai...)
 * @param comparaisonOperation : retourne dans un pointrer, l'instruction asm necessaire, à valider la comparaison.
 * @return
 *   - 1 : comparaisonOperation
 *   - 3 : constexpr vrai
 *   - 4 : constexpr faux
 */
int parcours_exp_comparaison( const n_exp * n, const char ** comparaisonOperationPtr )
{
  bool isComparaison;
  int codeExpr;

  //
  if( n->type != opExp )
  {
    parcours_exp( n );
    isComparaison = false;
  }
  else
  {
    codeExpr = parcours_exp_op( n, true );
    isComparaison = !( ( codeExpr == 2 ) || ( codeExpr == 0 ) );
  }

  //
  if( !isComparaison )
  {
    assembleur_entry_1s( ce, "pop", "eax" );
    assembleur_entry_2s_andComment( ce, "cmp", "eax", "00" , " est vrai ?" );
    *comparaisonOperationPtr = parcoursTools_comparaisonOperation( diff );
    return 1;
  }
  else
  {
    *comparaisonOperationPtr = parcoursTools_comparaisonOperation( n->u.opExp_.op );
    return codeExpr;
  }
}


#if IO_USE_READLINE_ENTRY==1
void parcours_exp_lire( )
{
  assembleur_entry_2s( ce, "mov", "eax", "sinput" );
  assembleur_entry_1s( ce, "call", "readline" );
  assembleur_entry_2s( ce, "mov", "eax", "sinput" );
  assembleur_entry_2s( ce, "mov", "eax", "sinput" );
  assembleur_entry_1s( ce, "call", "atoi" );
  assembleur_entry_1s( ce, "push", "eax" );
}
#else
void parcours_exp_lire( )
{
  assembleur_entry_2s( ce, "mov", "ecx", "sinput" );
  assembleur_entry_2s( ce, "mov", "edx", "255" );
  assembleur_entry_2s_andComment( ce, "mov", "eax", "3", "3 est le code de SYS_READ" );
  assembleur_entry_2s_andComment( ce, "mov", "ebx", "0", "0 est le code de STDIN"  );
  assembleur_entry_1s( ce, "int", "80h" );
  assembleur_entry_2s( ce, "mov", "eax", "sinput" );
  assembleur_entry_1s( ce, "call", "atoi" );
  assembleur_entry_1s( ce, "push", "eax" );
}
#endif


void parcours_exp_var( const n_var * variable )
{
  ErreurSiNulle( variable )

  //
  const char * comment = NULL;
  const char * varAccess = parcoursTools_accesVar( variable, &comment ); // 'eax' reservé voir @warning

  if( varAccess == NULL )
    erreurArgs( "Type de Variable inconnue" );

  /** @todo A Confirmer : PUSH direct de la variable, sans passer par MOV */

  #if PARCOURS_VAR_USE_DWORD==1

  assembleur_entry_1s_andComment( ce, "push dword", varAccess, comment );

  #else

  assembleur_entry_2s_andComment(
    ce, "mov", "ebx", varAccess , comment
  );
  assembleur_entry_1s( ce, "push", "ebx" );

  #endif


}

void parcours_exp_int( const int entier )
{
  assembleur_entry_1s( ce, "push", assembleur_make_constant( entier ) );
}


/**
 * @param desallocReturnValue cela désaloue automatiquement la variable de retour avec les arguments (si existant)
 */
void parcours_exp_appel( const n_appel * appel, const bool desallocReturnValue )
{
  ErreurSiNulle( appel )

  const desc_identif * fonctionSymb = tabsymb_getSymbole( appel->fonction );

  // Check...
  if( fonctionSymb == NULL )
    erreurArgs( "Fonction '%s' non déclarée !", appel->fonction );
  else if( fonctionSymb->type != T_FONCTION )
    erreurArgs( "'%s' n'est pas déclarée en tant que fonction", appel->fonction );

  // Valeur de retour
  assembleur_entry_2s_andComment( ce, "sub", "esp", "4", "allocation valeur de retour" );

  // Empiler les arguments dans l'ordre prefixe
  const unsigned int nbArgumentAppel = parcours_prefixe_l_exp( appel->args );

  // Check...
  if( fonctionSymb->complement != nbArgumentAppel )
    erreurArgs( "La fonction '%s' requiert %u argument(s) et non %u", appel->fonction, fonctionSymb->complement, nbArgumentAppel );

  // Appel
  assembleur_entry_1s( ce, "call", appel->fonction );

  // Desallocation parametres (note : valeur de retour non desalloué)
  if( nbArgumentAppel > 0 )
  {
    const unsigned int sizeArguments = 4 * nbArgumentAppel + ( desallocReturnValue ? 4 : 0 );
    assembleur_entry_2s_andComment(
      ce, "add", "esp", assembleur_make_constant( sizeArguments ),
      ( desallocReturnValue ? "'desallocation parametres' et 'valeur de retour ignoree'" : "desallocation parametres" )
    );
    // 4 * nbArgumentAppel : tous les arguments
    // 4 : taille d'un int 32bits
  }
  else if ( desallocReturnValue )
  {
    assembleur_entry_2s_andComment(
      ce, "add", "esp", "4", "valeur de retour ignoree"
    );
    // 4 : taille d'un int 32bits pour valeur de retour
  }
}

/**
 * @brief parcours_exp_op
 * @param isComparaison doit indiquer si l'on ressort avec un 'cmp' (comparaison) ou 'push' (expression)
 * @return
 *   - 0 : computeOperation
 *   - 1 : comparaisonOperation
 *   - 2 : constexpr
 *   - 3 : constexpr vrai
 *   - 4 : constexpr faux
 */
int parcours_exp_op( const n_exp * n, const bool isComparaison )
{
  ErreurSiNulle( n )
  ErreurSiMauvaisType( n, opExp )

  operation op = n->u.opExp_.op;
  n_exp * op1 = n->u.opExp_.op1;
  n_exp * op2 = n->u.opExp_.op2;

  //
  int constexprCompute;
  bool constexprComputeComparaison;
  if( parcoursTools_constexpr( n, &constexprCompute, &constexprComputeComparaison ) )
  {
    if( constexprComputeComparaison && isComparaison )
      return constexprCompute ? 3 : 4;
    else
      assembleur_entry_1s_andComment( ce, "push",  assembleur_make_constant( constexprCompute ), "constexpr !" );
    return 2;
  }

  //
  const bool op1_push = ( op1 != NULL );
  const bool op2_push = ( op2 != NULL );
  if( op1_push ) parcours_exp( op1 );
  if( op2_push ) parcours_exp( op2 );

  //
  if( op2_push )
    assembleur_entry_1s_andComment( ce, "pop", "ebx", "depile la seconde operande dans ebx" );
  if( op1_push )
    assembleur_entry_1s_andComment( ce, "pop", "eax", "depile la permière operande dans eax" );

  //
  const char * regReceiver = NULL;
  bool oneOperandDestination, computeOperationIsCall;
  const char * computeOperation = parcoursTools_computeOperation( op, &regReceiver, &oneOperandDestination, &computeOperationIsCall );
  if( computeOperation != NULL )
  {
    if( computeOperationIsCall )
      assembleur_entry_1s_andComment( ce, "call", computeOperation, "sous fonction d'opération" );
    else if( op2_push && op1_push )
    {
      if( oneOperandDestination )
        assembleur_entry_1s( ce, computeOperation, "ebx" );
      else
        assembleur_entry_2s( ce, computeOperation, "eax", "ebx" );
    }
    else if( op1_push )
      assembleur_entry_1s( ce, computeOperation, "eax" );
    else
      erreurArgs( "Il y à obligatoirement 1 opérande" );
    //
    assembleur_entry_1s_andComment( ce, "push", regReceiver, "empile le résultat" );
    //
    return 0;
  }

  //
  const char * comparaisonOperation = parcoursTools_comparaisonOperation( op );
  if( comparaisonOperation != NULL )
  {
    if( !op2_push && !op1_push )
      erreurArgs( "Il y à obligatoirement 2 opérandes" );
    //

    if( isComparaison )
      assembleur_entry_2s( ce, "cmp", "eax", "ebx" );
    else
    {
      const char * libCompExpr =  parcoursTools_lib_comparatorExpr( op );
      if( libCompExpr == NULL )
        erreurArgs( "Etrange, cette opération de comparaison ne semble pas connu" );

      //
      assembleur_entry_1s_andComment( ce, "call", libCompExpr, "comparaison expression" );
      assembleur_entry_1s( ce, "push", "eax" );
    }
    return 1;
  }

  //
  erreurArgs( "Opération innéxistante !" );
  return -1; // -Wall
}


/**
 * @}
 */


/**
 * @}
 */
