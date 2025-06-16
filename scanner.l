%{
#include "parser.tab.h"
#include <string.h>
%}

DIGIT     [0-9]
ID_START  [a-zA-Z_]
ID_CHAR   [a-zA-Z0-9_]

%%

/* Palabras clave Cewe (última vocal + w + vocal) */
"mainiwi"        { return MAIN; }
"intiwi"         { return TIPO_INT; }
"returnuwu"      { return RETURN; }
"inputuwu"       { return INPUT; }
"outputuwu"      { return OUTPUT; }
"ifiwi"          { return IF; }
"elsewe"         { return ELSE; }
"whiliwi"        { return WHILE; }
"funciwi"        { return FUNC; }

/* Operadores y separadores */
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

/* Ignorar comentarios de línea estilo C++ */
"//".*           { /* ignorar */ }

/* Ignorar espacios y saltos de línea */
[ \t\r\n]+       { /* ignorar */ }

/* Literales numéricas e identificadores */
{DIGIT}+         { yylval.ival = atoi(yytext); return NUMBER; }

{ID_START}{ID_CHAR}* {
    yylval.sval = strdup(yytext);
    return ID;
}

/* Cualquier otro carácter genera error léxico */
.                { printf("Carácter no reconocido: %s\\n", yytext); }

%%

int yywrap(void) { return 1; }