/**
 * @defgroup GenAsm
 * @{
 */

#ifndef __ASSEMBLEUR__
#define __ASSEMBLEUR__

#include <stddef.h>
#include <stdio.h>


/**
 * @defgroup GenAsmConfig Configuration du module Assembleur
 * @{
 */

/**
 * @brief permet d'afficher les entrées en fonction de leur nom et sous instructions..
 * @example une sous boucle d'une fonction main aura les instructions :
 *    main:  ...     [au lieu de]    main:
 *      main0_TqT:  ...                e1:  ...
 *      main1_TqV:  ...                e2:  ...
 *      main2_TqI:  ...                e2:  ...
 * @note se justifie esthétiquement...
 */
#define GENASM_ENTRY_PRETTY_NAME 1


/** @note se justifie esthétiquement... */
#define GENASM_ENTRY_ARE_INDENT 1


/** @note N'est pas très utile, mais peux se justifier esthétiquement... */
#define GENASM_ENTRYCOUNTER_SEPERATE_BY_UNDERSCORE 0

/** @note N'est pas très utile, mais peux se justifier esthétiquement... */
#define GENASM_ENTRYPOINT_SEPERATE_RETURNLINE 0

/**
 * @}
 * @defgroup GenAsmEnum Énumération
 * @{
 */

typedef enum
{
  db = 0, /**< 'define byte' declare un octet */
  dw,     /**< 'define word' declare deux octets */
  dd,     /**< 'define doubleword' declare quatre octets */
  dq,     /**< 'define quadword' declare huit octets */
  dt,     /**< 'define tenbytes' declare dix octets */
} pseudo_instruction_data;

typedef enum
{
  resb = 0, /**< 'reserve byte' declare un octet */
  resw,     /**< 'reserve word' declare deux octets */
  resd,     /**< 'reserve doubleword' declare quatre octets */
  resq,     /**< 'reserve quadword' declare huit octets */
  rest,     /**< 'reserve tenbytes' declare dix octets */
} pseudo_instruction_bss;


typedef enum
{
  byte = 0, /**< un octet*/
  word,     /**< deux octets*/
  dword,    /**< quatre octets*/
} byte_flag;


/**
 * @}
 */


#ifndef DOXYGEN_SHOULD_SKIP_THIS

// struct prototype ...
struct assembleur_entry;
typedef struct assembleur_entry assembleur_entry;

assembleur_entry * assembleur_new_entryName( const char * entryName );

void assembleur_add_data_andComment( const char * etiquette, const pseudo_instruction_data pi, const char * valeur, const char * comment );
void assembleur_add_bss_andComment( const char * etiquette, const pseudo_instruction_bss pi, const unsigned int nb, const char * comment );
void assembleur_add_entry_andComment( assembleur_entry * entry, const char * comment );
assembleur_entry * assembleur_add_entryName_andComment( const char * entryName, const char * comment );

void assembleur_add_data( const char * etiquette, const pseudo_instruction_data pi, const char * valeur );
void assembleur_add_bss( const char * etiquette, const pseudo_instruction_bss pi, const unsigned int nb );
void assembleur_add_entry( assembleur_entry * entry );
assembleur_entry * assembleur_add_entryName( const char * entryName );

void assembleur_dump( FILE * out );

const char * assembleur_make_tabVar( const char * mTab, const char * reg );
const char * assembleur_make_localVar( const char * reg, const int adr );
const char * assembleur_make_globalVar( const char * m );
const char * assembleur_make_constant( const int c );
const char * assembleur_make_idSuffix( const char * prefix, const unsigned int id );
const char * assembleur_make_concat( const char * str1, const char * str2 );

const char * assembleur_entry_make_preffix( assembleur_entry * entry, const char * str );
const char * assembleur_entry_make_suffix( assembleur_entry * entry, const char * str );
const char * assembleur_entry_make_next( assembleur_entry * entry );

void assembleur_entry_0s_andComment( assembleur_entry * entry, const char * instr, const char * comment );
void assembleur_entry_1s_andComment( assembleur_entry * entry, const char * instr, const char * str, const char * comment );
void assembleur_entry_2s_andComment( assembleur_entry * entry, const char * instr, const char * str1 , const char * str2, const char * comment );
void assembleur_entry_add_subEntry_andComment( assembleur_entry * entry, assembleur_entry * subEntry, const char * comment );
void assembleur_entry_add_entryPoint_andComment( assembleur_entry * entry, assembleur_entry * entryPoint, const char * comment );
assembleur_entry * assembleur_entry_subEntryName_andComment( assembleur_entry * entry, const char * subEntryName, const char * comment );

void assembleur_entry_0s( assembleur_entry * entry, const char * instr );
void assembleur_entry_1s( assembleur_entry * entry, const char * instr, const char * str );
void assembleur_entry_2s( assembleur_entry * entry, const char * instr, const char * str1 , const char * str2 );
void assembleur_entry_add_subEntry( assembleur_entry * entry, assembleur_entry * subEntry );
void assembleur_entry_add_entryPoint( assembleur_entry * entry, assembleur_entry * entryPoint );
assembleur_entry * assembleur_entry_subEntryName( assembleur_entry * entry, const char * subEntryName );

const char * assembleur_entry_getLastInstr( assembleur_entry * entry );
const char * assembleur_entry_getName( assembleur_entry * entry );

// void assembleur_free();

#endif /* DOXYGEN_SHOULD_SKIP_THIS */


#endif

/**
 * @}
 */
