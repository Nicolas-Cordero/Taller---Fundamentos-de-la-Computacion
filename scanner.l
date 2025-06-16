%{
#include "parser.tab.h"
#include <string.h>

void yyerror(const char* s);
%}

DIGIT     [0-9]
ID_START  [a-zA-Z_]
ID_CHAR   [a-zA-Z0-9_]

%%

/* Palabras clave Cewe */
"mainiwi"        { return MAIN; }
"intiwi"         { return TIPO_INT; }
"returnuwu"      { return RETURN; }
"inputuwu"       { return INPUT; }
"outputuwu"      { return OUTPUT; }
"ifiwi"          { return IF; }
"elsewe"         { return ELSE; }
"whiliwi"        { return WHILE; }
"funciwi"        { return FUNC; }

/* Operadores y signos */
"=="             { return EQ; }
"!="             { return NEQ; }
"+"              { return '+'; }
"-"              { return '-'; }
"*"              { return '*'; }
"/"              { return '/'; }
"="              { return '='; }
"("              { return '('; }
")"              { return ')'; }
"{"              { return '{'; }
"}"              { return '}'; }
";"              { return ';'; }
","              { return ','; }

/* Literales e identificadores */
{DIGIT}+         { yylval.ival = atoi(yytext); return NUMBER; }

{ID_START}{ID_CHAR}* {
    yylval.sval = strdup(yytext);
    return ID;
}

/* Ignorar espacios */
[ \t\r\n]+       ;

/* Error de carácter no reconocido */
.                { printf("Caracter no reconocido: %s\n", yytext); }

%%

int yywrap() { return 1; }