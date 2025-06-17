%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

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

%token <num> NUM INTIWI STRINGIWI
%token <str> ID STRING_TOKEN
%token PRINTIWI INPUTUWU IFIWI ELSEWE WHILEWE RETURNUWU  FUNCIWI RETURNIWI MAIN
%token SUMA RESTA MULT DIV POT
%token LT GT LE GE EQ NE


%left '+' '-'
%left '*' '/'
%nonassoc UMINUS
%token '=' '(' ')' '{' '}' ';'

%type <nodo> programa instruccion expresion cuerpo declaracion_funcion llamado_funcion lista_argumentos bloque
%type <nodo> lista_parametros
%type <num> tipo
%%

cuerpo
    : '{' programa '}'     { $$ = $2; }
    ;

tipo
    : INTIWI     { $$ = TYPE_INT; }
    | STRINGIWI  { $$ = TYPE_STRING; }
    ;

programa
    : instruccion programa        { $$ = crearNodoPrograma($1, $2); raiz = $$; }
    | declaracion_funcion programa { $$ = crearNodoPrograma($1, $2); raiz = $$; }
    |                             { $$ = NULL; raiz = $$; }
    ;

declaracion_funcion
    : tipo ID '(' lista_parametros ')' cuerpo {
    current_function_return_type = $1;
    $$ = crearNodoDeclaracionFuncion($2, $4, $6);  
    }
    ;

declaracion: INTIWI ID {
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
};

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
        if ($1->tipo == NODE_IDENTIFICADOR) {
            VarType t = get_symbol_type($1->nombre);
            if (t != TYPE_INT) {
                fprintf(stderr, "Error: argumento '%s' no es de tipo entero.\n", $1->nombre);
                exit(1);
            }
        }
        $$ = crearNodoListaArgumentos($1, NULL);
    }
    | expresion ',' lista_argumentos {
        if ($1->tipo == NODE_IDENTIFICADOR) {
            VarType t = get_symbol_type($1->nombre);
            if (t != TYPE_INT) {
                fprintf(stderr, "Error: argumento '%s' no es de tipo entero.\n", $1->nombre);
                exit(1);
            }
        }
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
    | RETURNUWU expresion ';' {$$ = crearNodoReturn($2, current_function_return_type);}
    | FUNCIWI ID '(' INTIWI ID ',' INTIWI ID ')' cuerpo {
        ASTNode *param1 = crearNodoIdentificador($5);
        ASTNode *param2 = crearNodoIdentificador($8);
        ASTNode *params = crearNodoPrograma(param1, crearNodoPrograma(param2, NULL));
        $$ = crearNodoFuncion($2, params, $10); 
    }
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
    | ID {
        if (!symbol_exists($1)) {
            fprintf(stderr, "Error: variable '%s' no declarada.\n", $1);
            exit(1);
        }
        $$ = crearNodoVariable($1);
    }
    | llamado_funcion               { $$ = $1; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error de análisis: %s\n", s);
}