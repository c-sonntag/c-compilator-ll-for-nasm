/* symboles non terminaux */

#define EPSILON 0

#define NB_NON_TERMINAUX 43

#define _listeDecVariables_ 1
#define _listeDecFonctions_ 2
#define _declarationVariable_ 3
#define _declarationFonction_ 4
#define _listeParam_ 5
#define _listeInstructions_ 6
#define _instruction_ 8
#define _instructionAffect_ 9
#define _instructionBloc_ 10
#define _instructionSi_ 11
#define _instructionTantque_ 12
#define _instructionAppel_ 13
#define _instructionRetour_ 14
#define _instructionEcriture_ 15
#define _instructionIncrementer_ 16
#define _instructionVide_ 17
#define _var_ 18
#define _expression_ 19
#define _appelFct_ 20
#define _conjonction_ 21
#define _negation_ 22
#define _comparaison_ 23
#define _expArith_ 24
#define _terme_ 25
#define _facteur_ 26
#define _argumentsEffectifs_ 27
#define _listeExpressions_ 28
#define _listeExpressionsBis_ 29
#define _conjonctionBis_ 30
#define _optTailleTableau_ 31
#define _expArithBis_ 32
#define _optSinon_ 33
#define _comparaisonBis_ 34
#define _optDecVariables_ 35
#define _optIndice_ 36
#define _listeDecVariablesBis_ 37
#define _instructionPour_ 38
#define _termeBis_ 39
#define _expressionBis_ 40
#define _instructionFaire_ 41
#define _optListeDecVariables_ 42
#define _programme_ 43




/* symboles terminaux */
#define NB_TERMINAUX 36

#define POINT_VIRGULE            1
#define PLUS                     2
#define MOINS                    3
#define FOIS                     4
#define DIVISE                   5
#define MODULO                   6
#define PARENTHESE_OUVRANTE      7
#define PARENTHESE_FERMANTE      8
#define CROCHET_OUVRANT          9
#define CROCHET_FERMANT          10
#define ACCOLADE_OUVRANTE        11
#define ACCOLADE_FERMANTE        12
#define EGAL                     13
#define DIFFERENT                14
#define INFERIEUR                15
#define SUPERIEUR                16
#define INFERIEUR_EGAL           17
#define SUPERIEUR_EGAL           18
#define ET                       19
#define OU                       20
#define NON                      21
#define SI                       22
#define ALORS                    23
#define SINON                    24
#define TANTQUE                  25
#define FAIRE                    26
#define ENTIER                   27
#define RETOUR                   28
#define LIRE                     29
#define ECRIRE                   30
#define INCR                     31
#define ID_VAR                   32
#define ID_FCT                   33
#define NOMBRE                   34
#define FIN                      35
#define VIRGULE                  36
