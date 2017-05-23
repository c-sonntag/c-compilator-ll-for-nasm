#include "afficheXml.h"

#include "message.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define ErreurSiAucuneSortie() \
  if(sortie_xml_file==NULL) { erreurArgs("Sortie XML necessaire !"); }


static int indent_xml = 0;
static int indent_step = 1; // set to 0 for no indentation
static FILE * sortie_xml_file = NULL;


void afficheXml_setOutput( FILE * file )
{
  sortie_xml_file = file;
}

void indent()
{
  int i;
  for( i = 0; i < indent_xml; i++ )
    fputs ( "  ",  sortie_xml_file );
}


void afficheXml_balise_ouvrante( const char * fct_ )
{
  ErreurSiAucuneSortie()

  //
  indent();
  indent_xml += indent_step ;
  printf ( "<%s>\n", fct_ );
  //fprintf ( stdout, "<%s>\n", fct_ );

}

void afficheXml_balise_fermante( const char * fct_ )
{
  ErreurSiAucuneSortie()

  //
  indent_xml -= indent_step ;
  indent();
  fprintf ( sortie_xml_file, "</%s>\n", fct_ );

}

void afficheXml_texte( char * texte_ )
{
  ErreurSiAucuneSortie()

  //
  indent();
  fprintf ( sortie_xml_file, "%s\n", texte_ );

}

void special_texte( char * texte_ )
{
  int i = 0;
  while( texte_[ i ] != '\0' )
  {
    if( texte_[ i ] == '<' )
      fprintf( sortie_xml_file, "&lt;" );
    else if( texte_[ i ] == '>' )
      fprintf( sortie_xml_file, "&gt;" );
    else if( texte_[ i ] == '&' )
      fprintf( sortie_xml_file, "&amp;" );
    else
      fputc( texte_[i], sortie_xml_file );
    i++;
  }
}

void afficheXml_element( char * fct_, char * texte_ )
{
  ErreurSiAucuneSortie()

  //
  indent();
  fprintf ( sortie_xml_file, "<%s>", fct_ );
  special_texte( texte_ );
  fprintf ( sortie_xml_file, "</%s>\n", fct_ );

}
