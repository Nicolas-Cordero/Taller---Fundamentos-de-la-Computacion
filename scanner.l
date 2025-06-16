%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
%}

%%

[ \t\r\n]+                ; // ignorar espacios y saltos
"//".*                      ; // ignorar comentarios

"mainiwi"                   { return MAINIWI; }
"int"                       { return INTIWI; }
"printiwi"                  { return PRINTIWI; }

[0-9]+                      { yylval.intval = atoi(yytext); return INT_LITERAL; }
[a-zA-Z_][a-zA-Z0-9_]*      { yylval.str = strdup(yytext); return IDENTIFIER; }

"="                         { return ASSIGN; }
";"                         { return SEMICOLON; }
"("                         { return LPAREN; }
")"                         { return RPAREN; }
"{"                         { return LBRACE; }
"}"                         { return RBRACE; }
"+"                         { return PLUS; }
"-"                         { return MINUS; }
"*"                         { return MUL; }
"/"                         { return DIV; }

.                           { return yytext[0]; }

%%

int yywrap(void) {
    return 1;
}
