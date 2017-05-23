/**
 * @defgroup Parcours
 * @{
 */

#ifndef __PARCOURS__
#define __PARCOURS__

#include "syntaxical/syntabs.h"

#include <stdbool.h>

/**
 * @defgroup ParcoursConfig Configuration du module parcours
 * @{
 */

/** permet au programme de renvoyer une valeur */
#define PARCOURS_STARTENTRY_ACCEPT_RETVALUE 1

/** voir dans le code, mais cela evite un instruction mov */
#define PARCOURS_VAR_USE_DWORD 1

/** Inverssement à booléenne */
#define HAVE_LOGIQUE_COMPARAISON 0

/** L'opération de lecture est réalisé par l'entré 'readline' incluse dans le fichier 'io.asm'
 * @note Cela permet de corriger le problème de lecture d'une entrée d'utilisateur,
 *  notament pour l'utilisation de cette valeur obtnu, dans une instruction de division et de modulo
 *  ce qui empeche le problème : 'Exception en point flottant (core dumped)'
 */
#define IO_USE_READLINE_ENTRY 1


/**
 * @}
 */



#ifndef DOXYGEN_SHOULD_SKIP_THIS

void parcours_setAffiche( bool b );
void parcours_prog( const n_prog * n );

#endif /* DOXYGEN_SHOULD_SKIP_THIS */


#endif


/**
 * @}
 */
