// Autor: Nicolás Cordero

%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex(void);
extern int yyerror(const char *s);
Node *raiz = NULL;
%}

%union {
    int num;
    char* str;
    Node* nodo;
}

%token <num> NUM
%token <str> ID
%token PRINTIWI INPUTUWU IFIWI ELSEWE WHILEWE RETURNUWU INTIWI

%type <nodo> programa instruccion expresion bloque

%%

programa
    : instruccion programa   { raiz = crearNodoPrograma($1, $2); }
    |                       { raiz = NULL; }
    ;

instruccion
    : PRINTIWI expresion ';'        { $$ = crearNodoPrint($2); }
    | ID '=' expresion ';'          { $$ = crearNodoAsignacion($1, $3); }
    | INPUTUWU ID ';'               { $$ = crearNodoInput($2); }
    | IFIWI '(' expresion ')' bloque ELSEUWU bloque { $$ = crearNodoIfElse($3, $5, $7); }
    | WHILEWE '(' expresion ')' bloque            { $$ = crearNodoWhile($3, $5); }
    | RETURNUWU expresion ';'       { $$ = crearNodoReturn($2); }
    ;

bloque
    : '{' programa '}'     { $$ = $2; }
    ;

expresion
    : expresion '+' expresion      { $$ = crearNodoOperacion('+', $1, $3); }
    | expresion '-' expresion      { $$ = crearNodoOperacion('-', $1, $3); }
    | expresion '*' expresion      { $$ = crearNodoOperacion('*', $1, $3); }
    | expresion '/' expresion      { $$ = crearNodoOperacion('/', $1, $3); }
    | '(' expresion ')'            { $$ = $2; }
    | NUM                          { $$ = crearNodoNumero($1); }
    | ID                           { $$ = crearNodoIdentificador($1); }
    ;

%%

int yyerror(const char *s) {
    fprintf(stderr, "Error de análisis: %s\n", s);
    return 0;
}