#ifndef __MESSAGE__
#define __MESSAGE__


#ifndef DOXYGEN_SHOULD_SKIP_THIS

void warningLigne( char * message );
void erreurLigne( char * message );

void erreur( char * message );
void erreurArgs( const char * fmt, ... );

void warning_1s( char * message, char * s );
void erreur_1s( char * message, char * s );

#endif /* DOXYGEN_SHOULD_SKIP_THIS */


#endif
