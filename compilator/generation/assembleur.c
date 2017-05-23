/**
 * @brief Partie génération code assembleur
 *
 * @author released by Yannick Robin
 * @author released by Christophe Sonntag (http://u4a.at)
 *
 * @defgroup GenAsm Génération du code assembleur
 * @{
 */

#include "assembleur.h"
#include "util/circularAdjList.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


/**
 * @defgroup GenAsmLocalListe Gestion des données
 * @{
 */

/**
 * @brief assembleur_data : donnée pour section data
 */
typedef struct
{
  const char * etiquette;
  pseudo_instruction_data pi;
  const char * valeur;
} assembleur_data;


/**
 * @brief assembleur_bss : donnée pour section bss
 */
typedef struct
{
  const char * etiquette;
  pseudo_instruction_bss pi;
  unsigned int nb;
} assembleur_bss;


// struct prototype ...
struct assembleur_entry_instr_circularAdjList;
typedef struct assembleur_entry_instr_circularAdjList assembleur_entry_instr_circularAdjList;

/**
 * @brief assembleur_entry : donnée pour une entrée
 */
typedef struct assembleur_entry
{
  const char * name;
  assembleur_entry_instr_circularAdjList * instrs;
  #if GENASM_ENTRY_PRETTY_NAME==1
  unsigned int counter;
  #endif
} assembleur_entry;


/**
 * @}
 * @defgroup GenAsmEnum
 * @{
 * @defgroup GenAsmEnumStr Tableau des corespondonce enum->str
 * @{
 */

const char * pseudo_instruction_data_str[] =
{
  "db",
  "dw",
  "dd",
  "dq",
  "dt",
};

const char * pseudo_instruction_bss_str[] =
{
  "resb",
  "resw",
  "resd",
  "resq",
  "rest",
};

/*
const char * byte_flag_str[] =
{
  "byte",
  "word",
  "dword",
};
*/

/**
 * @}
 * @}
 * @defgroup GenAsmAdjListGestion Fonction et variable pour la gestion des listes d'adjacence
 * @{
 */

void assembleur_add_commentary( const char * commentary );
const char * assembleur_commentary_next();

/**
 * @brief assembleur_data_circularAdjList Liste d'adjacence pour section data
 */
circularAdjListStruct_header( assembleur_data_circularAdjList )
{
  circularAdjListStruct_content( assembleur_data_circularAdjList )
  assembleur_data * data;
  const char * commentary;
};

/**
 * @brief assembleur_data_circularAdjList Fonction de copie pour assembleur_data_circularAdjList
 */
circularAdjListCopier_header( data_circularAdjList_copier )
{
  circularAdjListTools_contentCast( struct assembleur_data_circularAdjList, assembleur_data )
  head->data = data;
  head->commentary = assembleur_commentary_next();
}

/**
 * @brief data_circularAdjList_comparator Fonction de comparaison pour assembleur_data_circularAdjList
 */
circularAdjListComparator_header( data_circularAdjList_comparator )
{
  circularAdjListTools_contentCast( struct assembleur_data_circularAdjList, assembleur_data )
  return strcmp( head->data->etiquette, data->etiquette ) == 0;
}

/**
 * @brief data_circularAdjList Déclaration de la liste d'adjacence pour section data
 */
circularAdjListStaticDecl_struct( assembleur_data_circularAdjList, data_circularAdjList )

/// ///

/**
 * @brief assembleur_bss_circularAdjList Liste d'adjacence pour section bss
 */
circularAdjListStruct_header( assembleur_bss_circularAdjList )
{
  circularAdjListStruct_content( assembleur_bss_circularAdjList )
  assembleur_bss * bss;
  const char * commentary;
};

/**
 * @brief bss_circularAdjList_copier Fonction de copie pour assembleur_bss_circularAdjList
 */
circularAdjListCopier_header( bss_circularAdjList_copier )
{
  circularAdjListTools_contentCast( struct assembleur_bss_circularAdjList, assembleur_bss )
  head->bss = data;
  head->commentary = assembleur_commentary_next();
}

/**
 * @brief bss_circularAdjList_comparator Fonction de comparaison pour assembleur_bss_circularAdjList
 */
circularAdjListComparator_header( bss_circularAdjList_comparator )
{
  circularAdjListTools_contentCast( struct assembleur_bss_circularAdjList, assembleur_bss )
  return strcmp( head->bss->etiquette, data->etiquette ) == 0;
}

/**
 * @brief bss_circularAdjList Déclaration de la liste d'adjacence pour section bss
 */
circularAdjListStaticDecl_struct( assembleur_bss_circularAdjList, bss_circularAdjList )

/// ///

/**
 * @brief assembleur_entry_circularAdjList Liste d'adjacence pour section data
 */
circularAdjListStruct_header( assembleur_entry_circularAdjList )
{
  circularAdjListStruct_content( assembleur_entry_circularAdjList )
  assembleur_entry * entry;
  const char * commentary;
};

/**
 * @brief entry_circularAdjList_copier Fonction de copie pour assembleur_entry_circularAdjList
 */
circularAdjListCopier_header( entry_circularAdjList_copier )
{
  circularAdjListTools_contentCast( struct assembleur_entry_circularAdjList, assembleur_entry )
  head->entry = data;
  head->commentary = assembleur_commentary_next();
}

/**
 * @brief entry_circularAdjList_comparator Fonction de comparaison pour assembleur_entry_circularAdjList
 */
circularAdjListComparator_header( entry_circularAdjList_comparator )
{
  circularAdjListTools_contentCast( struct assembleur_entry_circularAdjList, assembleur_entry )
  return strcmp( head->entry->name, data->name ) == 0;
}

/**
 * @brief entry_circularAdjList Déclaration de la liste d'adjacence pour section text
 */
circularAdjListStaticDecl_struct( assembleur_entry_circularAdjList, entry_circularAdjList )

/// /// ///

/**
 * @brief The instrType enum : permet d'indiquer quel est le type d'ajout dans la liste d'instruction
 */
enum instrType
{
  instr, subEntry, entryPoint
};

/**
 * @brief assembleur_entry_instr_circularAdjList Liste d'adjacence pour les
 * instructions/sous-entrées/points-entrées dans une d'une entrée
 */
circularAdjListStruct_header( assembleur_entry_instr_circularAdjList )
{
  circularAdjListStruct_content( assembleur_entry_instr_circularAdjList )
  enum instrType type;
  union
  {
    const char * instr;
    assembleur_entry * entry;
  } u;
  const char * commentary;
};

typedef struct assembleur_entry_instr_circularAdjList assembleur_entry_instr_circularAdjList;

/**
 * @brief entry_instr_circularAdjList_copier Fonction de copie pour assembleur_entry_instr_circularAdjList
 */
circularAdjListCopier_header( entry_instr_circularAdjList_copier )
{
  circularAdjListTools_contentCast( struct assembleur_entry_instr_circularAdjList, const char )
  head->type = instr;
  head->u.instr = data;
  head->commentary = assembleur_commentary_next();
}

/**
 * @brief entry_subEntry_circularAdjList_copier Fonction de copie pour assembleur_entry_instr_circularAdjList
 */
circularAdjListCopier_header( entry_subEntry_circularAdjList_copier )
{
  circularAdjListTools_contentCast( struct assembleur_entry_instr_circularAdjList, assembleur_entry )
  head->type = subEntry;
  head->u.entry = data;
  head->commentary = assembleur_commentary_next();
}

/**
 * @brief entry_entryPoint_circularAdjList_copier Fonction de copie pour assembleur_entry_instr_circularAdjList
 */
circularAdjListCopier_header( entry_entryPoint_circularAdjList_copier )
{
  circularAdjListTools_contentCast( struct assembleur_entry_instr_circularAdjList, assembleur_entry )
  head->type = entryPoint;
  head->u.entry = data;
  head->commentary = assembleur_commentary_next();
}


/**
 * @}
 * @defgroup GenAsmLocal Variable de gestion de l'assembleur
 * @{
 */


/**
 * @}
 * @defgroup GenAsmFonctionCom Gestion des commentaires
 * @{
 */

static const char * currentCommentary = NULL;

/**
 * @brief assembleur_entry_lastInstr_setCommentary : ajouter un commentaire pour la prochaine instruction ajouté (si elle existe)
 */
void assembleur_add_commentary( const char * commentary )
{
  currentCommentary = commentary;
}

const char * assembleur_commentary_next()
{
  if( currentCommentary == NULL )
    return NULL;
  else
  {
    const char * lastCommentary = currentCommentary;
    currentCommentary = NULL;
    return lastCommentary;
  }
}


/**
 * @}
 * @defgroup GenAsmFonction Fonction de gestion pour l'assembleur
 * @{
 */


#define SECURE_MALLOC_STRUCT(dataType, varName) \
  dataType * varName = ( dataType * ) malloc( sizeof ( dataType ) ); \
  if( varName == NULL ) { perror( "malloc assembleur struct" ); exit( EXIT_FAILURE ); }


assembleur_entry * assembleur_new_entryName( const char * entryName )
{
  SECURE_MALLOC_STRUCT( assembleur_entry, entry )
  entry->name = entryName;
  entry->instrs = NULL;
  #if GENASM_ENTRY_PRETTY_NAME==1
  entry->counter = 1;
  #endif
  //
  return entry;
}


void assembleur_add_data_andComment( const char * etiquette, const pseudo_instruction_data pi, const char * valeur, const char * comment )
{
  if( comment ) assembleur_add_commentary( comment );
  assembleur_add_data( etiquette, pi, valeur );
}

void assembleur_add_bss_andComment( const char * etiquette, const pseudo_instruction_bss pi, const unsigned int nb, const char * comment )
{
  if( comment ) assembleur_add_commentary( comment );
  assembleur_add_bss( etiquette, pi, nb );
}

void assembleur_add_entry_andComment( assembleur_entry * entry, const char * comment )
{
  if( comment ) assembleur_add_commentary( comment );
  assembleur_add_entry( entry );
}

/**
 * @brief assembleur_entry : généré une entrée entryName et l'ajoute dans la liste des entrées de la section data
 * @param entry
 * @param entryName
 */
assembleur_entry * assembleur_add_entryName_andComment( const char * entryName, const char * comment )
{
  if( comment ) assembleur_add_commentary( comment );
  return assembleur_add_entryName( entryName );
}


/// ///

void assembleur_add_data( const char * etiquette, const pseudo_instruction_data pi, const char * valeur )
{
  SECURE_MALLOC_STRUCT( assembleur_data, data )
  data->etiquette = etiquette;
  data->pi = pi;
  data->valeur = valeur;
  //
  data_circularAdjList = circularAdjList_add( struct assembleur_data_circularAdjList, data_circularAdjList, data, data_circularAdjList_copier, data_circularAdjList_comparator );
}

void assembleur_add_bss( const char * etiquette, const pseudo_instruction_bss pi, const unsigned int nb )
{
  SECURE_MALLOC_STRUCT( assembleur_bss, bss )
  bss->etiquette = etiquette;
  bss->pi = pi;
  bss->nb = nb;
  //
  bss_circularAdjList = circularAdjList_add( struct assembleur_bss_circularAdjList, bss_circularAdjList, bss, bss_circularAdjList_copier, bss_circularAdjList_comparator );
}

void assembleur_add_entry( assembleur_entry * entry )
{
  entry_circularAdjList = circularAdjList_add( struct assembleur_entry_circularAdjList, entry_circularAdjList, entry, entry_circularAdjList_copier, entry_circularAdjList_comparator );
}

assembleur_entry * assembleur_add_entryName( const char * entryName )
{
  assembleur_entry * entry = assembleur_new_entryName( entryName );
  entry_circularAdjList = circularAdjList_add( struct assembleur_entry_circularAdjList, entry_circularAdjList, entry, entry_circularAdjList_copier, entry_circularAdjList_comparator );
  return entry;
}


/**
 * @}
 * @defgroup GenAsmFonctionDump Fonction de génération du flux de sortie
 * @{
 */

#define SIMPLE_ERROR(errStr) \
  { fprintf(stderr, "Erreur Assembleur : %s \n", errStr); exit( EXIT_FAILURE ); }

#define printSpace(space,out) \
  for ( int i = 0; i < space; i++ ) {fputs( " ", out );}

#if GENASM_ENTRY_ARE_INDENT==1
static int dump_marge = 0;
#endif


void assembleur_dump_section_oneEntry( FILE * out , assembleur_entry * entry, const char * commentary );

///

void assembleur_dump_section_import( FILE * out )
{
  fprintf( out, "%%include  'io.asm'\n" );
  fprintf( out, "\n" );
}

void assembleur_dump_section_data( FILE * out )
{
  if( data_circularAdjList == NULL )
    return;
  //
  fprintf( out, "section  .data\n" );
  struct assembleur_data_circularAdjList * head = data_circularAdjList;
  do
  {
    fprintf( out, "%-10s: %-6s %-5s", head->data->etiquette, pseudo_instruction_data_str[head->data->pi], head->data->valeur );
    if( head->commentary != NULL )
      fprintf( out, " ; %s", head->commentary );
    fprintf( out, "\n" );
  }
  while( circularAdjList_doWhileNext( head ) );
  fprintf( out, "\n" );
}

void assembleur_dump_section_bss( FILE * out )
{
  if( bss_circularAdjList == NULL )
    return;
  //
  fprintf( out, "section  .bss\n" );
  struct assembleur_bss_circularAdjList * head = bss_circularAdjList;
  do
  {
    fprintf( out, "%-10s: %-6s %-5u", head->bss->etiquette, pseudo_instruction_bss_str[head->bss->pi], head->bss->nb );
    if( head->commentary != NULL )
      fprintf( out, " ; %s", head->commentary );
    fprintf( out, "\n" );
  }
  while( circularAdjList_doWhileNext( head ) );
  fprintf( out, "\n" );
}

void assembleur_dump_section_entry_instrs( FILE * out, struct assembleur_entry_instr_circularAdjList * instrs )
{
  if( instrs == NULL )
    return;

  struct assembleur_entry_instr_circularAdjList * head = instrs;
  do
  {
    #if GENASM_ENTRY_ARE_INDENT==1
    printSpace( dump_marge, out )
    #endif

    if( head->type == instr )
    {
      fprintf( out, "  %-22s", head->u.instr );
      if( head->commentary != NULL )
        fprintf( out, " ; %s", head->commentary );
      fprintf( out, "\n" );
    }
    else if ( ( head->type == subEntry ) || ( head->type == entryPoint ) )
    {
      fprintf( out, "\n" );

      #if GENASM_ENTRY_ARE_INDENT==1
      if( head->type == subEntry ) dump_marge += 2;
      #endif

      assembleur_dump_section_oneEntry( out, head->u.entry, head->commentary );

      #if GENASM_ENTRYPOINT_SEPERATE_RETURNLINE==1
      if( head->type == entryPoint ) fprintf( out, "\n" );
      #endif

      #if GENASM_ENTRY_ARE_INDENT==1
      if( head->type == subEntry ) dump_marge -= 2;
      #endif
    }
    else
    {
      SIMPLE_ERROR( "Type d'instruction non reconu" )
    }
  }
  while( circularAdjList_doWhileNext( head ) );
}


void assembleur_dump_section_oneEntry( FILE * out , assembleur_entry * entry, const char * commentary )
{
  if( entry == NULL )
    return;

  #if GENASM_ENTRY_ARE_INDENT==1
  printSpace( dump_marge, out )
  #endif

  fprintf( out, "%s:", entry->name );
  if( commentary != NULL )
  {
    // oui, il le faut ! ;-p
    //const int space = 23 - strlen( entry->name );
    //printSpace( space, out )
    printSpace( 23 - strlen( entry->name ), out )
    fprintf( out, " ; %s", commentary );
  }
  fprintf( out, "\n" );

  //
  assembleur_dump_section_entry_instrs( out, entry->instrs );
}

void assembleur_dump_section_entry( FILE * out )
{
  fprintf( out,
           "section  .text\n"
           "global _start\n"
         );
  //
  if( entry_circularAdjList == NULL )
    return;

  //
  struct assembleur_entry_circularAdjList * head = entry_circularAdjList;
  do
  {
    fprintf( out, "\n" );
    assembleur_dump_section_oneEntry( out, head->entry, head->commentary );
  }
  while( circularAdjList_doWhileNext( head ) );

  fprintf( out, "\n" );
}

void assembleur_dump( FILE * out )
{
  assembleur_dump_section_import( out );
  assembleur_dump_section_data( out );
  assembleur_dump_section_bss( out );
  assembleur_dump_section_entry( out );
}


/**
 * @}
 * @defgroup GenAsmFonctionInstr Fonction d'instruction
 * @{
 */

#define CHECK_ENTRY() \
  if(entry==NULL) { fprintf(stderr,"entry ne doit pas être nul\n"); exit( EXIT_FAILURE ); }

// +1 pour le caractère final ('\0') en fin de chaine
#define SECURE_MALLOC_INST_STR(size) \
  char * instStr = ( char * ) malloc( size ); \
  if( instStr == NULL ) { perror( "malloc assembleur entry instr" ); exit( EXIT_FAILURE ); }

#define SECURE_INST_STR( size, pattern, args... ) \
  const unsigned int malloc_size = sizeof(char) * (size + 1); \
  SECURE_MALLOC_INST_STR( malloc_size ) \
  snprintf( instStr, malloc_size, pattern, args );

// pas de comparator = enregistrement direct à la fin de la liste !
#define ADD_ENTRY(newStr) \
  entry->instrs = circularAdjList_add( struct assembleur_entry_instr_circularAdjList, entry->instrs, newStr, entry_instr_circularAdjList_copier, NULL );

// log10 pour conaitre la longeur de 'c' (moins 1) un fois converit en str et si negatif alors abs(c)+1 (pour l'espace)
//#define LOG10(nb) ((nb==0)?1:((unsigned int)log10((nb<0)?(abs(nb)+1):nb)))

/**
 * @brief printedSizeOfSigned trouve la taille à afficher pour un signed int
 * @note si @p val depasse 10 alors log10(p) +1
 * @note si negatif alors doit prendre en compte le '-' (+1)
 */
unsigned int printedSizeOfSigned( const signed int val )
{
  return (
           ( ( val > -10 ) && ( val < 10 ) ) ?
           1 : (
             ( val < 0 ) ?
             ( ( unsigned int ) log10( abs( val ) ) + 1 + 1 ) :
             ( ( unsigned int ) log10( abs( val ) ) + 1 )
           )
         );
}

/**
 * @brief printedSizeOfUnsigned trouve la taille à afficher pour un unsigned int
 * @note si @p val depasse 10 alors log10(p) +1
 */
unsigned int  printedSizeOfUnsigned( const unsigned int val )
{
  return (
           ( val < 10 ) ?
           1 :
           ( ( unsigned int ) log10( abs( val ) ) + 1 )
         );
}


/**
 * @defgroup GenAsmFonctionInstrTools Conversion en str
 * @{
 */

/** @todo gerer la désallocation */
const char * assembleur_make_tabVar( const char * mTab, const char * reg )
{
  SECURE_INST_STR(
    strlen( mTab ) + strlen( reg ) + 5,
    "[%s + %s]",
    mTab, reg
  )
  return instStr;
}

/** @todo gerer la désallocation */
const char * assembleur_make_localVar( const char * reg, const int adr )
{
  const unsigned int adr_u = abs ( adr );
  const char op = ( adr < 0 ) ? '-' : '+';
  //
  SECURE_INST_STR(
    strlen( reg ) + printedSizeOfUnsigned( adr_u ) + 5,
    "[%s %c %u]",
    reg, op, adr_u
  )
  return instStr;
}

/** @todo gerer la désallocation */
const char * assembleur_make_globalVar( const char * m )
{
  SECURE_INST_STR(
    strlen( m ) + 2,
    "[%s]",
    m )
  return instStr;
}

/** @todo gerer la désallocation */
const char * assembleur_make_constant( const int c )
{
  SECURE_INST_STR(
    printedSizeOfSigned( c ),
    "%i",
    c
  )
  return instStr;
}

/** @todo gerer la désallocation */
const char * assembleur_make_idSuffix( const char * prefix, const unsigned int id )
{
  SECURE_INST_STR(
    strlen( prefix ) + printedSizeOfUnsigned( id ) ,
    "%s%u",
    prefix, id
  )
  return instStr;
}

/** @todo gerer la désallocation */
const char * assembleur_make_concat( const char * str1, const char * str2 )
{
  SECURE_INST_STR(
    strlen( str1 ) + strlen( str2 ) ,
    "%s%s",
    str1, str2
  )
  return instStr;
}

#if GENASM_ENTRYCOUNTER_SEPERATE_BY_UNDERSCORE == 1
#define GENASM_ENTRYCOUNTER_UNDERSCORE_SIZE 1
#else
#define GENASM_ENTRYCOUNTER_UNDERSCORE_SIZE 0
#endif


/** @todo gerer la désallocation */
const char * assembleur_entry_make_preffix( assembleur_entry * entry, const char * str )
{
  #if GENASM_ENTRY_PRETTY_NAME==1
  SECURE_INST_STR(
    strlen( entry->name ) + printedSizeOfUnsigned( entry->counter ) + strlen( str ) + 1 + GENASM_ENTRYCOUNTER_UNDERSCORE_SIZE,
    #if GENASM_ENTRYCOUNTER_SEPERATE_BY_UNDERSCORE == 1
    "%s_%u_%s",
    #else
    "%s%u_%s",
    #endif
    entry->name, entry->counter, str
  )
  entry->counter++;
  return instStr;
  #else
  return assembleur_entry_make_next( entry );
  #endif
}

/** @todo gerer la désallocation */
const char * assembleur_entry_make_suffix( assembleur_entry * entry, const char * str )
{
  #if GENASM_ENTRY_PRETTY_NAME==1
  SECURE_INST_STR(
    strlen( entry->name ) + printedSizeOfUnsigned( entry->counter ) + strlen( str ) + 1 + GENASM_ENTRYCOUNTER_UNDERSCORE_SIZE,
    #if GENASM_ENTRYCOUNTER_SEPERATE_BY_UNDERSCORE == 1
    "%s_%s_%u",
    #else
    "%s_%s%u",
    #endif
    str, entry->name, entry->counter
  )
  entry->counter++;
  return instStr;
  #else
  return assembleur_entry_make_next( entry );
  #endif
}

/** @todo gerer la désallocation */
const char * assembleur_entry_make_next( assembleur_entry * entry )
{
  #if GENASM_ENTRY_PRETTY_NAME==1
  SECURE_INST_STR(
    strlen( entry->name ) + printedSizeOfUnsigned( entry->counter ) + GENASM_ENTRYCOUNTER_UNDERSCORE_SIZE ,
    #if GENASM_ENTRYCOUNTER_SEPERATE_BY_UNDERSCORE == 1
    "%s_%u",
    #else
    "%s%u",
    #endif
    entry->name , entry->counter
  )
  entry->counter++;
  #else
  static unsigned int counter = 1;
  SECURE_INST_STR( printedSizeOfUnsigned( counter ) + 1,
                   "e%u",
                   counter
                 )
  counter++;
  #endif
  return instStr;
}



/**
 * @}
 */


void assembleur_entry_0s_andComment( assembleur_entry * entry, const char * instr, const char * comment )
{
  if( comment ) assembleur_add_commentary( comment );
  assembleur_entry_0s( entry, instr );
}


void assembleur_entry_1s_andComment( assembleur_entry * entry, const char * instr, const char * str , const char * comment )
{
  if( comment ) assembleur_add_commentary( comment );
  assembleur_entry_1s( entry, instr, str );
}

void assembleur_entry_2s_andComment( assembleur_entry * entry, const char * instr, const char * str1 , const char * str2 , const char * comment )
{
  if( comment ) assembleur_add_commentary( comment );
  assembleur_entry_2s( entry, instr, str1, str2 );
}

void assembleur_entry_add_subEntry_andComment( assembleur_entry * entry, assembleur_entry * subEntry, const char * comment )
{
  if( comment ) assembleur_add_commentary( comment );
  assembleur_entry_add_subEntry( entry, subEntry );
}

void assembleur_entry_add_entryPoint_andComment( assembleur_entry * entry, assembleur_entry * entryPoint, const char * comment )
{
  if( comment ) assembleur_add_commentary( comment );
  assembleur_entry_add_entryPoint( entry, entryPoint );
}


assembleur_entry * assembleur_entry_subEntryName_andComment( assembleur_entry * entry, const char * subEntryName, const char * comment )
{
  if( comment ) assembleur_add_commentary( comment );
  return assembleur_entry_subEntryName( entry, subEntryName );
}

/// ///

void assembleur_entry_0s( assembleur_entry * entry, const char * instr )
{
  CHECK_ENTRY()
  ADD_ENTRY( instr )
}

void assembleur_entry_1s( assembleur_entry * entry, const char * instr, const char * str )
{
  CHECK_ENTRY()
  SECURE_INST_STR(
    strlen( instr ) + strlen( str ) + 1 + 4 ,
    "%-4s %4s",
    instr, str
  )
  ADD_ENTRY( instStr )
}

void assembleur_entry_2s( assembleur_entry * entry, const char * instr, const char * str1 , const char * str2 )
{
  CHECK_ENTRY()
  SECURE_INST_STR(
    strlen( instr ) + strlen( str1 ) + strlen( str2 ) + 3 + 4 ,
    "%-4s %4s, %s",
    instr, str1, str2
  )
  ADD_ENTRY( instStr )
}

void assembleur_entry_add_subEntry( assembleur_entry * entry, assembleur_entry * subEntry )
{
  CHECK_ENTRY()
  entry->instrs = circularAdjList_add( struct assembleur_entry_instr_circularAdjList, entry->instrs, subEntry, entry_subEntry_circularAdjList_copier, NULL );
}

void assembleur_entry_add_entryPoint( assembleur_entry * entry, assembleur_entry * entryPoint )
{
  CHECK_ENTRY()
  entry->instrs = circularAdjList_add( struct assembleur_entry_instr_circularAdjList, entry->instrs, entryPoint, entry_entryPoint_circularAdjList_copier, NULL );
}

assembleur_entry * assembleur_entry_subEntryName( assembleur_entry * entry, const char * subEntryName )
{
  CHECK_ENTRY()
  assembleur_entry * subEntry = assembleur_new_entryName( subEntryName );
  assembleur_entry_add_subEntry( entry, subEntry );
  return subEntry;
}

/// ///

const char * assembleur_entry_getLastInstr( assembleur_entry * entry )
{
  if( entry->instrs == NULL )
    return NULL;

  //
  if( entry->instrs->prev->type != instr )
    return NULL;

  //
  return entry->instrs->prev->u.instr;
}

const char * assembleur_entry_getName( assembleur_entry * entry )
{
  return entry->name;
}


/**
 * @}
 * @defgroup GenAsmFonctionMemoire Fonction de gestion mémoire
 * @{
 */

//
// #define freeConst(var) free((void*)var)
//
// circularAdjListFreer_header( data_circularAdjList_freer )
// {
//   circularAdjListTools_headCast( struct assembleur_data_circularAdjList )
//   free( head->data );
// }
//
// circularAdjListFreer_header( bss_circularAdjList_freer )
// {
//   circularAdjListTools_headCast( struct assembleur_bss_circularAdjList )
//   free( head->bss );
// }
//
// circularAdjListFreer_header( entry_instr_circularAdjList_freer )
// {
//   circularAdjListTools_headCast( struct assembleur_entry_instr_circularAdjList )
//   if( head->type == instr )
//     freeConst( head->u.instr );
//   else if( ( head->type == subEntry ) || ( head->type == entryPoint ) )
//   {
//     circularAdjList_free( struct assembleur_entry_instr_circularAdjList, head->u.entry->instrs, entry_instr_circularAdjList_freer );
//     free( head->u.entry );
//   }
// }
//
// circularAdjListFreer_header( entry_circularAdjList_freer )
// {
//   circularAdjListTools_headCast( struct assembleur_entry_circularAdjList )
//   circularAdjList_free( struct assembleur_entry_instr_circularAdjList, head->entry->instrs, entry_instr_circularAdjList_freer );
// }
//
// /** @todo Terminer l'implementation pour libere la mémoire */
// void assembleur_free()
// {
//   circularAdjList_free( struct assembleur_data_circularAdjList, data_circularAdjList, data_circularAdjList_freer );
//   circularAdjList_free( struct assembleur_bss_circularAdjList, bss_circularAdjList, bss_circularAdjList_freer );
//   circularAdjList_free( struct assembleur_entry_circularAdjList, entry_circularAdjList, entry_circularAdjList_freer );
// }


/**
 * @}
 */


/**
 * @}
 */
