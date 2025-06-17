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
%nterm <nodo> parametros argumentos

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
    : ID ',' lista_parametros {
        $$ = crearNodoListaParametros($1, $3);
    }
    | ID {
        $$ = crearNodoListaParametros($1, NULL);
    }
    | /* vacío */ {
        $$ = NULL;
    }
    ;

llamado_funcion
    : ID '(' lista_argumentos ')' {
        $$ = crearNodoLlamadoFuncion($1, $3);
    }
    ;

lista_argumentos
    : expresion ',' lista_argumentos {
        $$ = crearNodoListaArgumentos($1, $3);
    }
    | expresion {
        $$ = crearNodoListaArgumentos($1, NULL);
    }
    | /* vacío */ {
        $$ = NULL;
    }
    ;


instruccion
    : FUNCIWI ID '(' lista_parametros ')' '{' programa '}' {
        $$ = crearNodoFuncion($2, $4, $7);
    }
    | PRINTIWI '(' expresion ')' {
        $$ = crearNodoPrint($3);
    }
    | RETURNUWU expresion {
        $$ = crearNodoReturn($2);
    }
    | ID '=' expresion {
        $$ = crearNodoAsignacion($1, $3);
    }
    | WHILEWE '(' expresion ')' '{' programa '}' {
        $$ = crearNodoWhile($3, $6);
    }
    | expresion {
        $$ = $1; // Permite funciones solas como instrucción
    }
    ;
cuerpo
    : '{' programa '}'     { $$ = $2; }
    ;


expresion
    : expresion '+' expresion {
        $$ = crearNodoOperacion('+', $1, $3);
    }
    | expresion '-' expresion {
        $$ = crearNodoOperacion('-', $1, $3);
    }
    | expresion '*' expresion {
        $$ = crearNodoOperacion('*', $1, $3);
    }
    | expresion '/' expresion {
        $$ = crearNodoOperacion('/', $1, $3);
    }
    | ID {
        $$ = crearNodoIdentificador($1);
    }
    | NUM {
        $$ = crearNodoNumero($1);
    }
    | ID '(' lista_argumentos ')' {
        $$ = crearNodoLlamadoFuncion($1, $3);
    }
    | '(' expresion ')' {
        $$ = $2;
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error de análisis: %s\n", s);
}