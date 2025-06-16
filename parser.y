%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode* root;
void yyerror(const char* s);
int yylex(void);
%}

%union {
    int ival;
    char* sval;
    ASTNode* node;
    ASTNode** nodelist;
    char** strlist;
}

/* Tokens sincronizados con scanner.l */
%token MAIN TIPO_INT RETURN INPUT OUTPUT IF ELSE WHILE FUNC
%token <ival> NUMBER
%token <sval> ID
%token EQ NEQ

%type <node> program stmt expr block decl assign input output ret ifstmt whilestmt funcdef funccall args
%type <nodelist> stmts_list args_list
%type <strlist> params params_list

%start program

%%

program
    : MAIN '(' ')' block            { root = $4; }
    ;

stmt
    : decl                          { $$ = $1; }
    | assign                        { $$ = $1; }
    | input                         { $$ = $1; }
    | output                        { $$ = $1; }
    | ret                           { $$ = $1; }
    | ifstmt                        { $$ = $1; }
    | whilestmt                     { $$ = $1; }
    | funcdef                       { $$ = $1; }
    | funccall ';'                  { $$ = $1; }
    | block                         { $$ = $1; }
    ;

block
    : '{' stmts_list '}'            { 
        int count = 0;
        while ($2 && $2[count]) count++;
        $$ = n_block($2, count);
    }
    ;

stmts_list
    : stmts_list stmt               {
        int count = 0;
        while ($1 && $1[count]) count++;
        $1 = realloc($1, sizeof(ASTNode*) * (count + 2));
        $1[count] = $2;
        $1[count + 1] = NULL;
        $$ = $1;
    }
    | stmt                          {
        $$ = malloc(sizeof(ASTNode*) * 2);
        $$[0] = $1;
        $$[1] = NULL;
    }
    ;

decl
    : TIPO_INT ID ';'               { $$ = n_decl($2); }
    ;

assign
    : ID '=' expr ';'               { $$ = n_assign($1, $3); }
    ;

input
    : INPUT ID ';'                  { $$ = n_input($2); }
    ;

output
    : OUTPUT expr ';'               { $$ = n_output($2); }
    ;

ret
    : RETURN expr ';'               { $$ = n_return($2); }
    ;

ifstmt
    : IF '(' expr ')' stmt ELSE stmt { $$ = n_if($3, $5, $7); }
    | IF '(' expr ')' stmt           { $$ = n_if($3, $5, NULL); }
    ;

whilestmt
    : WHILE '(' expr ')' stmt       { $$ = n_while($3, $5); }
    ;

funcdef
    : FUNC ID '(' params ')' block  {
        int count = 0;
        while ($4 && $4[count]) count++;
        $$ = n_func_def($2, $4, count, $6);
    }
    ;

params
    : params_list                   { $$ = $1; }
    |                               { $$ = NULL; }
    ;

params_list
    : params_list ',' ID            {
        int count = 0;
        while ($1 && $1[count]) count++;
        $1 = realloc($1, sizeof(char*) * (count + 2));
        $1[count] = strdup($3);
        $1[count + 1] = NULL;
        $$ = $1;
    }
    | ID                            {
        $$ = malloc(sizeof(char*) * 2);
        $$[0] = strdup($1);
        $$[1] = NULL;
    }
    ;

funccall
    : ID '(' args ')' {
        int count = 0;
        while ($3 && $3[count]) count++;
        $$ = n_func_call($1, $3, count);
    }
    ;
args
    : args_list                     { $$ = $1; }
    |                               { $$ = NULL; }
    ;

args_list
    : args_list ',' expr            {
        int count = 0;
        while ($1 && $1[count]) count++;
        $1 = realloc($1, sizeof(ASTNode*) * (count + 2));
        $1[count] = $3;
        $1[count + 1] = NULL;
        $$ = $1;
    }
    | expr                          {
        $$ = malloc(sizeof(ASTNode*) * 2);
        $$[0] = $1;
        $$[1] = NULL;
    }
    ;

expr
    : expr '+' expr                 { $$ = n_bin(OP_ADD, $1, $3); }
    | expr '-' expr                 { $$ = n_bin(OP_SUB, $1, $3); }
    | expr '*' expr                 { $$ = n_bin(OP_MUL, $1, $3); }
    | expr '/' expr                 { $$ = n_bin(OP_DIV, $1, $3); }
    | expr EQ expr                  { $$ = n_bin(OP_EQ, $1, $3); }
    | expr NEQ expr                 { $$ = n_bin(OP_NEQ, $1, $3); }
    | '(' expr ')'                  { $$ = $2; }
    | NUMBER                        { $$ = n_int($1); }
    | ID                            { $$ = n_id($1); }
    | funccall                      { $$ = $1; }
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Error de sintaxis: %s\\n", s);
}