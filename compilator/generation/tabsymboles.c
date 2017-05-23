/**
 * @brief Partie gestion de la Table des symboles
 *
 * @author Alexis Nasr (http://pageperso.lif.univ-mrs.fr/~alexis.nasr/)
 * @author released by Yannick Robin
 * @author released by Christophe Sonntag (http://u4a.at)
 *
 * @defgroup TabGest Gestion de la table des symboles
 * @{
 */

#include "tabsymboles.h"
#include "util/message.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief tabsymboles : Table des symboles (globale ET locale)
 */

/**
 * @struct tabsymboles_ :  Type 'table des symboles' avec le tableau et deux indices pour le contexte
 */
typedef struct
{
  desc_identif tab[MAX_IDENTIF];
  int base;   /**< base=0 : contexte global, base=sommet contexte local          */
  int sommet; /**< pointe toujours vers la prochaine ligne disponible du tableau */
} tabsymboles_;


static tabsymboles_ tabsymboles;

static bool afficheTable = false;



void tabsymb_setAffiche( bool b )
{
  afficheTable = b;
}



/**
 * @defgroup TabGestFonction Fonction de Gestion
 * @{
 */

/**
 * @brief tabsymb_entreeFonction : Fonction qui bascule table globale -> table locale. À appeler lors qu'on
 * parcourt une déclaration de fonction, juste avant la liste d'arguments.
 */
void tabsymb_entreeFonction( void )
{
  tabsymboles.base = tabsymboles.sommet;
  adresseLocaleCourante = 0;
  adresseArgumentCourant = 0;
}


/**
 * @brief tabsymb_sortieFonction : Fonction qui bascule table locale -> table globale. À appeler lors qu'on
 * a fini de parcourir une déclaration de fonction, après le bloc
 * d'instructions qui constitue le corps de la fonction
 */
void tabsymb_sortieFonction( void )
{
  tabsymboles.sommet = tabsymboles.base;
  tabsymboles.base = 0;
}


/**
 * @brief tabsymb_ajouteIdentificateur : Ajoute un nouvel identificateur à la table des symboles courante.
 *
 * @param identif Nom du nouvel identificateur (variable ou fonction)
 * @param portee Constante parmi P_VARIABLE_GLOBALE, P_ARGUMENT ou
 *               P_VARIABLE_LOCALE
 * @param type Constante parmi T_ENTIER, T_TABLEAU_ENTIER et T_FONCTION
 * @param adresse Nombre d'octets de décalage par rapport à la base de la zone
 *                mémoire des variables ($fp pour locales/arguments, .data pour
 *                globales)
 * @param complement Nombre de paramètres d'une fonction OU nombre de cases
 *                   d'un tableau. Indéfini (0) quand type=T_ENTIER.
 * @return Numéro de ligne de l'entrée qui vient d'être rajoutée
 */
int tabsymb_ajouteIdentificateur( char * identif, int portee, int type,
                                  size_t adresse, size_t complement )
{
  tabsymboles.tab[tabsymboles.sommet].identif = strdup( identif );
  tabsymboles.tab[tabsymboles.sommet].portee = portee;
  tabsymboles.tab[tabsymboles.sommet].type = type;
  tabsymboles.tab[tabsymboles.sommet].adresse = adresse;
  tabsymboles.tab[tabsymboles.sommet].complement = complement;
  tabsymboles.sommet++;

  if( tabsymboles.sommet == MAX_IDENTIF )
    erreur( "Plus de place dans la table des symboles, augmenter MAX_IDENTIF\n" );
  return tabsymboles.sommet - 1;
}


/**
 * @brief tabsymb_rechercheExecutable : Recherche si un identificateur est présent dans la table LOCALE ou GLOBALE
 * Cette fonction doit être utilisée pour s'assurer que tout identificateur
 * utilisé a été déclaré auparavant. Elle doit être utilisée lors de
 * l'UTILISATION d'un identificateur, soit dans un appel à fonction, une
 * partie gauche d'affectation ou une variable dans une expression. Si deux
 * identificateurs ont le même nom dans des portées différentes (un global et
 * un local), la fonction renvoie le plus proche, c-à-d le local.
 * @param identif Le nom de variable ou fonction que l'on cherche
 * @return Si oui, renvoie le numéro de ligne dans tabsymboles, si non -1
 */
int tabsymb_rechercheExecutable( const char * identif )
{
  int i;
  // Parcours dans l'ordre : var. locales, arguments, var. globales
  for( i = tabsymboles.sommet - 1; i >= 0; i-- )
  {
    if( !strcmp( identif, tabsymboles.tab[i].identif ) )
      return i;
  }
  return -1;
}


/**
 * @brief tabsymb_rechercheDeclarative : Recherche si un identificateur est présent dans la table LOCALE
 * Cette fonction doit être utilisée pour s'assurer qu'il n'y a pas deux
 * identificateurs avec le même lors d'une DÉCLARATION d'un identificateur.
 * @param identif Le nom de variable ou fonction que l'on cherche
 * @return Si oui, renvoie le numéro de ligne dans tabsymboles, si non -1
 */
int tabsymb_rechercheDeclarative( const char * identif )
{
  int i;
  for( i = tabsymboles.base; i < tabsymboles.sommet; i++ )
  {
    if( !strcmp( identif, tabsymboles.tab[i].identif ) )
      return i;
  }
  return -1; // Valeur absente
}

/**
 * @brief tabsymb_getSymbole : Trouve le premier exectuable,
 *     voir tabsymb_rechercheExecutable pour plus d'information.
 * @return si le symbole existe alors il est renvoyé, sinon NULL
 */
const desc_identif * tabsymb_getSymbole( const char * nom )
{
  const int symbId = tabsymb_rechercheExecutable( nom );
  //
  if( symbId < 0 )
    return NULL;
  else
    return &tabsymboles.tab[symbId];
}


/**
 * @brief tabsymb_getSymboleFonctionCourante : Trouve  la fonction courante
 * @return si la fonction est trouvé alors elle est renvoyé, sinon NULL
 */
const desc_identif * tabsymb_getSymboleFonctionCourante()
{
  if(tabsymboles.base == 0)
    return NULL;
  //
  const desc_identif * baseSymp = &tabsymboles.tab[tabsymboles.base-1];
  return ( baseSymp->type == T_FONCTION ) ? baseSymp : NULL;
}



/**
 * @brief tabsymb_dump : Fonction auxiliaire qui permet d'afficher le contenu actuel de la table
 * des symboles.
 * @note Cette fonction est conditionné sur une variable booléenne
 * qui contrôle si on veut ou pas afficher la table des symboles en fonction
 * des options passées au compilateur.
 */
void tabsymb_dump( void )
{
  if( !afficheTable )
    return;

  //
  int i;
  printf( "------------------------------------------\n" );
  printf( "base = %d\n", tabsymboles.base );
  printf( "sommet = %d\n", tabsymboles.sommet );
  for( i = 0; i < tabsymboles.sommet; i++ )
  {
    printf( "%d ", i );
    printf( "%s ", tabsymboles.tab[i].identif );

    if( tabsymboles.tab[i].portee == P_VARIABLE_GLOBALE )
      printf( "GLOBALE " );
    else if( tabsymboles.tab[i].portee == P_VARIABLE_LOCALE )
      printf( "LOCALE " );
    else if( tabsymboles.tab[i].portee == P_ARGUMENT )
      printf( "ARGUMENT " );

    if( tabsymboles.tab[i].type == T_ENTIER )
      printf( "ENTIER " );
    else if( tabsymboles.tab[i].type == T_TABLEAU_ENTIER )
      printf( "TABLEAU " );
    else if( tabsymboles.tab[i].type == T_FONCTION )
      printf( "FONCTION " );

    printf( "%u ", tabsymboles.tab[i].adresse );
    printf( "%u\n", tabsymboles.tab[i].complement );
  }
  printf( "------------------------------------------\n" );
}

/**
 * @}
 */

/**
 * @}
 */
