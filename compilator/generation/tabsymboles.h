/**
 * @defgroup TabGest
 * @{
 */

#ifndef __TABSYMBOLES__
#define __TABSYMBOLES__

#include <stdbool.h>
#include <stddef.h>

/**
 * @defgroup TabGestDef Definition pour la Gestion
 * @{
 */

/** Nombre maximum d'identificateurs (globaux + locaux dans 1 fonction) */
#define MAX_IDENTIF 1000


/**
 * @defgroup TabGestDefPortee Definition des portées
 * @{
 */

#define P_VARIABLE_GLOBALE 1
#define P_VARIABLE_LOCALE 2
#define P_ARGUMENT 3

/**
 * @}
 * @defgroup TabGestDefType Definition des types
 * @{
 */

#define T_ENTIER 1
#define T_TABLEAU_ENTIER 2
#define T_FONCTION 3

/**
 * @}
 */

/**
 * @}
 */

/**
 * @struct desc_identif : Enregistrement décrivant une entrée de la table des symboles
 */

typedef struct
{
  char * identif; /**< Nom de l'identificateur                                             */
  int portee;     /**< Valeurs possibles P_VARIABLE_GLOBALE, P_VARIABLE_LOCALE, P_ARGUMENT */
  int type;       /**< Valeurs possibles T_ENTIER, T_TABLEAU_ENTIER et T_FONCTION          */
  unsigned int adresse;    /**< décalage à partir de $fp ou .data nombre d'octets                   */
  unsigned int complement; /**< taille d'un tableau OU nombre d'arguments d'une fonction            */
} desc_identif;


#ifndef DOXYGEN_SHOULD_SKIP_THIS

void tabsymb_entreeFonction( void );
void tabsymb_sortieFonction( void );

int tabsymb_ajouteIdentificateur( char * identif,  int portee, int type, size_t adresse, size_t complement );

int tabsymb_rechercheExecutable( const char * identif );
int tabsymb_rechercheDeclarative( const char * identif );

const desc_identif * tabsymb_getSymbole( const char * nom );
const desc_identif * tabsymb_getSymboleFonctionCourante();

void tabsymb_dump( void );
void tabsymb_setAffiche(bool b);

#endif /* DOXYGEN_SHOULD_SKIP_THIS */


// Déclaré dans tabsymboles.c
//extern tabsymboles_ tabsymboles;

// À déclarer dans votre module de génération de code
extern int adresseGlobalCourant;
extern int adresseLocaleCourante;
extern int adresseArgumentCourant;
// adresseGlobaleCourante n'est pas utilisée dans tabsymboles.c
// elle peut être locale à votre module de génération de code


#endif

/**
 * @}
 */
