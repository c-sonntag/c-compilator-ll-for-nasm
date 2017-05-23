#ifndef __SUIVANTS__
#define __SUIVANTS__

#include "lexical/symboles.h"

extern int suivants[NB_NON_TERMINAUX + 1][NB_TERMINAUX + 1];


#ifndef DOXYGEN_SHOULD_SKIP_THIS

void initialise_suivants( void );
int est_suivant( int terminal, int non_terminal );

#endif /* DOXYGEN_SHOULD_SKIP_THIS */


#endif
