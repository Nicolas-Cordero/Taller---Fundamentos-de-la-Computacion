%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

void yyerror(const char *s);
int yylex();
ASTNode* root;
%}

%union {
    int intval;
    float floatval;
    char* str;
    ASTNode* node;
    struct {
        ASTNode** list;
        int count;
    } stmt_list;
}

%token <str> IDENTIFIER STRING_LITERAL
%token <intval> INT_LITERAL
%token INTIWI MAINIWI PRINTIWI
%token ASSIGN SEMICOLON LPAREN RPAREN LBRACE RBRACE
%token PLUS MINUS MUL DIV

%type <node> program statement expression
%type <stmt_list> statement_list

%%

program:
    MAINIWI LBRACE statement_list RBRACE {
        root = make_block($3.list, $3.count);
    }
    ;

statement_list:
    /* vacío */ {
        $$.list = NULL;
        $$.count = 0;
    }
    | statement_list statement {
        $$.count = $1.count + 1;
        $$.list = realloc($1.list, $$.count * sizeof(ASTNode*));
        $$.list[$1.count] = $2;
    }
    ;

statement:
    INTIWI IDENTIFIER SEMICOLON {
        $$ = make_declaration("int", $2);
    }
    | IDENTIFIER ASSIGN expression SEMICOLON {
        $$ = make_assignment($1, $3);
    }
    | PRINTIWI LPAREN expression RPAREN SEMICOLON {
        $$ = make_output($3);
    }
    ;

expression:
    INT_LITERAL {
        $$ = make_int_literal($1);
    }
    | IDENTIFIER {
        $$ = make_identifier($1);
    }
    | expression PLUS expression {
        $$ = make_binary_op(OP_PLUS, $1, $3);
    }
    | expression MINUS expression {
        $$ = make_binary_op(OP_MINUS, $1, $3);
    }
    | expression MUL expression {
        $$ = make_binary_op(OP_MUL, $1, $3);
    }
    | expression DIV expression {
        $$ = make_binary_op(OP_DIV, $1, $3);
    }
    | LPAREN expression RPAREN {
        $$ = $2;
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
