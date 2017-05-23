#include "lexical/symboles.h"
#include "premiers.h"

#include <stdlib.h>
#include <stdio.h>

int premiers[NB_NON_TERMINAUX + 1][NB_TERMINAUX + 1];


int est_premier( int non_terminal, int terminal )
{
  return premiers[terminal][non_terminal];
}

void initialise_premiers_terminaux();
void initialise_premiers_nonterminaux();

void initialise_premiers( void )
{
  int i, j;

  // Initialiser toutes les cases du tableau à 0
  for( i = 0; i <= NB_NON_TERMINAUX; i++ )
    for( j = 0; j <= NB_TERMINAUX; j++ )
      premiers[i][j] = 0;

  initialise_premiers_terminaux();
  initialise_premiers_nonterminaux();

}

/**
 * @defgroup Generation/Remplissage des premiers
 * @{ */

/**
 * @brief premier_valide valide un terminal de la table 'premier' pour un 'non_terminal' donné
 * @param non_terminal le 'non_terminal' qui reçoi le 'terminal' a valider
 * @param terminal le 'terminal' à valider
 */
void premier_valide( int non_terminal, int terminal )
{
  premiers[non_terminal][terminal] = 1;
}

/**
 * @brief premier_union permet de copier un groupe de terminaux de la table 'premier'.
 * @param idSource l'id de la table 'premier' a copier
 * @param idDestination l'id de la table 'premier' qui reçois
 */
void premier_union( int idSource, int idDestination )
{
  //int debugCopie = 0; /**< a enlever après test */
  for( size_t j = 0; j <= NB_TERMINAUX; j++ )
  {
    if( premiers[idSource][j] == 1 )
    {
      premiers[idDestination][j] = 1;
      //debugCopie = 1;
    }
  }
  //if( debugCopie == 0 )
  //  printf( "!!! premier_union(%i,%i) ne fait pas d'union !!! \n", idSource, idDestination );
}

void initialise_premiers_terminaux()
{
  // ODV
  premier_valide( _optDecVariables_, EPSILON );
  // LDVB
  premier_valide( _listeDecVariablesBis_, EPSILON );
  premier_valide( _listeDecVariablesBis_, VIRGULE );
  // DV
  premier_valide( _declarationVariable_, ENTIER );
  // OTT
  premier_valide( _optTailleTableau_, EPSILON );
  premier_valide( _optTailleTableau_, CROCHET_OUVRANT );
  // LDF
  premier_valide( _listeDecFonctions_, EPSILON );
  // DF
  premier_valide( _declarationFonction_, ID_FCT );
  // LP
  premier_valide( _listeParam_, PARENTHESE_OUVRANTE );
  // OLDV
  premier_valide( _optListeDecVariables_, EPSILON );
  // IB
  premier_valide( _instructionBloc_, ACCOLADE_OUVRANTE );
  // LI
  premier_valide( _listeInstructions_, EPSILON );
  // ISI
  premier_valide( _instructionSi_, SI );
  // OSINON
  premier_valide( _optSinon_, EPSILON );
  premier_valide( _optSinon_, SINON );
  // IINCR
  premier_valide( _instructionIncrementer_, INCR );
  // ITQ
  premier_valide( _instructionTantque_, TANTQUE );
  // IF
  premier_valide( _instructionFaire_, FAIRE );
  // IRET
  premier_valide( _instructionRetour_, RETOUR );
  // IECR
  premier_valide( _instructionEcriture_, ECRIRE );
  // IVIDE
  premier_valide( _instructionVide_, POINT_VIRGULE );
  // EXPB
  premier_valide( _expressionBis_, EPSILON );
  premier_valide( _expressionBis_, OU );
  // CONJB
  premier_valide( _conjonctionBis_, EPSILON );
  premier_valide( _conjonctionBis_, ET );
  // COMPB
  premier_valide( _comparaisonBis_, EPSILON );
  premier_valide( _comparaisonBis_, EGAL );
  premier_valide( _comparaisonBis_, DIFFERENT );
  premier_valide( _comparaisonBis_, INFERIEUR );
  premier_valide( _comparaisonBis_, SUPERIEUR );
  premier_valide( _comparaisonBis_, INFERIEUR_EGAL );
  premier_valide( _comparaisonBis_, SUPERIEUR_EGAL );
  // EB
  premier_valide( _expArithBis_, EPSILON );
  premier_valide( _expArithBis_, PLUS );
  premier_valide( _expArithBis_, MOINS );
  // TB
  premier_valide( _termeBis_, FOIS );
  premier_valide( _termeBis_, DIVISE );
  premier_valide( _termeBis_, MODULO );
  premier_valide( _termeBis_, EPSILON );
  // NEG
  premier_valide( _negation_, NON );
  // F
  premier_valide( _facteur_, PARENTHESE_OUVRANTE );
  premier_valide( _facteur_, NOMBRE );
  premier_valide( _facteur_, ID_VAR );
  premier_valide( _facteur_, ID_FCT );
  premier_valide( _facteur_, LIRE );
  // VAR
  premier_valide( _var_, ID_VAR );
  // OIND
  premier_valide( _optIndice_, EPSILON );
  premier_valide( _optIndice_, CROCHET_OUVRANT );
  // APPF
  premier_valide( _appelFct_, ID_FCT );
  // LEXP
  premier_valide( _listeExpressions_, EPSILON );
  // LEXPB
  premier_valide( _listeExpressionsBis_, VIRGULE );
  premier_valide( _listeExpressionsBis_, EPSILON );
}


void initialise_premiers_nonterminaux()
{
  // NEG
  premier_union( _facteur_, _negation_ );
  // T
  premier_union( _negation_, _terme_ );
  // E
  premier_union( _terme_, _expArith_ );
  // CONP
  premier_union( _expArith_, _comparaison_ );
  // CONJ
  premier_union( _comparaison_, _conjonction_ );
  // EXP
  premier_union( _conjonction_, _expression_ );
  // LEXP
  premier_union( _expression_, _listeExpressions_ );
  // IAPP
  premier_union( _appelFct_, _instructionAppel_ );
  // IAFF
  premier_union( _var_, _instructionAffect_ );
  // I
  premier_union( _instructionAffect_, _instruction_ );
  premier_union( _instructionIncrementer_, _instruction_ );
  premier_union( _instructionBloc_, _instruction_ );
  premier_union( _instructionSi_, _instruction_ );
  premier_union( _instructionTantque_, _instruction_ );
  premier_union( _instructionAppel_, _instruction_ );
  premier_union( _instructionRetour_, _instruction_ );
  premier_union( _instructionEcriture_, _instruction_ );
  premier_union( _instructionVide_, _instruction_ );
  premier_union( _instructionFaire_, _instruction_ );
  // LI
  premier_union( _instruction_, _listeInstructions_ );
  // LDF
  premier_union( _declarationFonction_, _listeDecFonctions_ );
  // LDV
  premier_union( _declarationVariable_, _listeDecVariables_ );
  // ODV
  premier_union( _listeDecVariables_, _optDecVariables_ );
  // OLDV
  premier_union( _listeDecVariables_, _optListeDecVariables_ );
  // PG
  premier_union( _optDecVariables_, _programme_ );
}


/** }@  */
