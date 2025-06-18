%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include <string.h>

ASTNode *raiz = NULL;

VarType current_function_return_type;

void yyerror(const char *s);
int yylex(void);
%}

%union {
    int num;
    char* str;
    ASTNode* nodo;
}


%start archivo

%token <num> NUM INTIWI STRINGIWI
%token <str> ID STRING_TOKEN STR_LITERAL
%token PRINTIWI INPUTUWU IFIWI ELSEWE WHILEWE RETURNUWU  FUNCIWI RETURNIWI MAIN  
%token SUMA RESTA MULT DIV POT
%token LT GT LE GE EQ NE
%token IGUALIGUAL


%left '+' '-'
%left '*' '/'
%right POT
%left EQ NE IGUALIGUAL LT LE GT GE
%nonassoc UMINUS

%token '=' '(' ')' '{' '}' ';'

%type <nodo> archivo programa instruccion expresion cuerpo declaracion_funcion llamado_funcion lista_argumentos bloque
%type <nodo> lista_parametros
%type <num> tipo
%type <nodo> declaracion
%%

archivo
    : MAIN cuerpo { $$ = $2; raiz = $$; }
    ;

cuerpo
    : '{' programa '}'     { $$ = $2; }
    ;

tipo
    : INTIWI     { $$ = TYPE_INT; }
    | STRINGIWI  { $$ = TYPE_STRING; }
    ;

programa
    : declaracion_funcion programa { $$ = crearNodoPrograma($1, $2); raiz = $$; }
    | instruccion programa         { $$ = crearNodoPrograma($1, $2); raiz = $$; }
    |                              { $$ = NULL; raiz = $$; }
    ;

declaracion_funcion
    : tipo ID '(' lista_parametros ')' cuerpo {
    current_function_return_type = $1;
    $$ = crearNodoDeclaracionFuncion($2, $4, $6);  
    }
    ;

declaracion
    : INTIWI ID {
        if (!add_symbol($2, TYPE_INT)) {
            fprintf(stderr, "Error: variable '%s' ya declarada.\n", $2);
            exit(1);
        }
    }
    | STRINGIWI ID {
        if (!add_symbol($2, TYPE_STRING)) {
            fprintf(stderr, "Error: variable '%s' ya declarada.\n", $2);
            exit(1);
        }
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
    : expresion {
        if ($1->tipo == NODE_IDENTIFICADOR && !symbol_exists($1->nombre)) {
            fprintf(stderr, "Error: argumento '%s' no declarado.\n", $1->nombre);
            exit(1);
        }
        $$ = crearNodoListaArgumentos($1, NULL);
    }
    | expresion ',' lista_argumentos {
        if ($1->tipo == NODE_IDENTIFICADOR && !symbol_exists($1->nombre)) {
            fprintf(stderr, "Error: argumento '%s' no declarado.\n", $1->nombre);
            exit(1);
        }
        $$ = crearNodoListaArgumentos($1, $3);
    }
    |                             { $$ = NULL; }
    ;

instruccion
    : PRINTIWI expresion ';'        { $$ = crearNodoPrint($2); }
    | ID '=' expresion ';'          { $$ = crearNodoAsignacion($1, $3); }
    | INPUTUWU tipo ID ';' {
        $$ = crearNodo(NODE_INPUT);
        $$->input.identificador = strdup($3);
        $$->input.tipo = $2;
        add_symbol($3, $2);
    }
    | IFIWI '(' expresion ')' bloque ELSEWE bloque { $$ = crearNodoIfElse($3, $5, $7); }
    | WHILEWE '(' expresion ')' bloque            { $$ = crearNodoWhile($3, $5); }
    | RETURNUWU expresion ';' {$$ = crearNodoReturn($2, current_function_return_type);}
    | FUNCIWI ID '(' INTIWI ID ',' INTIWI ID ')' cuerpo {
        ASTNode *param1 = crearNodoIdentificador($5);
        ASTNode *param2 = crearNodoIdentificador($8);
        ASTNode *params = crearNodoPrograma(param1, crearNodoPrograma(param2, NULL));
        $$ = crearNodoFuncion($2, params, $10); 
    }
    | declaracion
    ;


bloque
    : '{' programa '}'     { $$ = $2; }
    ;

expresion
    : expresion '+' expresion      { $$ = crearNodoOperacion('+', $1, $3); }
    | expresion '-' expresion      { $$ = crearNodoOperacion('-', $1, $3); }
    | expresion '*' expresion      { $$ = crearNodoOperacion('*', $1, $3); }
    | expresion '/' expresion      { $$ = crearNodoOperacion('/', $1, $3); }
    | expresion EQ expresion      { $$ = crearNodoOperacionRel('=', $1, $3); }
    | expresion NE expresion      { $$ = crearNodoOperacionRel('!', $1, $3); }
    | expresion IGUALIGUAL expresion { $$ = crearNodoOperacionRel('=', $1, $3); }
    | expresion LT expresion      { $$ = crearNodoOperacionRel('<', $1, $3); }
    | expresion LE expresion      { $$ = crearNodoOperacionRel('l', $1, $3); }
    | expresion GT expresion      { $$ = crearNodoOperacionRel('>', $1, $3); }
    | expresion GE expresion      { $$ = crearNodoOperacionRel('g', $1, $3); }
    | expresion POT expresion      { $$ = crearNodoOperacion('^', $1, $3); }
    | '-' expresion %prec UMINUS   { $$ = crearNodoOperacion('-', crearNodoNumero(0), $2); }
    | '(' expresion ')'            { $$ = $2; }
    | NUM                          { $$ = crearNodoNumero($1); }
    | ID {
        if (!symbol_exists($1)) {
            fprintf(stderr, "Error: variable '%s' no declarada.\n", $1);
            exit(1);
        }
        $$ = crearNodoVariable($1);
    }
    | STR_LITERAL               { $$ = crearNodoString($1); }
    | CADENA                    { $$ = crearNodoString($1); }
    | llamado_funcion               { $$ = $1; }
    | INPUTUWU '(' tipo ')'  { $$ = crearNodoInput(NULL, $3); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error de análisis: %s\n", s);
}