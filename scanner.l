%{
#include "parser.tab.h"
#include <string.h>
%}

DIGIT      [0-9]
ID         [a-zA-Z_][a-zA-Z0-9_]*

%%

"mainiwi"      { return MAINIWI; }
"inputuwu"     { return INPUTUWU; }
"outputuwu"    { return OUTPUTUWU; }
"ifiwi"        { return IFIWI; }
"elseiwi"      { return ELSEIWI; }
"whiliwi"      { return WHILIWI; }
"returnuwu"    { return RETURNUWU; }
"intiwi"       { return INTIWI; }

"=="           { return EQ; }
">"            { return GT; }
"<"            { return LT; }
"+"            { return PLUS; }
"-"            { return MINUS; }
"*"            { return MUL; }
"/"            { return DIV; }
"^"            { return POW; }

"="            { return ASSIGN; }
";"            { return SEMICOLON; }
","            { return COMMA; }
"("            { return LPAREN; }
")"            { return RPAREN; }
"{"            { return LBRACE; }
"}"            { return RBRACE; }

{DIGIT}+       { yylval.num = atoi(yytext); return NUMBER; }

/* Palabras clave ya se validaron arriba. Esto captura los identificadores */
{ID}           { yylval.id = strdup(yytext); return ID; }

[ \t\n\r]+     { /* ignorar espacios */ }

.              { return yytext[0]; }

%%

int yywrap(void) {
    return 1;
}