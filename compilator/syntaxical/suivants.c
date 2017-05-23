#include "lexical/symboles.h"
#include "suivants.h"
#include "premiers.h"

#include <stdlib.h>
#include <stdio.h>

int suivants[NB_NON_TERMINAUX + 1][NB_TERMINAUX + 1];

int est_suivant( int terminal, int non_terminal )
{
  return suivants[non_terminal][terminal];
}

void initialise_suivants_terminaux();
void initialise_suivants_nonterminaux();

void initialise_suivants( void )
{
  int i, j;

  // Initialiser toutes les cases du tableau à 0
  for( i = 0; i <= NB_NON_TERMINAUX; i++ )
    for( j = 0; j <= NB_TERMINAUX; j++ )
      suivants[i][j] = 0;


  initialise_suivants_terminaux();
  initialise_suivants_nonterminaux();
}

/**
 * @defgroup Generation/Remplissage des premiers
 * @{ */


/**
 * @brief suivant_valide valide un terminal de la table 'suivant' pour un 'non_terminal' donné
 * @param non_terminal le 'non_terminal' qui reçoi le 'terminal' a valider
 * @param terminal le 'terminal' à valider
 */
void suivant_valide( int non_terminal, int terminal )
{
  suivants[non_terminal][terminal] = 1;
}

/**
 * @brief suivant_union permet de copier un groupe de terminaux de la table 'suivant'.
 * @param idSource l'id de la table 'suivant' a copier
 * @param idDestination l'id de la table 'suivant' qui reçois
 */
void suivant_union( int idSource, int idDestination )
{
  //int debugCopie = 0; /**< a enlever après test */
  for( size_t j = 0; j <= NB_TERMINAUX; j++ )
  {
    if( suivants[idSource][j] == 1 )
    {
      suivants[idDestination][j] = 1;
      //debugCopie = 1;
    }
  }
  //if( debugCopie == 0 )
  //  printf( "!!! suivant_union(%i,%i) ne fait pas d'union !!! \n", idSource, idDestination );
}

/**
 * @brief premier_union_suivant permet de copier un groupe de terminaux de la table 'premiers' vers la table 'suivant'.
 * @param idSourcePremier l'id de la table 'premier' a copier
 * @param idDestinationSuivant l'id de la table 'suivant' qui reçois
 */
void premier_union_suivant( int idSourcePremier, int idDestinationSuivant )
{
  //int debugCopie = 0; /**< a enlever après test */
  for( size_t j = 0; j <= NB_TERMINAUX; j++ )
  {
    if( ( premiers[idSourcePremier][j] == 1 ) && ( j != EPSILON ) )
    {
      suivants[idDestinationSuivant][j] = 1;
      // debugCopie = 1;
    }
  }
  //if( debugCopie == 0 )
  //  printf( "!!! premier_union_suivant(%i,%i) ne fait pas d'union !!! \n", idSourcePremier, idDestinationSuivant );
}



void initialise_suivants_terminaux()
{
  // PG
  suivant_valide( _programme_, FIN );
  // LDV
  suivant_valide( _listeDecVariables_, POINT_VIRGULE );
  // LDF
  suivant_valide( _listeDecFonctions_, FIN );
  // OLDV
  suivant_valide( _optListeDecVariables_, PARENTHESE_FERMANTE );
  // LI
  suivant_valide( _listeInstructions_, ACCOLADE_FERMANTE );

  // EXP
  suivant_valide( _expression_, PARENTHESE_FERMANTE );
  suivant_valide( _expression_, CROCHET_FERMANT );
  suivant_valide( _expression_, POINT_VIRGULE );
  suivant_valide( _expression_, FAIRE );
  suivant_valide( _expression_, ALORS );
  // CONJ
  suivant_valide( _conjonction_, OU );
  // COMP
  suivant_valide( _comparaison_, ET );
  // E
  suivant_valide( _expArith_, EGAL );
  suivant_valide( _expArith_, DIFFERENT );
  suivant_valide( _expArith_, INFERIEUR );
  suivant_valide( _expArith_, SUPERIEUR );
  suivant_valide( _expArith_, INFERIEUR_EGAL );
  suivant_valide( _expArith_, SUPERIEUR_EGAL );
  // T
  suivant_valide( _terme_, PLUS );
  suivant_valide( _terme_, MOINS );
  // TB
  suivant_valide( _termeBis_, FOIS );
  suivant_valide( _termeBis_, DIVISE );
  suivant_valide( _termeBis_, MODULO );
  // APPF
  suivant_valide( _appelFct_, NON );
  // LEXP
  suivant_valide( _listeExpressions_, PARENTHESE_FERMANTE );
  // LEXPB
  suivant_valide( _listeExpressionsBis_, PARENTHESE_FERMANTE );

  // IB
  suivant_valide( _instructionBloc_, TANTQUE );
}


void initialise_suivants_nonterminaux()
{
  // LDV
  suivant_union( _optListeDecVariables_, _listeDecVariables_ );
  // LDVB
  premier_union_suivant( _listeDecVariablesBis_, _listeDecVariablesBis_ );
  suivant_union( _listeDecVariables_, _listeDecVariablesBis_ );
  // DV
  premier_union_suivant( _listeDecVariablesBis_, _declarationVariable_ );
  suivant_union( _listeDecVariables_, _declarationVariable_ );
  // OTT
  premier_union_suivant( _listeDecVariablesBis_, _optTailleTableau_ );
  suivant_union( _listeDecVariables_, _optTailleTableau_ );
  // LDF
  premier_union_suivant( _declarationFonction_, _listeDecFonctions_ );

  // ODV
  premier_union_suivant( _listeDecFonctions_, _optDecVariables_  );
  premier_union_suivant( _instructionBloc_, _optDecVariables_  );
  // LP
  premier_union_suivant( _optDecVariables_, _listeParam_ );

  // I
  premier_union_suivant( _instruction_, _instruction_ );
  suivant_union( _listeInstructions_, _instruction_ );
  // IAFF
  suivant_union( _instruction_, _instructionAffect_ );
  // IINCR
  suivant_union( _instruction_, _instructionAffect_ );
  // IB
  suivant_union( _instruction_, _instructionBloc_ );
  premier_union_suivant( _listeDecFonctions_, _instructionBloc_ );
  // ITQ
  suivant_union( _instruction_, _instructionTantque_ );
  // IF
  suivant_union( _instruction_, _instructionFaire_ );
  // IAPP
  suivant_union( _instruction_, _instructionAppel_ );
  // IRET
  suivant_union( _instruction_, _instructionRetour_ );
  // IECR
  suivant_union( _instruction_, _instructionEcriture_ );
  // IVIDE
  suivant_union( _instruction_, _instructionVide_ );

  // ISI
  suivant_union( _instruction_, _instructionSi_ );
  // OSINON
  suivant_union( _instructionSi_, _optSinon_ );

  // EXP
  premier_union_suivant( _listeExpressionsBis_ , _expression_ );

  // CONJ
  suivant_union( _expression_, _conjonction_ );
  // EXPB
  suivant_union( _conjonction_, _expressionBis_ );

  // COMP
  suivant_union( _conjonction_, _comparaison_ );
  // CONJB
  suivant_union( _comparaison_, _conjonctionBis_ );

  // E
  suivant_union( _comparaison_, _expArith_ );
  // COMPB
  suivant_union( _expArith_, _comparaisonBis_ );

  // EB
  suivant_union( _expArith_, _expArithBis_ );
  // T
  suivant_union( _expArith_, _terme_ );
  premier_union_suivant( _expArith_, _expArithBis_ );
  // TB
  suivant_union( _terme_, _termeBis_ );

  // APPF
  suivant_union( _termeBis_, _appelFct_ );

  // NEG
  suivant_union( _appelFct_, _negation_ );
  // F
  suivant_union( _appelFct_, _facteur_ );

  // OIND
  suivant_union( _appelFct_, _optIndice_ );
  // VAR
  suivant_union( _optIndice_, _var_ );
}


/** }@  */
