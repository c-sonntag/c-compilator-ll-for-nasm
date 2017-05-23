#ifndef __LIRE_UNITE__
#define __LIRE_UNITE__

#include "stdio.h"

extern char yytext[100];
extern FILE * yyin;
extern int nb_ligne;

#ifndef DOXYGEN_SHOULD_SKIP_THIS

int yylex( void );
void nom_token( int token, char * nom, char * valeur );

void afficherLexique( FILE * yyin, FILE * output );

#endif /* DOXYGEN_SHOULD_SKIP_THIS */


#endif
