/**
 * @brief Compilateur
 *
 * @author Alexis Nasr (http://pageperso.lif.univ-mrs.fr/~alexis.nasr/)
 * @author released by Yannick Robin
 * @author released by Christophe Sonntag (http://u4a.at)
 *
 * @defgroup Comp Compilateur
 * @{
 */

#include "lexical/analyseur_lexical.h"
#include "syntaxical/analyseur_syntaxical.h"
#include "syntaxical/syntabs.h"
#include "util/afficheXml.h"
#include "util/afficheArbreAbstrait.h"
#include "generation/parcours.h"
#include "generation/tabsymboles.h"
#include "generation/assembleur.h"


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

char yytext[100];
FILE * yyin = NULL;

static FILE * outputBuffer = NULL;

void afficherAideF()
{
  printf( "Compilateur v1.0 - TP L3 Informatique\n" );
  printf( " créer par : Alexis Nasr (http://pageperso.lif.univ-mrs.fr/~alexis.nasr/)\n" );
  printf( " mis à jour par : - Yannick Robin\n" );
  printf( "                  - Christophe Sonntag (http://u4a.at)\n" );
  printf( "\n" );
  printf( "Usage : ./compilo [<flag-analyse>] [-o <fichier-sortie>] <fichier-entrée> | \n" );
  printf( "                  <fichier-entrée> | -h | --help \n" );
  printf( "\n" );
  printf( "  <flag-analyse> :(optionel) \n" );
  printf( "    -l : afficher l'analyseur lexical \n" );
  printf( "    -s : afficher l'analyseur syntaxique \n" );
  printf( "    -a : afficher l'arbre abstrait \n" );
  printf( "    -t : afficher la table des symboles \n" );
  printf( "    -n : afficher le code NASM \n" );
  printf( "  -o <fichier-sortie> : fichier de sortie (sinon stdout) \n" );
  printf( "  <fichier-entrée> : est un fichier en langage L qui sera analyser\n" );
  printf( "  --help | -h : affiche ce message\n" );
}

/**
 * @brief The ArgsSecond : enum necessaire pour parser la seconde valeur d'un arguments
 */
enum ArgsSecond { ArgsSecond_None, ArgsSecond_outputPath };

int main( int argc, char * argv[] )
{
  bool executionCompilateur = 1;
  bool afficherLex = 0;
  bool afficherSynt = 0;
  bool afficherAsynt = 0;
  bool afficherTableSymbole = 0;
  bool afficherNASM = 0;
  bool afficherAide = 0;
  enum ArgsSecond argSecond = ArgsSecond_None;
  char  * fichierSource = NULL;
  char  * fichierSortie = NULL;
  //
  for( size_t i = 1; i < argc; i++ )
  {
    if( argSecond != ArgsSecond_None )
    {
      if( argSecond == ArgsSecond_outputPath )
        fichierSortie = argv[i];
      //
      argSecond = ArgsSecond_None;
      continue;
    }
    //
    if ( strcmp( argv[i], "-l" ) == 0 )
      afficherLex = 1;
    else if ( strcmp( argv[i], "-s" ) == 0 )
      afficherSynt = 1;
    else if ( strcmp( argv[i], "-a" ) == 0 )
      afficherAsynt = 1;
    else if ( strcmp( argv[i], "-t" ) == 0 )
      afficherTableSymbole = 1;
    else if ( strcmp( argv[i], "-n" ) == 0 )
      afficherNASM = 1;
    else if ( strcmp( argv[i], "-o" ) == 0 )
      argSecond = ArgsSecond_outputPath;
    else if ( ( strcmp( argv[i], "--help" ) == 0  ) || ( strcmp( argv[i], "-h" ) == 0 ) )
      afficherAide = 1;
    else
      fichierSource = argv[i];
  }
  //
  if( afficherAide )
  {
    afficherAideF();
    exit( EXIT_SUCCESS );
  }
  //
  if( argSecond != ArgsSecond_None )
  {
    printf( "Argument second requis !.\n\n" );
    afficherAideF();
    exit( EXIT_FAILURE );
  }
  //
  if( fichierSource == NULL  )
  {
    printf( "<fichier> est inconnu !.\n\n" );
    afficherAideF();
    exit( EXIT_FAILURE );
  }
  //
  yyin = fopen( fichierSource, "r" );
  if( yyin == NULL )
  {
    fprintf( stderr, "impossible d'ouvrir le fichier %s en lecture\n", fichierSource );
    exit( EXIT_FAILURE );
  }
  //
  if( fichierSortie != NULL  )
  {
    outputBuffer = fopen( fichierSortie, "w" );
    if( outputBuffer == NULL )
    {
      fprintf( stderr, "impossible d'ouvrir le fichier %s en écriture\n", fichierSortie );
      exit( EXIT_FAILURE );
    }
  }
  else
    outputBuffer = stdout;
  //
  if( afficherLex )
  {
    rewind( yyin );
    afficherLexique( yyin, outputBuffer );
  }
  //
  if( !executionCompilateur )
    return EXIT_SUCCESS;
  //
  afficheXml_setOutput( outputBuffer ); // pour 'afficherSynt' et 'afficherAsynt'

  //
  syntaxical_setAffiche( afficherSynt );
  tabsymb_setAffiche( afficherTableSymbole );

  //
  rewind( yyin );
  n_prog * arbreAbstrait = syntaxical( yyin );

  //
  fclose( yyin );

  //
  parcours_prog( arbreAbstrait );

  //
  if( afficherAsynt )
    afficheArbreAbstrait_n_prog( arbreAbstrait );

  //
  if( afficherNASM )
    assembleur_dump( outputBuffer );

  //
  /** @todo terminer la désallocation */
  //assembleur_free();
  //free( arbreAbstrait ); /** @todo doit être recursif ... */
  //arbreAbstrait = NULL;

  //
  return EXIT_SUCCESS;
}
