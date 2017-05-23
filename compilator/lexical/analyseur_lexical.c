/**
 * @brief Partie création de la Table des symboles
 *
 * @author Alexis Nasr (http://pageperso.lif.univ-mrs.fr/~alexis.nasr/)
 * @author released by Yannick Robin
 * @author released by Christophe Sonntag (http://u4a.at)
 *
 * @defgroup Lexi Analyseur lexical
 * @{
 */


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "symboles.h"
#include "analyseur_lexical.h"
#include "util/message.h"


/**
 * @defgroup LexiDef Test des catégories
 * @{
 */

#define YYTEXT_MAX 100
#define is_num(c)(('0' <= (c)) && ((c) <= '9'))
#define is_maj(c)(('A' <= (c)) && ((c) <= 'Z'))
#define is_min(c)(('a' <= (c)) && ((c) <= 'z'))
#define is_alpha(c)(is_maj(c) || is_min(c) || (c) == '_' || (c) == '$')
#define is_alpha_simple(c)(is_maj(c) || is_min(c) || (c) == '_')
#define is_alphanum(c)(is_num((c)) || is_alpha((c)))


/**
 * @}
 * @defgroup LexiTab_Operators_1Carac Table Opérateurs 1 Caractere
 * @{
 */

#define MotsClefs_Operators_1Carac_SIZE 15

static char tableMotsClefs_Operators_1Carac[MotsClefs_Operators_1Carac_SIZE] =
{
  ';' , '+' , '-' , '*' , '/', '%' , '(' , ')' , '[' , ']' , '{' , '}' , '&' , '|', ','
};
static int codeMotsClefs_Operators_1Carac[MotsClefs_Operators_1Carac_SIZE] =
{
  POINT_VIRGULE , PLUS , MOINS , FOIS , DIVISE , MODULO, PARENTHESE_OUVRANTE , PARENTHESE_FERMANTE , CROCHET_OUVRANT , CROCHET_FERMANT , ACCOLADE_OUVRANTE , ACCOLADE_FERMANTE , ET , OU , VIRGULE
};


/**
 * @}
 * @defgroup LexiTab_Operators Table des Opérateurs
 * @{
 */

#define MotsClefs_Operators_SIZE 7

// Note : ici certain uni-caractère sont present pour ne pas prendre la priorité depuis la table tableMotsClefs_Operators_1Carac
static char * tableMotsClefs_Operators[MotsClefs_Operators_SIZE] =
{
  "!=", "<=", ">=", "=", "<", ">", "!"
};
static int codeMotsClefs_Operators[MotsClefs_Operators_SIZE] =
{
  DIFFERENT, INFERIEUR_EGAL, SUPERIEUR_EGAL, EGAL , INFERIEUR, SUPERIEUR, NON
};


/**
 * @}
 * @defgroup LexiTab_MotsInstruction Mots instructions
 * @{
 */

#define MotsClefs_Instruction_SIZE 8

static char * tableMotsClefs_Instruction[MotsClefs_Instruction_SIZE] =
{
  "si" , "alors" , "sinon" , "tantque" , "faire" , "entier" , "retour", "incr"
};
static int codeMotsClefs_Instruction[MotsClefs_Instruction_SIZE] =
{
  SI , ALORS , SINON , TANTQUE , FAIRE , ENTIER , RETOUR, INCR
};

/**
 * @}
 * @defgroup LexiTab_MotsFonctions Mots fonction
 * @{
 */


#define MotsClefs_Fonction_SIZE 2

static char * tableMotsClefs_Fonction[MotsClefs_Fonction_SIZE] =
{
  "lire" , "ecrire"
};

static int codeMotsClefs_Fonction[MotsClefs_Fonction_SIZE] =
{
  LIRE , ECRIRE
};

/**
 * @}
 * @defgroup LexiVar Variables du parser yylex
 * @{
 */

char yytext[YYTEXT_MAX];
static int yyleng;

/** Compter les lignes pour afficher les messages d'erreur avec numero ligne */
int nb_ligne = 1;


/**
 * @}
 * @defgroup LexiGestion Fonctions du parser yylex
 * @{
 */

void erreurLexical( const char * typeErreur )
{
  static char erreurStr[512];
  snprintf(
    erreurStr, sizeof( erreurStr ),
    "\n \"%s\" :\n    Texte : %s\n    Longeur : %i",
    typeErreur, yytext, yyleng
  );
  erreurLigne( erreurStr );
}


/**
 * @brief mangeEspaces : Fonction qui ignore les espaces et commentaires.
 * @return Renvoie -1 si arrivé à la fin du fichier, 0 si tout va bien
 */
int mangeEspaces()
{
  char c = fgetc( yyin );
  int comment = 0;
  while( comment || ( c == ' ' ) || ( c == '\n' ) || ( c == '\t' ) || ( c == '#' ) )
  {
    if( c == '#' )
      comment = 1;
    if( c == '\n' )
    {
      nb_ligne++;
      comment = 0;
    }
    c = fgetc( yyin );
  }
  if ( feof( yyin ) )
    return -1;
  ungetc( c, yyin );
  return 0;
}

/**
 * @brief lireCar : Lit un caractère et le stocke dans le buffer yytext
 */
char lireCar( void )
{
  if( ( yyleng + 1 ) >= YYTEXT_MAX )
    erreurLexical( "Chaine trop longue pour le buffer de l'analyseur lexical" );

  //
  yytext[yyleng++] = fgetc( yyin );
  yytext[yyleng] = '\0';
  return yytext[yyleng - 1];
}


/**
 * @brief delireCar : Remet le dernier caractère lu au buffer clavier et enlève du buffer yytext
 */
void delireCar()
{
  char c;
  c = yytext[yyleng - 1];
  yytext[--yyleng] = '\0';
  ungetc( c, yyin );
}

void initBuff()
{
  yytext[yyleng = 0] = '\0';
}


/**
 * @brief estUneVariable : test si la valeur courante correspond à un nom de variable
 * @note un test est effectuer dans yylex avant
 */
bool estUneVariable()
{
  if( yyleng < 1 ) return false;
  if( yytext[0] != '$' ) return false;
  return true;
}


/**
 * @brief estUneFonction : test si la valeur courante correspond à un nom de fonction
 * @note un test est effectuer dans yylex avant
 */
bool estUneFonction()
{
  if( yyleng < 1 ) return false;
  if( !is_alpha_simple( yytext[0] ) ) return false;
  //
  if( mangeEspaces() == 0  )
  {
    char suivant_est_parenthese = lireCar();
    delireCar();
    return ( suivant_est_parenthese == '(' );
  }
  // if
  erreurLexical( "Manque parenthese d'ouverture à la fonction" );
  return 0;
}

/**
 * @brief estUnNombre : test si la valeur courante est nombre
 * @note un test est effectuer dans yylex avant
 * @note on accepte 45aa oû l'analyseur renverra Nombre = 45 et fonction = aa
 */
bool estUnNombre()
{
  if( yyleng < 1 ) return false;
  if( !is_num( yytext[0] ) ) return false;
  //
  for( size_t i = 1; i < yyleng; i++ )
    if( !is_num( yytext[i] ) )
    { delireCar() ; return true;}
  // { erreurLexical( "Nombre incoreccte" ); return false;}
  return true;
}

/**
 * @brief yylex :
 *   Fonction principale de l'analyseur lexical, lit les caractères de yyin et
 *   renvoie les tokens sous forme d'entier. Le code de chaque unité est défini
 *   dans symboles.h sinon (mot clé, idententifiant, etc.). Pour les tokens de
 *   type ID_FCT, ID_VAR et NOMBRE la valeur du token est dans yytext, visible
 *   dans l'analyseur syntaxique.
 */
int yylex( void )
{
  initBuff();
  //
  if( mangeEspaces() == -1 ) // La fin du fichier
    return FIN;
  //
  size_t i;
  char c;
  while( true  )
  {
    c = lireCar();
    // Trouve le mot a 1 caractere ou opérateur à un 1caractère ambigus
    if( yyleng == 1 )
    {
      for( i = 0; i < MotsClefs_Operators_1Carac_SIZE; i++ )
      {
        if( tableMotsClefs_Operators_1Carac[i] == c  )
          return codeMotsClefs_Operators_1Carac[i];
      }

      // Si opérateur ambigus
      lireCar();
      for( i = 0; i < MotsClefs_Operators_SIZE; i++ )
      {
        if( strcmp( tableMotsClefs_Operators[i], yytext ) == 0  )
          return codeMotsClefs_Operators[i];
      }
      delireCar();
    }
    //
    // Test opérateurs avec plus d'un caractére ou pas
    for( i = 0; i < MotsClefs_Operators_SIZE; i++ )
    {
      if( strcmp( tableMotsClefs_Operators[i], yytext ) == 0  )
        return codeMotsClefs_Operators[i];
    }
    //
    // Trouve la chaine alphanumerique
    if( !is_alphanum( c )  )
    {
      delireCar(); // retourne a l'etat : chaine
      //
      // Test utilisateur : variable
      if( estUnNombre() )
        return NOMBRE;
      //
      // Test utilisateur : variable
      if( estUneVariable() )
        return ID_VAR;
      //
      // Test Systeme : instructions
      for( i = 0; i < MotsClefs_Instruction_SIZE; i++ )
      {
        if( strcmp( tableMotsClefs_Instruction[i], yytext ) == 0  )
          return codeMotsClefs_Instruction[i];
      }
      // Test Systeme : fonctions
      for( i = 0; i < MotsClefs_Fonction_SIZE; i++ )
      {
        if( strcmp( tableMotsClefs_Fonction[i], yytext ) == 0  )
          if( estUneFonction() )
            return codeMotsClefs_Fonction[i];
      }
      //
      // Test utilisateur : fonction
      if( estUneFonction() )
        return ID_FCT;
      //
      //
      erreurLexical( "Symbole inconnu" );
    }
  }
}


/**
 * @brief nom_token :
 *   Fonction auxiliaire appelée par l'analyseur syntaxique tout simplement pour
 *   afficher des messages d'erreur et l'arbre XML
 */
void nom_token( int token, char * nom, char * valeur )
{
#define ELIF_TEST_TOKEN(tok) \
  else if( token == tok ) strcpy( valeur, #tok );

  int i;
  strcpy( nom, "symbole" );
  if( false ) {}

  ELIF_TEST_TOKEN( POINT_VIRGULE )
  ELIF_TEST_TOKEN( PLUS )
  ELIF_TEST_TOKEN( MOINS )
  ELIF_TEST_TOKEN( FOIS )
  ELIF_TEST_TOKEN( DIVISE )
  ELIF_TEST_TOKEN( MODULO )
  ELIF_TEST_TOKEN( PARENTHESE_OUVRANTE )
  ELIF_TEST_TOKEN( PARENTHESE_FERMANTE )
  ELIF_TEST_TOKEN( CROCHET_OUVRANT )
  ELIF_TEST_TOKEN( CROCHET_FERMANT )
  ELIF_TEST_TOKEN( ACCOLADE_OUVRANTE )
  ELIF_TEST_TOKEN( ACCOLADE_FERMANTE )
  ELIF_TEST_TOKEN( EGAL )
  ELIF_TEST_TOKEN( DIFFERENT )
  ELIF_TEST_TOKEN( INFERIEUR )
  ELIF_TEST_TOKEN( SUPERIEUR )
  ELIF_TEST_TOKEN( INFERIEUR_EGAL )
  ELIF_TEST_TOKEN( SUPERIEUR_EGAL )
  ELIF_TEST_TOKEN( ET )
  ELIF_TEST_TOKEN( OU )
  ELIF_TEST_TOKEN( NON )
  ELIF_TEST_TOKEN( FIN )
  ELIF_TEST_TOKEN( VIRGULE )

  else if( token == ID_VAR )
  {
    strcpy( nom, "id_variable" );
    strcpy( valeur, yytext );
  }
  else if( token == ID_FCT )
  {
    strcpy( nom, "id_fonction" );
    strcpy( valeur, yytext );
  }
  else if( token == NOMBRE )
  {
    strcpy( nom, "nombre" );
    strcpy( valeur, yytext );
  }
  else
  {
    for( i = 0; i < MotsClefs_Instruction_SIZE; i++ )
    {
      if( token ==  codeMotsClefs_Instruction[i] )
      {
        /** @todo voir pour remettre 'sys_instruction' */
        //strcpy( nom, "sys_instruction" );
        strcpy( nom, "mot_clef" );
        strcpy( valeur, tableMotsClefs_Instruction[i] );
        return;
      }
    }
    for( i = 0; i < MotsClefs_Fonction_SIZE; i++ )
    {
      if( token ==  codeMotsClefs_Fonction[i] )
      {
        //strcpy( nom, "sys_fonction" );
        strcpy( nom, "mot_clef" );
        strcpy( valeur, tableMotsClefs_Fonction[i] );
        return;
      }
    }
    strcpy( nom, "autre" );
    strcpy( valeur, "?" );
  }
}


/**
 * @}
 * @defgroup LexiExec Script d'execution
 * @{
 */

/**
 * @brief afficherLexique :
 *   Fonction auxiliaire appelée par le compilo en mode -l, pour tester l'analyseur
 *   lexical et, étant donné un programme en entrée, afficher la liste des tokens.
 */
void afficherLexique( FILE * yyin, FILE * output )
{
  int uniteCourante;
  char nom[100];
  char valeur[100];
  do
  {
    uniteCourante = yylex();
    nom_token( uniteCourante, nom, valeur );
    fprintf( output, "%s\t%s\t%s\n", yytext, nom, valeur );
  }
  while ( uniteCourante != FIN );
}

/**
 * @}
 */

/**
 * @}
 */
