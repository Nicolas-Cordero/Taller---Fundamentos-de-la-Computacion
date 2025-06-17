%{
#include "parser.tab.h"
#include <string.h>

int yylineno = 1;
%}

%%

[ \t\r]+               ;   // Ignorar espacios y tabulaciones
\n                    { yylineno++; }

"printiwi"            { return PRINTIWI; }
"inputuwu"            { return INPUTUWU; }
"ifiwi"               { return IFIWI; }
"elsewe"             { return ELSEWE; }
"whilewe"            { return WHILEWE; }
"returnuwu"           { return RETURNUWU; }
"intiwi"              { return INTIWI; }

[0-9]+                { yylval.num = atoi(yytext); return NUM; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return ID; }

"=="                  { return '='; }  // Comparador opcional, puedes ajustarlo
"="                   { return '='; }
"("                   { return '('; }
")"                   { return ')'; }
"{"                   { return '{'; }
"}"                   { return '}'; }
";"                   { return ';'; }
"+"                   { return '+'; }
"-"                   { return '-'; }
"*"                   { return '*'; }
"/"                   { return '/'; }

.                     { printf("Caracter no reconocido: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}