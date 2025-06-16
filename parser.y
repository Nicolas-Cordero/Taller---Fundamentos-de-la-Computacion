%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

void yyerror(const char *s);
int  yylex(void);
%}

/* -------------------------  VALORES DE YYSTYPE  ----------------------- */
%union {
    int        ival;       /* literales numéricas */
    char      *sval;       /* identificadores / strings */
    ASTNode   *node;       /* un nodo cualquiera del AST */
    ASTNode  **nodelist;   /* vector (lista) de nodos AST  */
}

/* -------------------------  TOKENS TERMINALES  ------------------------ */
%token <ival> NUMBER
%token <sval> ID

/*  Palabras-clave Cewe  */
%token MAINIWI   FUNCIWI  INTIWI
%token RETURNUWU INPUTUWU OUTPUTUWU
%token IFIWI ELSEWE WHILIWI

/*  Operadores simples  */
%token EQ NEQ LT LE GT GE
%token ASSIGN           /* =  */
%token PLUS  MINUS      /* +  - */
%token TIMES DIVIDE     /* *  / */
%token POWER            /* ^  */

/*  Separadores  */
%token LPAREN  RPAREN   /* ( ) */
%token LBRACE  RBRACE   /* { } */
%token COMMA   SEMICOLON

/* ---------------------  PRECEDENCIA DE OPERADORES  -------------------- */
%left PLUS  MINUS
%left TIMES DIVIDE
%right POWER

/* -------------------------  TIPOS DE NO TERMINAL  --------------------- */
%type <node>      program main_func func_decl block stmt stmt_list
%type <node>      decl_stmt assign_stmt input_stmt output_stmt
%type <node>      return_stmt if_stmt while_stmt func_call expr term factor
%type <nodelist>  args args_list

/* -------------------------  SÍMBOLO INICIAL  -------------------------- */
%start program

%%  /* =====================  REGLAS GRAMATICALES  ===================== */

/* Programa completo = función main + cero o más funciones auxiliares */
program
    : main_func func_list      { $$ = n_program($1, $2); }
    ;

/* Lista (posiblemente vacía) de funciones  */
func_list
    : func_list func_decl      { $$ = n_func_list($1, $2); }
    |                          { $$ = NULL; }
    ;

/* Función mainiwi sin argumentos */
main_func
    : MAINIWI LPAREN RPAREN block
                              { $$ = n_func_def("main", NULL, 0, $4); }
    ;

/* Declaración de función con parámetros */
func_decl
    : FUNCIWI ID LPAREN args RPAREN block
                              {
                                  int n=0; while($4&&$4[n]) n++;
                                  $$ = n_func_def($2, $4, n, $6);
                              }
    ;

/* --------------------  LISTAS DE PARÁMETROS (DECL)  ------------------- */
args
    : args_list               { $$ = $1; }
    |                         { $$ = NULL; }
    ;

args_list
    : args_list COMMA INTIWI ID   { $$ = n_arg_list($1, $4); }
    | INTIWI ID                   { $$ = n_arg_list(NULL, $2); }
    ;

/* --------------------------  BLOQUE DE CÓDIGO  ------------------------ */
block
    : LBRACE stmt_list RBRACE
                              { $$ = n_block($2); }
    ;

stmt_list
    : stmt_list stmt          { $$ = n_stmt_list($1, $2); }
    |                         { $$ = NULL; }
    ;

/* ---------------------------  SENTENCIAS  ----------------------------- */
stmt
    : decl_stmt
    | assign_stmt
    | input_stmt
    | output_stmt
    | return_stmt
    | if_stmt
    | while_stmt
    | func_call SEMICOLON     { $$ = $1; }
    ;

/* --- Declarar variable --- */
decl_stmt
    : INTIWI ID SEMICOLON     { $$ = n_decl($2); }
    ;

/* --- Asignación --- */
assign_stmt
    : ID ASSIGN expr SEMICOLON{ $$ = n_assign($1, $3); }
    ;

/* --- Entrada --- */
input_stmt
    : INPUTUWU ID SEMICOLON   { $$ = n_input($2); }
    ;

/* --- Salida --- */
output_stmt
    : OUTPUTUWU expr SEMICOLON{ $$ = n_output($2); }
    ;

/* --- Return --- */
return_stmt
    : RETURNUWU expr SEMICOLON{ $$ = n_return($2); }
    ;

/* --- If / Else --- */
if_stmt
    : IFIWI LPAREN expr RPAREN block ELSEWE block
                              { $$ = n_if($3, $5, $7); }
    | IFIWI LPAREN expr RPAREN block
                              { $$ = n_if($3, $5, NULL); }
    ;

/* --- While --- */
while_stmt
    : WHILIWI LPAREN expr RPAREN block
                              { $$ = n_while($3, $5); }
    ;

/* -----------------  LLAMADA A FUNCIÓN CON ARGUMENTOS  ----------------- */
func_call
    : ID LPAREN args_values RPAREN
                              {
                                  int n=0; while($3&&$3[n]) n++;
                                  $$ = n_func_call($1, $3, n);
                              }
    ;

args_values
    : args_values COMMA expr  { $$ = n_arg_expr_list($1, $3); }
    | expr                    { $$ = n_arg_expr_list(NULL, $1); }
    |                         { $$ = NULL; }
    ;

/* --------------------------  EXPRESIONES  ----------------------------- */
expr
    : expr PLUS  term         { $$ = n_bin(OP_ADD, $1, $3); }
    | expr MINUS term         { $$ = n_bin(OP_SUB, $1, $3); }
    | term                    { $$ = $1; }
    ;

term
    : term TIMES factor       { $$ = n_bin(OP_MUL, $1, $3); }
    | term DIVIDE factor      { $$ = n_bin(OP_DIV, $1, $3); }
    | factor                  { $$ = $1; }
    ;

factor
    : NUMBER                  { $$ = n_int($1); }
    | ID                      { $$ = n_id($1); }
    | func_call               { $$ = $1; }
    | LPAREN expr RPAREN      { $$ = $2; }
    ;

%%  /* =================  CÓDIGO C ADICIONAL  ========================== */

void yyerror(const char *s)
{
    fprintf(stderr, "Error sintáctico: %s\n", s);
}