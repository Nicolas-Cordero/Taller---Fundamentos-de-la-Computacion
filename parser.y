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
%token <str> STRING
%token PRINTIWI INPUTUWU IFIWI ELSEWE WHILEWE RETURNUWU INTIWI FUNCIWI
%token LT GT LE GE EQ NE

%left '+' '-'
%left '*' '/'
%nonassoc UMINUS
%token '=' '(' ')' '{' '}' ';'

%type <nodo> programa instruccion expresion cuerpo declaracion_funcion lista_parametros_typed llamado_funcion lista_argumentos bloque

%%

programa
    : instruccion programa        { $$ = crearNodoPrograma($1, $2); raiz = $$; }
    | declaracion_funcion programa { $$ = crearNodoPrograma($1, $2); raiz = $$; }
    |                             { $$ = NULL; raiz = $$; }
    ;

/* Lista de parámetros con tipo explícito (por ahora sólo INTIWI) */
lista_parametros_typed
    : INTIWI ID                              { $$ = crearNodoListaParametros($2, NULL); }
    | INTIWI ID ',' lista_parametros_typed   { $$ = crearNodoListaParametros($2, $4); }
    |                                         { $$ = NULL; }
    ;

declaracion_funcion
    : FUNCIWI INTIWI ID '(' lista_parametros_typed ')' cuerpo
        { $$ = crearNodoDeclaracionFuncion($3, $5, $7); }
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
    | IFIWI '(' expresion ')' bloque ELSEWE bloque { $$ = crearNodoIfElse($3, $5, $7); }
    | WHILEWE '(' expresion ')' bloque            { $$ = crearNodoWhile($3, $5); }
    | RETURNUWU expresion ';'       { $$ = crearNodoReturn($2); }
    | FUNCIWI INTIWI ID '(' INTIWI ID ',' INTIWI ID ')' cuerpo {
        ASTNode *param1 = crearNodoIdentificador($6); /* primer identificador */
        ASTNode *param2 = crearNodoIdentificador($9); /* segundo identificador */
        ASTNode *params = crearNodoPrograma(param1, crearNodoPrograma(param2, NULL));
        $$ = crearNodoFuncion($3, params, $11);
    }
    | INTIWI ID '=' expresion ';' { $$ = crearNodoAsignacion($2, $4); }
    ;

cuerpo
    : '{' programa '}'     { $$ = $2; }
    ;

bloque
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
    | STRING { $$ = crearNodoString($1); }
    | expresion LE expresion { $$ = crearNodoOperacion(OP_LE, $1, $3); }
    | expresion GE expresion { $$ = crearNodoOperacion(OP_GE, $1, $3); }
    | expresion EQ expresion { $$ = crearNodoOperacion(OP_EQ, $1, $3); }
    | expresion NE expresion { $$ = crearNodoOperacion(OP_NE, $1, $3); }
    | expresion LT expresion { $$ = crearNodoOperacion(OP_LT, $1, $3); }
    | expresion GT expresion { $$ = crearNodoOperacion(OP_GT, $1, $3); }
    ;

%%

void yyerror(const char *s) {
    extern int yylineno;
    fprintf(stderr, "Error de análisis en línea %d: %s\n", yylineno, s);
}