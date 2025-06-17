%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

ASTNode *raiz = NULL;

void yyerror(const char *s);
int yylex(void);
%}

%union {
    int num;
    char* str;
    ASTNode* nodo;
}

%token <num> NUM
%token <str> ID
%token PRINTIWI INPUTUWU IFIWI ELSEWE WHILEWE RETURNUWU INTIWI FUNCIWI

%left '+' '-'
%left '*' '/'
%nonassoc UMINUS
%token '=' '(' ')' '{' '}' ';'

%type <nodo> programa instruccion expresion cuerpo declaracion_funcion lista_parametros llamado_funcion lista_argumentos

%%

programa
    : instruccion programa        { $$ = crearNodoPrograma($1, $2); }
    | declaracion_funcion programa { $$ = crearNodoPrograma($1, $2); }
    |                             { $$ = NULL; }
    ;

declaracion_funcion
    : ID '(' lista_parametros ')' cuerpo {
        $$ = crearNodoDeclaracionFuncion($1, $3, $5);
    }
    ;

lista_parametros
    : ID                          { $$ = crearNodoListaParametros($1, NULL); }
    | ID ',' lista_parametros     { $$ = crearNodoListaParametros($1, $3); }
    |                             { $$ = NULL; }
    ;

llamado_funcion
    : ID '(' lista_argumentos ')' {
        $$ = crearNodoLlamadoFuncion($1, $3);
    }
    ;

lista_argumentos
    : expresion                   { $$ = crearNodoListaArgumentos($1, NULL); }
    | expresion ',' lista_argumentos {
        $$ = crearNodoListaArgumentos($1, $3);
    }
    |                             { $$ = NULL; }
    ;

instruccion
    : PRINTIWI expresion ';'        { $$ = crearNodoPrint($2); }
    | ID '=' expresion ';'          { $$ = crearNodoAsignacion($1, $3); }
    | INPUTUWU ID ';'               { $$ = crearNodoInput($2); }
    | IFIWI '(' expresion ')' cuerpo ELSEWE cuerpo { $$ = crearNodoIfElse($3, $5, $7); }
    | WHILEWE '(' expresion ')' cuerpo            { $$ = crearNodoWhile($3, $5); }
    | RETURNUWU expresion ';'       { $$ = crearNodoReturn($2); }
    | FUNCIWI ID '(' ID ',' ID ')' cuerpo { 
        ASTNode *param1 = crearNodoIdentificador($4);
        ASTNode *param2 = crearNodoIdentificador($6);
        ASTNode *params = crearNodoPrograma(param1, crearNodoPrograma(param2, NULL));
        $$ = crearNodoFuncion($2, params, $8);
    }
    ;

cuerpo
    : '{' programa '}'     { $$ = $2; }
    ;

expresion
    : expresion '+' expresion      { $$ = crearNodoOperacion('+', $1, $3); }
    | expresion '-' expresion      { $$ = crearNodoOperacion('-', $1, $3); }
    | expresion '*' expresion      { $$ = crearNodoOperacion('*', $1, $3); }
    | expresion '/' expresion      { $$ = crearNodoOperacion('/', $1, $3); }
    | '-' expresion %prec UMINUS   { $$ = crearNodoOperacion('-', crearNodoNumero(0), $2); }
    | '(' expresion ')'            { $$ = $2; }
    | NUM                          { $$ = crearNodoNumero($1); }
    | ID                           { $$ = crearNodoIdentificador($1); }
    | llamado_funcion               { $$ = $1; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error de análisis: %s\n", s);
}