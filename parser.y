%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode* root;
ASTNode** stmt_buffer = NULL;
int stmt_count = 0;

void append_stmt(ASTNode* stmt) {
    stmt_buffer = realloc(stmt_buffer, sizeof(ASTNode*) * (stmt_count + 1));
    if (!stmt_buffer) {
        fprintf(stderr, "Error al asignar memoria para sentencias\n");
        exit(1);
    }
    stmt_buffer[stmt_count++] = stmt;
}

void yyerror(const char* s) {
    fprintf(stderr, "Error de sintaxis: %s\n", s);
}
%}

%union {
    int num;
    char* id;
    ASTNode* node;
}

%token <num> NUMBER
%token <id> ID
%token MAINIWI INPUTUWU OUTPUTUWU IFIWI ELSEIWI WHILIWI RETURNUWU INTIWI
%token EQ GT LT PLUS MINUS MUL DIV POW
%token ASSIGN SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE

%type <node> programa bloque stmt expr

%%

programa:
    MAINIWI bloque {
        root = n_stmt_list(stmt_buffer, stmt_count);
    }
;

bloque:
    LBRACE lista_stmts RBRACE
;

lista_stmts:
    /* vacío */ { /* no hacer nada */ }
    | lista_stmts stmt {
        append_stmt($2);
    }
;

stmt:
    INTIWI ID SEMICOLON {
        $$ = n_assign($2, n_int(0));
    }
    | ID ASSIGN expr SEMICOLON {
        $$ = n_assign($1, $3);
    }
    | INPUTUWU LPAREN expr RPAREN SEMICOLON {
        $$ = n_input($3);
    }
    | OUTPUTUWU LPAREN expr RPAREN SEMICOLON {
        $$ = n_output($3);
    }
    | RETURNUWU expr SEMICOLON {
        $$ = n_return($2);
    }
    | IFIWI LPAREN expr RPAREN bloque {
        $$ = n_if($3, n_stmt_list(stmt_buffer, stmt_count), NULL);
        stmt_count = 0; // limpiar lista
    }
    | IFIWI LPAREN expr RPAREN bloque ELSEIWI bloque {
        $$ = n_if($3,
                  n_stmt_list(stmt_buffer, stmt_count), // then
                  n_stmt_list(stmt_buffer, stmt_count)); // else (provisional)
        stmt_count = 0;
    }
    | WHILIWI LPAREN expr RPAREN bloque {
        $$ = n_while($3, n_stmt_list(stmt_buffer, stmt_count));
        stmt_count = 0;
    }
;

expr:
    NUMBER {
        $$ = n_int($1);
    }
    | ID {
        $$ = n_id($1);
    }
    | expr PLUS expr {
        $$ = n_binop(OP_ADD, $1, $3);
    }
    | expr MINUS expr {
        $$ = n_binop(OP_SUB, $1, $3);
    }
    | expr MUL expr {
        $$ = n_binop(OP_MUL, $1, $3);
    }
    | expr DIV expr {
        $$ = n_binop(OP_DIV, $1, $3);
    }
    | expr POW expr {
        $$ = n_binop(OP_POW, $1, $3);
    }
    | expr EQ expr {
        $$ = n_binop(OP_EQ, $1, $3);
    }
    | expr GT expr {
        $$ = n_binop(OP_GT, $1, $3);
    }
    | expr LT expr {
        $$ = n_binop(OP_LT, $1, $3);
    }
    | LPAREN expr RPAREN {
        $$ = $2;
    }
;

%%