/**
 * @brief Partie syntaxique du compileur
 *
 * @author Alexis Nasr (http://pageperso.lif.univ-mrs.fr/~alexis.nasr/)
 * @author released by Yannick Robin
 * @author released by Christophe Sonntag (http://u4a.at)
 *
 * @defgroup Synt Ananlyse Syntaxique
 * @{
 */

#include "analyseur_syntaxical.h"
#include "premiers.h"
#include "suivants.h"

#include "lexical/symboles.h"
#include "lexical/analyseur_lexical.h"

#include "util/message.h"
#include "util/afficheXml.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @defgroup SyntDefFontion Define des fonctions
 * @{
 */

#define Entre \
  if(afficheArbreSyntaxique) \
    afficheXml_balise_ouvrante(__FUNCTION__);

/**
 * @brief necessite une variable préalablement définit 'retData'
 * @warning Sortie doit imperativement renvoyer une variable et non une fonction !
 * car sinon la fonction est execute lors du 'return' c'est a dire, après la fonction d'affichage 'afficheXml_balise_fermante'
 */

#define Sortie(retData) \
  if(afficheArbreSyntaxique) \
    afficheXml_balise_fermante(__FUNCTION__); \
  return retData;

#define Erreur \
  erreurSyntaxical(__FUNCTION__); \
  return NULL;

/**
 * @}
 */


/**
 * @defgroup SyntVarGlob Variables globales
 * @{
 */

static int uniteCourante = -1;
static char nom[100];
static char valeur[100];

static bool afficheArbreSyntaxique = false;

/**
 * @}
 */

/**
 * @defgroup GestYylex Gestion erreur et yylex
 * @{
 */

/**
 * Passe a l'uniteCourante suivante et
 * affiche le lexique (si existant) dans la sortie (si ok)
 */
void uniteCouranteSuivante()
{
  if( afficheArbreSyntaxique )
    if( uniteCourante != -1 )// Si l'uniteCourante contient un lexique, alors il est affiché
      afficheXml_element( nom, valeur );

  // Avance le lexique
  uniteCourante = yylex();
  // Remplil les varible globale
  nom_token( uniteCourante, nom, valeur );
}

void erreurSyntaxical( const char * fonctionName )
{
  static char erreurStr[512];
  snprintf(
    erreurStr, sizeof( erreurStr ),
    "\n ! Expression mal formée !\n Lexique inconnu dans le grammaire \"%s\" :\n    Nom : %s\n    Valeur : %s",
    fonctionName, nom, valeur
  );
  erreurLigne( erreurStr );
}

/**
 * @}
 */


/**
 * @defgroup SyntAnalyse Analyse
 * @{
 */


/** @todo Ajouter opérateur 'neg' : '-' */


/**
 * @defgroup SyntAnalyseHead Entrée, Variable globale, Fonction, Variable
 * @{
 */

n_prog * programme()
{
  Entre
  if ( est_premier( uniteCourante, _optDecVariables_ )
       || est_premier( uniteCourante, _listeDecFonctions_ )
       || est_suivant( uniteCourante, _programme_ ) )
  {
    n_l_dec * variables = optDecVariables();
    n_l_dec * fonctions = listeDecFonctions();
    //
    n_prog * retour = cree_n_prog( variables, fonctions );
    Sortie( retour )
  }
  Erreur
}

n_l_dec * optDecVariables()
{
  Entre
  if ( est_premier( uniteCourante, _listeDecVariables_ ) )
  {
    n_l_dec * listeVar = listeDecVariables();
    if ( uniteCourante == POINT_VIRGULE )
    {
      uniteCouranteSuivante();
      Sortie( listeVar )
    }
  }
  if ( est_suivant( uniteCourante, _optDecVariables_ ) )
  {
    Sortie ( NULL )
  }
  Erreur
}

n_l_dec * listeDecVariables()
{
  Entre
  if ( est_premier( uniteCourante, _declarationVariable_ ) )
  {
    n_dec * decVariable = declarationVariable();
    n_l_dec * listeDec = listeDecVariablesBis();
    //
    n_l_dec  * retour = cree_n_l_dec( decVariable, listeDec );
    Sortie( retour )
  }
  Erreur
}

n_l_dec * listeDecVariablesBis()
{
  Entre
  if ( uniteCourante == VIRGULE )
  {
    uniteCouranteSuivante();

    n_dec * decVariable = declarationVariable();
    n_l_dec * listeDec = listeDecVariablesBis();
    //
    n_l_dec * retour = cree_n_l_dec( decVariable, listeDec );
    Sortie( retour )
  }
  if ( est_suivant( uniteCourante, _listeDecVariablesBis_ ) )
  {
    Sortie ( NULL )
  }
  Erreur
}

n_dec * declarationVariable()
{
  Entre
  if ( uniteCourante == ENTIER )
  {
    uniteCouranteSuivante();
    if ( uniteCourante == ID_VAR )
    {
      char * nomVariable = strdup ( valeur );
      uniteCouranteSuivante();
      n_dec * decTab = optTailleTableau( nomVariable );
      //
      n_dec  * retour = ( decTab == NULL ) ?  cree_n_dec_var( nomVariable ) : decTab;
      Sortie( retour )
    }
  }
  Erreur
}

n_dec * optTailleTableau( char * nomVariable )
{
  Entre
  if ( uniteCourante == CROCHET_OUVRANT )
  {
    uniteCouranteSuivante();
    if( uniteCourante == NOMBRE )
    {
      int tailleTab = atoi( valeur );
      uniteCouranteSuivante();
      if ( uniteCourante == CROCHET_FERMANT )
      {
        uniteCouranteSuivante();
        //
        n_dec  * retour = cree_n_dec_tab( nomVariable, tailleTab );
        Sortie( retour )
      }
    }
  }
  if ( est_suivant( uniteCourante, _optTailleTableau_ ) )
  {
    Sortie ( NULL )
  }
  Erreur
}

n_l_dec * listeDecFonctions()
{
  Entre
  if ( est_premier( uniteCourante, _declarationFonction_ ) )
  {
    n_dec * decFonctions = declarationFonction();
    n_l_dec * listeFonctions = listeDecFonctions();
    //
    n_l_dec  * retour =  cree_n_l_dec( decFonctions, listeFonctions );
    Sortie( retour )
  }
  else if ( est_suivant( uniteCourante, _listeDecFonctions_ ) )
  {
    Sortie ( NULL )
  }
  Erreur
}

n_dec * declarationFonction()
{
  Entre

  if ( uniteCourante == ID_FCT )
  {
    char * nomFonction = strdup ( valeur );
    uniteCouranteSuivante();
    n_l_dec * lParam = listeParam();
    n_l_dec * oDecVariables = optDecVariables();
    n_instr * corps = instructionBloc();
    //
    n_dec * retour = cree_n_dec_fonc( nomFonction, lParam, oDecVariables, corps );
    Sortie ( retour )
  }
  Erreur
}

n_l_dec * listeParam()
{
  Entre
  if ( uniteCourante == PARENTHESE_OUVRANTE )
  {
    uniteCouranteSuivante();
    n_l_dec * oListeDecVar = optListeDecVariables();
    if ( uniteCourante == PARENTHESE_FERMANTE )
    {
      uniteCouranteSuivante();
      Sortie( oListeDecVar )
    }
  }
  Erreur
}

n_l_dec * optListeDecVariables()
{
  Entre
  if ( est_premier( uniteCourante, _listeDecVariables_ ) )
  {
    n_l_dec * listeDecVar = listeDecVariables();
    Sortie ( listeDecVar )
  }
  else if ( est_suivant( uniteCourante, _optListeDecVariables_ ) )
  {
    Sortie ( NULL )
  }
  Erreur
}

/**
 * @}
 */

/**
 * @defgroup SyntAnalyseInstruction Instruction
 * @{
 */

n_instr * instruction()
{
  Entre
  if ( est_premier( uniteCourante, _instructionAffect_ ) )
  {
    n_instr * affectation = instructionAffect();
    Sortie ( affectation )
  }
  else if ( est_premier( uniteCourante, _instructionBloc_ ) )
  {
    n_instr * bloc = instructionBloc();
    Sortie ( bloc )
  }
  else if ( est_premier( uniteCourante, _instructionSi_ ) )
  {
    n_instr * si = instructionSi();
    Sortie ( si )
  }
  else if ( est_premier( uniteCourante, _instructionTantque_ ) )
  {
    n_instr * tantQue = instructionTantque();
    Sortie ( tantQue )
  }
  else if ( est_premier( uniteCourante, _instructionFaire_ ) )
  {
    n_instr * faire = instructionFaire();
    Sortie ( faire )
  }
  else if ( est_premier( uniteCourante, _instructionAppel_ ) )
  {
    n_instr * appel = instructionAppel();
    Sortie ( appel )
  }
  else if ( est_premier( uniteCourante, _instructionRetour_ ) )
  {
    n_instr * retour = instructionRetour();
    Sortie ( retour )
  }
  else if ( est_premier( uniteCourante, _instructionEcriture_ ) )
  {
    n_instr * ecrire = instructionEcriture();
    Sortie ( ecrire )
  }
  else if ( est_premier( uniteCourante, _instructionVide_ ) )
  {
    n_instr * iVide = instructionVide();
    Sortie ( iVide )
  }
  else if ( est_premier( uniteCourante, _instructionIncrementer_ ) )
  {
      n_instr * incrementer = instructionIncrementer();
      Sortie ( incrementer )
  }
  Erreur
}

n_instr * instructionAffect()
{
  Entre
  if ( est_premier( uniteCourante, _var_ ) )
  {
    n_var * variable = var();
    if ( uniteCourante == EGAL )
    {
      uniteCouranteSuivante();
      n_exp * exp = expression();
      if ( uniteCourante == POINT_VIRGULE )
      {
        uniteCouranteSuivante();
        //
        n_instr * retour = cree_n_instr_affect( variable, exp ) ;
        Sortie( retour )
      }
    }
  }
  Erreur
}

n_instr * instructionBloc()
{
  Entre
  if ( uniteCourante == ACCOLADE_OUVRANTE )
  {
    uniteCouranteSuivante();
    n_l_instr * listeInstr = listeInstructions();
    if ( uniteCourante == ACCOLADE_FERMANTE )
    {
      uniteCouranteSuivante();
      //
      n_instr * retour = cree_n_instr_bloc( listeInstr ) ;
      Sortie( retour )
    }
  }
  Erreur
}


n_l_instr * listeInstructions()
{
  Entre
  if( est_premier( uniteCourante, _instruction_ )  )
  {
    n_instr * instr = instruction();
    n_l_instr * listeInstr = listeInstructions();
    //
    n_l_instr * retour = cree_n_l_instr( instr, listeInstr ) ;
    Sortie( retour )
  }
  else if( est_suivant( uniteCourante, _listeInstructions_ )  )
  {
    Sortie ( NULL )
  }
  Erreur
}

n_instr * instructionSi()
{
  Entre
  if( uniteCourante == SI )
  {
    uniteCouranteSuivante();
    n_exp * exp = expression();
    if( uniteCourante == ALORS )
    {
      uniteCouranteSuivante();
      n_instr * alors = instructionBloc();
      n_instr * oSinon = optSinon();
      //
      n_instr * retour = cree_n_instr_si ( exp, alors, oSinon ) ;
      Sortie( retour )
    }
  }
  Erreur
}

n_instr * optSinon(  )
{
  Entre
  if( uniteCourante == SINON )
  {
    uniteCouranteSuivante();
    //
    n_instr * retour = instructionBloc() ;
    Sortie( retour )
  }
  else if( est_suivant( uniteCourante, _optSinon_ )  )
  {
    Sortie ( NULL )
  }
  Erreur
}

n_instr * instructionTantque()
{
  Entre
  if( uniteCourante == TANTQUE )
  {
    uniteCouranteSuivante();
    n_exp * exp = expression();
    if( uniteCourante == FAIRE )
    {
      uniteCouranteSuivante();
      n_instr * faire = instructionBloc();
      //
      n_instr * retour = cree_n_instr_tantque( exp, faire ) ;
      Sortie( retour )
    }
  }
  Erreur
}

n_instr * instructionFaire()
{
  Entre
  if( uniteCourante == FAIRE )
  {
    uniteCouranteSuivante();
    n_instr * faire = instructionBloc();
    if( uniteCourante == TANTQUE )
    {
      uniteCouranteSuivante();
      n_exp * test = expression();
      if( uniteCourante == POINT_VIRGULE )
      {
        uniteCouranteSuivante();
        //
        n_instr * retour = cree_n_instr_faire( faire, test ) ;
        Sortie( retour )
      }
    }
  }
  Erreur
}

n_instr * instructionAppel()
{
  Entre
  if( est_premier( uniteCourante, _appelFct_ ) )
  {
    n_appel * appel = appelFct();
    if( uniteCourante == POINT_VIRGULE )
    {
      uniteCouranteSuivante();
      //
      n_instr * retour = cree_n_instr_appel( appel ) ;
      Sortie( retour )
    }
  }
  Erreur
}

n_instr * instructionRetour()
{
  Entre
  if( uniteCourante == RETOUR )
  {
    uniteCouranteSuivante();
    n_exp * exp = expression();
    if( uniteCourante == POINT_VIRGULE )
    {
      uniteCouranteSuivante();
      //
      n_instr * retour = cree_n_instr_retour( exp ) ;
      Sortie( retour )
    }
  }
  Erreur
}

n_instr * instructionEcriture()
{
  Entre
  if( uniteCourante == ECRIRE )
  {
    uniteCouranteSuivante();
    if( uniteCourante == PARENTHESE_OUVRANTE )
    {
      uniteCouranteSuivante();
      n_exp * exp = expression();
      if( uniteCourante == PARENTHESE_FERMANTE )
      {
        uniteCouranteSuivante();
        if( uniteCourante == POINT_VIRGULE )
        {
          uniteCouranteSuivante();
          //
          n_instr * retour = cree_n_instr_ecrire ( exp ) ;
          Sortie( retour )
        }
      }
    }
  }
  Erreur
}

n_instr * instructionVide()
{
  Entre
  if( uniteCourante == POINT_VIRGULE )
  {
    uniteCouranteSuivante();
    //
    n_instr * retour = cree_n_instr_vide() ;
    Sortie( retour )
  }
  Erreur
}

n_instr * instructionIncrementer()
{
  Entre
  if ( est_premier( uniteCourante, _instructionIncrementer_ ) )
  {
    if ( uniteCourante == INCR )
    {
      uniteCouranteSuivante();
      if ( est_premier( uniteCourante, _var_ ) )
      {
        n_var * variable = var();
         if ( uniteCourante == POINT_VIRGULE )
         {
            uniteCouranteSuivante();
            //
            n_instr * retour = cree_n_instr_incr(variable) ;
            Sortie( retour )
        }
      }
    }
  }
  Erreur
}



/**
 * @}
 */

/**
 * @defgroup SyntAnalyseExpression Expression
 * @{
 */

n_exp * expression()
{
  Entre
  if( est_premier( uniteCourante, _conjonction_ )  )
  {
    n_exp * op1 = conjonction();
    n_exp * expExpression = expressionBis( op1 );
    Sortie( expExpression )
  }
  Erreur
}

n_exp * expressionBis( n_exp * op1 )
{
  Entre
  if( uniteCourante == OU )
  {
    uniteCouranteSuivante();
    n_exp * op2 = conjonction();
    //
    n_exp * firstExpression = cree_n_exp_op( ou, op1, op2 );
    //
    n_exp * subExpExpression = expressionBis( firstExpression );
    Sortie( subExpExpression )
  }
  else if( est_suivant( uniteCourante, _expressionBis_ )  )
  {
    Sortie( op1 )
  }
  Erreur
}

n_exp * conjonction()
{
  Entre
  if( est_premier( uniteCourante, _comparaison_ )  )
  {
    n_exp * op1 = comparaison();
    n_exp * expConjonction = conjonctionBis( op1 );
    Sortie( expConjonction )
  }
  Erreur
}

n_exp * conjonctionBis( n_exp * op1 )
{
  Entre
  if( uniteCourante == ET )
  {
    uniteCouranteSuivante();
    n_exp * op2 = comparaison();
    //
    n_exp * firstExpression = cree_n_exp_op( et, op1, op2 );
    //
    n_exp * subExpConjonction = conjonctionBis( firstExpression );
    Sortie( subExpConjonction )
  }
  else if( est_suivant( uniteCourante, _conjonctionBis_ )  )
  {
    Sortie( op1 )
  }
  Erreur
}

n_exp * comparaison()
{
  Entre
  if( est_premier( uniteCourante, _expression_ )  )
  {
    n_exp * op1 = expArith();
    n_exp * expComparaison = comparaisonBis( op1 );
    Sortie ( expComparaison )
  }
  Erreur
}

n_exp * comparaisonBis( n_exp * op1 )
{
  Entre

  //
  operation opType;
  switch( uniteCourante )
  {
  case EGAL:
    opType = egal;
    break;
  case DIFFERENT:
    opType = diff;
    break;
  case INFERIEUR:
    opType = inf;
    break;
  case SUPERIEUR:
    opType = sup;
    break;
  case INFERIEUR_EGAL:
    opType = infeg;
    break;
  case SUPERIEUR_EGAL:
    opType = supeg;
    break;

  default:
    if( est_suivant( uniteCourante, _comparaisonBis_ )  )
    { Sortie( op1 ) }
    Erreur
  }

  //
  uniteCouranteSuivante();
  n_exp * op2 = expArith();
  //
  n_exp * firstExpression = cree_n_exp_op( opType, op1, op2 );
  //
  n_exp * subExpComparaison = comparaisonBis( firstExpression );
  Sortie( subExpComparaison )
}


n_exp * expArith()
{
  Entre
  if( est_premier( uniteCourante, _terme_ )  )
  {
    n_exp * op1 = terme();
    n_exp * expArith = expArithBis( op1 );
    Sortie( expArith )
  }
  Erreur
}

n_exp * expArithBis( n_exp * op1 )
{
  Entre
  if( ( uniteCourante == PLUS ) || ( uniteCourante == MOINS ) )
  {
    operation opType = ( uniteCourante == PLUS ) ? plus : moins;
    uniteCouranteSuivante();
    n_exp * op2 = terme();
    //
    n_exp * firstExpression = cree_n_exp_op( opType, op1, op2 );
    //
    n_exp * subExpArith = expArithBis( firstExpression );
    Sortie( subExpArith )
  }
  else if( est_suivant( uniteCourante, _expArithBis_ )  )
  {
    Sortie( op1 )
  }
  Erreur
}

n_exp * terme()
{
  Entre
  if( est_premier( uniteCourante, _negation_ )  )
  {
    n_exp * op1 = negation();
    n_exp * expTerme = termeBis( op1 );
    Sortie( expTerme )
  }
  Erreur
}

n_exp * termeBis( n_exp * op1 )
{
  Entre
  if( ( uniteCourante == FOIS ) || ( uniteCourante == DIVISE ) || ( uniteCourante == MODULO ) )
  {
    const operation opType = ( uniteCourante == FOIS ) ? fois : ( ( uniteCourante == DIVISE ) ? divise : modulo );
    uniteCouranteSuivante();
    n_exp * op2 = negation();
    //
    n_exp * firstExpression = cree_n_exp_op( opType, op1, op2 );
    //
    n_exp * subExpTerme = termeBis( firstExpression );
    Sortie( subExpTerme )
  }
  else if( est_suivant( uniteCourante, _termeBis_ )  )
  {
    Sortie( op1 )
  }
  Erreur
}

n_exp * negation()
{
  Entre
  if( uniteCourante == NON )
  {
    uniteCouranteSuivante();
    n_exp * expNegation = negation();
    //n_exp * expNull = cree_n_exp_entier( 0 );
    //
    n_exp * retour = cree_n_exp_op( non, expNegation, NULL ) ;
    Sortie( retour )
  }
  else if( est_premier( uniteCourante , _facteur_ ) )
  {
    //
    n_exp * retour = facteur() ;
    Sortie( retour )
  }
  Erreur
}

n_exp * facteur()
{
  Entre
  if( uniteCourante == PARENTHESE_OUVRANTE )
  {
    uniteCouranteSuivante();
    n_exp * expParenthesee = expression();
    if( uniteCourante == PARENTHESE_FERMANTE )
    {
      uniteCouranteSuivante();
      Sortie( expParenthesee )
    }
  }
  else if( uniteCourante == NOMBRE )
  {
    int nombre = atoi( valeur );
    uniteCouranteSuivante();
    //
    n_exp * retour = cree_n_exp_entier( nombre ) ;
    Sortie( retour )
  }
  else if( est_premier( uniteCourante, _appelFct_ ) )
  {
    //
    n_exp * retour = cree_n_exp_appel( appelFct() ) ;
    Sortie( retour )
  }
  else if( est_premier( uniteCourante, _var_ ) )
  {
    //
    n_exp * retour = cree_n_exp_var( var() ) ;
    Sortie( retour )
  }
  else if( uniteCourante == LIRE )
  {
    uniteCouranteSuivante();
    if( uniteCourante == PARENTHESE_OUVRANTE )
    {
      uniteCouranteSuivante();
      if( uniteCourante == PARENTHESE_FERMANTE )
      {
        uniteCouranteSuivante();
        //
        n_exp * retour = cree_n_exp_lire() ;
        Sortie( retour )
      }
    }
  }
  Erreur
}

n_var * var()
{
  Entre
  if( uniteCourante == ID_VAR )
  {
    char * nomVariable = strdup( valeur );
    uniteCouranteSuivante();
    n_var * varTableau = optIndice( nomVariable );
    //
    n_var * retour = ( varTableau == NULL ) ? cree_n_var_simple( nomVariable ) : varTableau ;
    Sortie( retour )
  }
  Erreur
}

n_var * optIndice( char * nomVariable )
{
  Entre
  if( uniteCourante == CROCHET_OUVRANT )
  {
    uniteCouranteSuivante();
    n_exp * expIndice = expression();
    if( uniteCourante == CROCHET_FERMANT )
    {
      uniteCouranteSuivante();
      //
      n_var * retour = cree_n_var_indicee( nomVariable, expIndice ) ;
      Sortie( retour )
    }
  }
  else if( est_suivant( uniteCourante, _optIndice_ ) )
  {
    Sortie( NULL )
  }
  Erreur
}

n_appel * appelFct()
{
  Entre
  if( uniteCourante == ID_FCT )
  {
    char * nomFonction = strdup( valeur );
    uniteCouranteSuivante();
    if( uniteCourante == PARENTHESE_OUVRANTE )
    {
      uniteCouranteSuivante();
      n_l_exp * listeArgs = listeExpressions();
      if( uniteCourante == PARENTHESE_FERMANTE )
      {
        uniteCouranteSuivante();
        //
        n_appel * retour = cree_n_appel( nomFonction, listeArgs ) ;
        Sortie( retour )
      }
    }
  }
  Erreur
}


n_l_exp * listeExpressions()
{
  Entre
  if( est_premier( uniteCourante, _expression_ ) )
  {
    n_exp * expTete = expression();
    n_l_exp * expQueud = listeExpressionsBis();
    //
    n_l_exp * retour = cree_n_l_exp( expTete, expQueud ) ;
    Sortie( retour )
  }
  else if( est_suivant( uniteCourante, _listeExpressions_ ) )
  {
    Sortie( NULL )
  }
  Erreur
}

n_l_exp * listeExpressionsBis()
{
  Entre
  if( uniteCourante == VIRGULE )
  {
    uniteCouranteSuivante();
    n_exp * expTete = expression();
    n_l_exp * expQueud = listeExpressionsBis();
    //
    n_l_exp * retour = cree_n_l_exp( expTete, expQueud ) ;
    Sortie( retour )
  }
  else if( est_suivant( uniteCourante, _listeExpressionsBis_ ) )
  {
    Sortie( NULL )
  }
  Erreur
}

/**
 * @}
 */

/**
 * @}
 */


/**
 * @brief syntaxicalAffiche : spécifier si l'arbre syntaxique doit s'afficher
 */
void syntaxical_setAffiche( bool b )
{
  afficheArbreSyntaxique = b;
}


/**
 * @brief syntaxical : appel du parseur syntaxique
 * @param yyin le fichier a parcourir
 * @param afficherSynt
 * @return
 */
n_prog * syntaxical( )
{
  initialise_premiers();
  initialise_suivants();
  //
  uniteCouranteSuivante();
  n_prog * p = programme();
  return p;
}

/**
 * @}
 */
