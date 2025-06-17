%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
void yyerror(const char *s);
ASTNode* raiz;
%}

%union {
    int valor_numero;
    char* cadena;
    ASTNode* nodo;
}

%token <valor_numero> NUMERO
%token <cadena> IDENTIFICADOR
%token FUNC MAIN IF ELSE WHILE RETURN PRINT INPUT
%token SUMA RESTA MULT DIV ASIG IGUAL DIFERENTE MENOR MAYOR MENOR_IGUAL MAYOR_IGUAL
%token PARIZQ PARDER LLAVEIZQ LLAVEDER COMA PUNTOYCOMA CADENA

%type <nodo> programa lista_funciones funcion instrucciones instruccion expresion lista_parametros lista_argumentos

%%

programa:
    lista_funciones { raiz = $1; }
;

lista_funciones:
    funcion { $$ = $1; }
    | lista_funciones funcion { $$ = agregarHermano($1, $2); }
;

funcion:
    FUNC MAIN PARIZQ PARDER LLAVEIZQ instrucciones LLAVEDER { $$ = crearNodoDeclaracionFuncion("main", NULL, $6); }
;

instrucciones:
    /* reglas para instrucciones */
    /* por simplicidad, aquí solo un ejemplo */
    instruccion { $$ = $1; }
    | instrucciones instruccion { /* combinar nodos de instrucciones */ }
;

instruccion:
    /* reglas para instruccion */
;

expresion:
    /* reglas para expresion */
;

lista_parametros:
    /* reglas para lista_parametros */
;

lista_argumentos:
    /* reglas para lista_argumentos */
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    yyparse();
    // Aquí podrías procesar el AST en 'raiz'
    return 0;
}