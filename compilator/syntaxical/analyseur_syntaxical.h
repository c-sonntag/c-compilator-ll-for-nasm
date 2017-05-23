#ifndef __SYNTAX_UNITE__
#define __SYNTAX_UNITE__

#include "syntabs.h"

#include <stdio.h>
#include <stdbool.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS

  n_prog * programme( void );
  n_l_dec * optDecVariables( void );
  n_l_dec * listeDecVariables( void );
  n_l_dec * listeDecVariablesBis( void );
  n_dec * declarationVariable( void );
  n_dec * optTailleTableau( char * nomVariable );
  n_l_dec * listeDecFonctions( void );
  n_dec * declarationFonction( void );
  n_l_dec * listeParam( void );
  n_l_dec * optListeDecVariables( void );
  n_instr * instruction( void );
  n_instr * instructionAffect( void );
  n_instr * instructionBloc( void );
  n_l_instr * listeInstructions( void );
  n_instr * instructionSi( void );
  n_instr * optSinon( void );
  n_instr * instructionTantque( void );
  n_instr * instructionFaire( void );
  n_instr * instructionAppel( void );
  n_instr * instructionRetour( void );
  n_instr * instructionEcriture( void );
  n_instr * instructionVide( void );
  n_instr * instructionIncrementer( void );
  n_exp * expression( void );
  n_exp * expressionBis( n_exp * op1 );
  n_exp * conjonction( void );
  n_exp * conjonctionBis( n_exp * op1 );
  n_exp * comparaison( void );
  n_exp * comparaisonBis( n_exp * op1 );
  n_exp * expArith( void );
  n_exp * expArithBis( n_exp * op1 );
  n_exp * terme( void );
  n_exp * termeBis( n_exp * op1 );
  n_exp * negation( void );
  n_exp * facteur( void );
  n_var * var( void );
  n_var * optIndice( char * nomVariable );
  n_appel * appelFct( void );
  n_l_exp * listeExpressions( void );
  n_l_exp * listeExpressionsBis( void );

  void syntaxical_setAffiche( bool b );
  n_prog * syntaxical( );

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif
