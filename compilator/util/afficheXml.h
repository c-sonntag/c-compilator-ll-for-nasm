#ifndef __AFFICHE_XML__
#define __AFFICHE_XML__

#include <stdio.h>


#ifndef DOXYGEN_SHOULD_SKIP_THIS

void afficheXml_setOutput( FILE * file  );
void afficheXml_balise_ouvrante( const char * fct_ );
void afficheXml_balise_fermante( const char * fct_ );
void afficheXml_element( char * fct_, char * texte_ );
void afficheXml_texte( char * texte_ );

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif
