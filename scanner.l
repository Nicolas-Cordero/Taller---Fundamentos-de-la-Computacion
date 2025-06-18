%{
#include "ast.h"
#include "parser.tab.h"
#include <string.h>
%}

%%

[ \t\r]+               ;   // Ignorar espacios y tabulaciones
\n                    { yylineno++; }

"printiwi"            { return PRINTIWI; }
"inputuwu"            { return INPUTUWU; }
"ifiwi"               { return IFIWI; }
"elsewe"              { return ELSEWE; }
"whilewe"             { return WHILEWE; }
"returnuwu"           { return RETURNUWU; }
"intiwi"              { return INTIWI; }
"funciwi"             { return FUNCIWI; }
"stringiwi"           { return STRINGIWI; }




\"([^\\"]|\\.)*\" {
    char* raw = strdup(yytext);
    raw[strlen(raw) - 1] = '\0';     // quitar comilla final
    yylval.str = strdup(raw + 1);    // copiar sin comilla inicial
    free(raw);
    return STR_LITERAL;
}

[0-9]+                { yylval.num = atoi(yytext); return NUM; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return ID; }

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
","                   { return ','; }


.                     { printf("Caracter no reconocido: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}