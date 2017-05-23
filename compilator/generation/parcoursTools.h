#ifndef __PARCOURS_TOOLS__
#define __PARCOURS_TOOLS__

#include "syntaxical/syntabs.h"

#include <stdbool.h>
#include <stddef.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS

//int parcoursTools_constexpr( operation op, int op1, int op2 );

bool parcoursTools_constexpr( const n_exp * n, int * compute,  bool * computeComparaison );

const char * parcoursTools_computeOperation( const operation op, const char ** reg, bool * oneOperandDestination, bool * isCall );
const char * parcoursTools_comparaisonOperation( const operation op );

size_t parcoursTools_taille_n_l( n_l_dec * n );

const char * parcoursTools_accesVar( const n_var * variable, const char ** comment );

void parcoursTools_dump_asm_lib();

const char * parcoursTools_lib_comparatorExpr( operation op );
const char * parcoursTools_lib_comparaisonBool( operation op );

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif
