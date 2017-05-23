#include <stdio.h>
#include "syntaxical/syntabs.h"
#include "util/afficheXml.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

void afficheArbreAbstrait_n_prog( n_prog * n );
void afficheArbreAbstrait_l_instr( n_l_instr * n );
void afficheArbreAbstrait_instr( n_instr * n );
void afficheArbreAbstrait_instr_si( n_instr * n );
void afficheArbreAbstrait_instr_tantque( n_instr * n );
void afficheArbreAbstrait_instr_faire( n_instr * n );   /* MODIFIE POUR EVAL */
void afficheArbreAbstrait_instr_pour( n_instr * n );    /* MODIFIE POUR EVAL */
void afficheArbreAbstrait_instr_affect( n_instr * n );
void afficheArbreAbstrait_instr_appel( n_instr * n );
void afficheArbreAbstrait_instr_retour( n_instr * n );
void afficheArbreAbstrait_instr_ecrire( n_instr * n );
void afficheArbreAbstrait_l_exp( n_l_exp * n );
void afficheArbreAbstrait_exp( n_exp * n );
void afficheArbreAbstrait_varExp( n_exp * n );
void afficheArbreAbstrait_opExp( n_exp * n );
void afficheArbreAbstrait_intExp( n_exp * n );
void afficheArbreAbstrait_lireExp( n_exp * n );
void afficheArbreAbstrait_appelExp( n_exp * n );
void afficheArbreAbstrait_l_dec( n_l_dec * n );
void afficheArbreAbstrait_dec( n_dec * n );
void afficheArbreAbstrait_foncDec( n_dec * n );
void afficheArbreAbstrait_varDec( n_dec * n );
void afficheArbreAbstrait_tabDec( n_dec * n );
void afficheArbreAbstrait_var( n_var * n );
void afficheArbreAbstrait_var_simple( n_var * n );
void afficheArbreAbstrait_var_indicee( n_var * n );
void afficheArbreAbstrait_appel( n_appel * n );

#endif /* DOXYGEN_SHOULD_SKIP_THIS */


void afficheArbreAbstrait_n_prog( n_prog * n )
{
  const char fct[] = "prog";
  afficheXml_balise_ouvrante( fct );
  afficheArbreAbstrait_l_dec( n->variables );
  afficheArbreAbstrait_l_dec( n->fonctions );
  afficheXml_balise_fermante( fct );
}




void afficheArbreAbstrait_l_instr( n_l_instr * n )
{
  const char fct[] = "l_instr";
  if( n )
  {
    afficheXml_balise_ouvrante( fct );
    afficheArbreAbstrait_instr( n->tete );
    afficheArbreAbstrait_l_instr( n->queue );
    afficheXml_balise_fermante( fct );
  }
}



void afficheArbreAbstrait_instr( n_instr * n )
{
  if( n )
  {
    if( n->type == blocInst ) afficheArbreAbstrait_l_instr( n->u.liste );
    else if( n->type == affecteInst ) afficheArbreAbstrait_instr_affect( n );
    else if( n->type == siInst ) afficheArbreAbstrait_instr_si( n );
    else if( n->type == tantqueInst ) afficheArbreAbstrait_instr_tantque( n );
    else if( n->type == faireInst ) afficheArbreAbstrait_instr_faire( n );
    else if( n->type == appelInst ) afficheArbreAbstrait_instr_appel( n );
    else if( n->type == retourInst ) afficheArbreAbstrait_instr_retour( n );
    else if( n->type == ecrireInst ) afficheArbreAbstrait_instr_ecrire( n );
    // pas dans syntab ... : else if( n->type == pourInst ) afficheArbreAbstrait_instr_pour( n );
  }
}



void afficheArbreAbstrait_instr_si( n_instr * n )
{
  const char fct[] = "instr_si";
  afficheXml_balise_ouvrante( fct );

  afficheArbreAbstrait_exp( n->u.si_.test );
  afficheArbreAbstrait_instr( n->u.si_.alors );
  if( n->u.si_.sinon )
    afficheArbreAbstrait_instr( n->u.si_.sinon );
  afficheXml_balise_fermante( fct );
}



void afficheArbreAbstrait_instr_tantque( n_instr * n )
{
  const char fct[] = "instr_tantque";
  afficheXml_balise_ouvrante( fct );

  afficheArbreAbstrait_exp( n->u.tantque_.test );
  afficheArbreAbstrait_instr( n->u.tantque_.faire );
  afficheXml_balise_fermante( fct );
}



void afficheArbreAbstrait_instr_faire( n_instr * n )       /* MODIFIE POUR EVAL */
{
  /* MODIFIE POUR EVAL */
  /* MODIFIE POUR EVAL */
  const char fct[] = "instr_faire";           /* MODIFIE POUR EVAL */
  afficheXml_balise_ouvrante( fct );             /* MODIFIE POUR EVAL */
  afficheArbreAbstrait_instr( n->u.faire_.faire );         /* MODIFIE POUR EVAL */
  afficheArbreAbstrait_exp( n->u.faire_.test );            /* MODIFIE POUR EVAL */
  afficheXml_balise_fermante( fct );             /* MODIFIE POUR EVAL */
}                                             /* MODIFIE POUR EVAL */



//  void afficheArbreAbstrait_instr_pour( n_instr * n )             /* MODIFIE POUR EVAL */
//  {
//    /* MODIFIE POUR EVAL */
//    const char fct[] = "instr_pour";                       /* MODIFIE POUR EVAL */
//    afficheXml_balise_ouvrante( fct);       /* MODIFIE POUR EVAL */
//    afficheArbreAbstrait_instr( n->u.pour_.init );                /* MODIFIE POUR EVAL */
//    afficheArbreAbstrait_exp( n->u.pour_.test );                  /* MODIFIE POUR EVAL */
//    afficheArbreAbstrait_instr( n->u.pour_.faire );               /* MODIFIE POUR EVAL */
//    afficheArbreAbstrait_instr( n->u.pour_.incr );                /* MODIFIE POUR EVAL */
//    afficheXml_balise_fermante( fct);       /* MODIFIE POUR EVAL */
//  }                                                  /* MODIFIE POUR EVAL */



void afficheArbreAbstrait_instr_affect( n_instr * n )
{
  const char fct[] = "instr_affect";
  afficheXml_balise_ouvrante( fct );
  afficheArbreAbstrait_var( n->u.affecte_.var );
  afficheArbreAbstrait_exp( n->u.affecte_.exp );
  afficheXml_balise_fermante( fct );
}



void afficheArbreAbstrait_instr_appel( n_instr * n )
{
  const char fct[] = "instr_appel";
  afficheXml_balise_ouvrante( fct );


  afficheArbreAbstrait_appel( n->u.appel );
  afficheXml_balise_fermante( fct );
}


void afficheArbreAbstrait_appel( n_appel * n )
{
  const char fct[] = "appel";
  afficheXml_balise_ouvrante( fct );
  afficheXml_texte( n->fonction );
  afficheArbreAbstrait_l_exp( n->args );
  afficheXml_balise_fermante( fct );
}



void afficheArbreAbstrait_instr_retour( n_instr * n )
{
  const char fct[] = "instr_retour";
  afficheXml_balise_ouvrante( fct );
  afficheArbreAbstrait_exp( n->u.retour_.expression );
  afficheXml_balise_fermante( fct );

}



void afficheArbreAbstrait_instr_ecrire( n_instr * n )
{
  const char fct[] = "instr_ecrire";
  afficheXml_balise_ouvrante( fct );
  afficheArbreAbstrait_exp( n->u.ecrire_.expression );
  afficheXml_balise_fermante( fct );
}



void afficheArbreAbstrait_l_exp( n_l_exp * n )
{
  const char fct[] = "l_exp";
  afficheXml_balise_ouvrante( fct );

  if( n )
  {
    afficheArbreAbstrait_exp( n->tete );
    afficheArbreAbstrait_l_exp( n->queue );
  }
  afficheXml_balise_fermante( fct );
}



void afficheArbreAbstrait_exp( n_exp * n )
{
  if( n->type == varExp ) afficheArbreAbstrait_varExp( n );
  else if( n->type == opExp ) afficheArbreAbstrait_opExp( n );
  else if( n->type == intExp ) afficheArbreAbstrait_intExp( n );
  else if( n->type == appelExp ) afficheArbreAbstrait_appelExp( n );
  else if( n->type == lireExp ) afficheArbreAbstrait_lireExp( n );
}



void afficheArbreAbstrait_varExp( n_exp * n )
{
  const char fct[] = "varExp";
  afficheXml_balise_ouvrante( fct );
  afficheArbreAbstrait_var( n->u.var );
  afficheXml_balise_fermante( fct );
}


void afficheArbreAbstrait_opExp( n_exp * n )
{
  const char fct[] = "opExp";
  afficheXml_balise_ouvrante( fct );
  if( n->u.opExp_.op == plus ) afficheXml_texte( "plus" );
  else if( n->u.opExp_.op == moins ) afficheXml_texte( "moins" );
  else if( n->u.opExp_.op == fois ) afficheXml_texte( "fois" );
  else if( n->u.opExp_.op == divise ) afficheXml_texte( "divise" );
  else if( n->u.opExp_.op == modulo ) afficheXml_texte( "modulo" );
  else if( n->u.opExp_.op == egal ) afficheXml_texte( "egal" );
  else if( n->u.opExp_.op == diff ) afficheXml_texte( "diff" );
  else if( n->u.opExp_.op == inf ) afficheXml_texte( "inf" );
  else if( n->u.opExp_.op == infeg ) afficheXml_texte( "infeg" );
  else if( n->u.opExp_.op == sup ) afficheXml_texte( "sup" );
  else if( n->u.opExp_.op == supeg ) afficheXml_texte( "supeg" );
  else if( n->u.opExp_.op == ou ) afficheXml_texte( "ou" );
  else if( n->u.opExp_.op == et ) afficheXml_texte( "et" );
  else if( n->u.opExp_.op == non ) afficheXml_texte( "non" );
  if( n->u.opExp_.op1 != NULL )
    afficheArbreAbstrait_exp( n->u.opExp_.op1 );
  if( n->u.opExp_.op2 != NULL )
    afficheArbreAbstrait_exp( n->u.opExp_.op2 );
  afficheXml_balise_fermante( fct );
}



void afficheArbreAbstrait_intExp( n_exp * n )
{
  char texte[ 50 ]; // Max. 50 chiffres
  sprintf( texte, "%d", n->u.entier );
  afficheXml_element( "intExp", texte );
}


void afficheArbreAbstrait_lireExp( n_exp * n )
{
  const char fct[] = "lireExp";
  afficheXml_balise_ouvrante( fct );
  afficheXml_balise_fermante( fct );

}



void afficheArbreAbstrait_appelExp( n_exp * n )
{
  const char fct[] = "appelExp";
  afficheXml_balise_ouvrante( fct );
  afficheArbreAbstrait_appel( n->u.appel );
  afficheXml_balise_fermante( fct );
}



void afficheArbreAbstrait_l_dec( n_l_dec * n )
{
  const char fct[] = "l_dec";
  if( n )
  {
    afficheXml_balise_ouvrante( fct );
    afficheArbreAbstrait_dec( n->tete );
    afficheArbreAbstrait_l_dec( n->queue );
    afficheXml_balise_fermante( fct );
  }
}



void afficheArbreAbstrait_dec( n_dec * n )
{
  if( n )
  {
    if( n->type == foncDec )
      afficheArbreAbstrait_foncDec( n );
    else if( n->type == varDec )
      afficheArbreAbstrait_varDec( n );
    else if( n->type == tabDec )
      afficheArbreAbstrait_tabDec( n );
  }
}



void afficheArbreAbstrait_foncDec( n_dec * n )
{
  const char fct[] = "foncDec";
  afficheXml_balise_ouvrante( fct );
  afficheXml_texte( n->nom );
  afficheArbreAbstrait_l_dec( n->u.foncDec_.param );
  afficheArbreAbstrait_l_dec( n->u.foncDec_.variables );
  afficheArbreAbstrait_instr( n->u.foncDec_.corps );
  afficheXml_balise_fermante( fct );
}



void afficheArbreAbstrait_varDec( n_dec * n )
{
  afficheXml_element( "varDec", n->nom );
}



void afficheArbreAbstrait_tabDec( n_dec * n )
{
  char texte[100]; // Max. 100 chars nom tab + taille
  sprintf( texte, "%s[%d]", n->nom, n->u.tabDec_.taille );
  afficheXml_element( "tabDec", texte );
}



void afficheArbreAbstrait_var( n_var * n )
{
  if( n->type == simple )
    afficheArbreAbstrait_var_simple( n );
  else if( n->type == indicee )
    afficheArbreAbstrait_var_indicee( n );
}


void afficheArbreAbstrait_var_simple( n_var * n )
{
  afficheXml_element( "var_simple", n->nom );
}


void afficheArbreAbstrait_var_indicee( n_var * n )
{
  const char fct[] = "var_indicee";
  afficheXml_balise_ouvrante( fct );
  afficheXml_element( "var_base_tableau", n->nom );
  afficheArbreAbstrait_exp( n->u.indicee_.indice );
  afficheXml_balise_fermante( fct );
}
