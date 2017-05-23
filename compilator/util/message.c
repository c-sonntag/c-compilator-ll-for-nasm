#include "message.h"
#include "lexical/analyseur_lexical.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/**
 * @brief warningLigne : Affiche le message d'alerte donné en paramètre, avec le numéro de ligne
 */
void warningLigne( char * message )
{
  fprintf ( stderr, "WARNING ligne %d : ", nb_ligne );
  fprintf ( stderr, "%s\n", message );
}

/**
 * @brief erreurLigne : Affiche le message d'erreur donné en paramètre, avec le numéro de ligne
 */
void erreurLigne( char * message )
{
  fprintf ( stderr, "\nERREUR ligne %d : ", nb_ligne );
  fprintf ( stderr, "%s\n", message );
  exit( EXIT_FAILURE );
}

/**
 * @brief erreur : Affiche le message d'erreur donné en paramètre
 */
void erreur( char * message )
{
  fprintf ( stderr, "\nERREUR : %s\n", message );
  exit( EXIT_FAILURE );
}

/**
 * @brief  erreur : Affiche le message d'erreur donné en paramètre avec un Wrapper pour printf
 * @author (http://stackoverflow.com/a/14766163/7423251)
 */
void erreurArgs( const char * fmt, ... )
{
  char buffer[4096];
  va_list args;
  va_start( args, fmt );
  /*int rc = */vsnprintf( buffer, sizeof( buffer ), fmt, args );
  va_end( args );
  fprintf ( stderr, "\nERREUR : %s\n", buffer );
  exit( EXIT_FAILURE );
}

/**
 * @brief warning_1s :
 *   Affiche le message d'alerte donné en paramètre, avec le numéro de ligne.
 *   Le message d'alerte peut contenir un %s variable, qui sera donné en
 *   argument @p s
 */
void warning_1s( char * message, char * s )
{
  fprintf ( stderr, "WARNING ligne %d : ", nb_ligne );
  fprintf( stderr, message, s );
  fprintf( stderr, "\n" );
}


/**
 * @brief erreur_1s :
 *   Affiche le message d'erreur donné en paramètre, avec le numéro de ligne.
 *   Le message d'erreur peut contenir un %s variable, qui sera donné en
 *   argument @p s
 */
void erreur_1s( char * message, char * s )
{
  fprintf( stderr, "Ligne %d : ", nb_ligne );
  fprintf( stderr, message, s );
  fprintf( stderr, "\n" );
  exit( 1 );
}
