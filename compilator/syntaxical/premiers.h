#ifndef __PREMIERS__
#define __PREMIERS__

#include "lexical/symboles.h"

extern int premiers[NB_NON_TERMINAUX + 1][NB_TERMINAUX + 1];


#ifndef DOXYGEN_SHOULD_SKIP_THIS

void initialise_premiers( void );
int est_premier( int terminal, int non_terminal );

#endif /* DOXYGEN_SHOULD_SKIP_THIS */


#endif
