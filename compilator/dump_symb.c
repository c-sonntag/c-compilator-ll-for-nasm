#include "syntaxical/premiers.h"
#include "syntaxical/suivants.h"
#include "lexical/symboles.h"
#include "lexical/analyseur_lexical.h"

#include <stdio.h>
#include <stdlib.h>

char yytext[100];
FILE * yyin;


int main()
{
  initialise_premiers();
  initialise_suivants();
  //
  printf( "nPremier :\n" );
  printf( "[Non-Terminaux] : [Terminaux]\n" );
  //
  char nom[100];
  char valeur[100];
  unsigned int i, j;
  //
  for( i = 0; i <= NB_NON_TERMINAUX; i++ )
  {
    printf( "%i : ", i );
    for( j = 0; j <= NB_TERMINAUX; j++ )
    {
      if( premiers[i][j] == 1 )
      {
        nom_token( j, nom, valeur );
        printf( "%s(%s) ", nom, valeur );
      }
    }
    printf( "\n" );
  }
  printf( "\n\nSuivant :\n" );
  printf( "[Non-Terminaux] : [Terminaux]\n" );
  //
  for( i = 0; i <= NB_NON_TERMINAUX; i++ )
  {
    printf( "%i : ", i );
    for( j = 0; j <= NB_TERMINAUX; j++ )
    {
      if( suivants[i][j] == 1 )
      {
        nom_token( j, nom, valeur );
        printf( "%s(%s) ", nom, valeur );
      }
    }
    printf( "\n" );
  }

  return 0;
}
