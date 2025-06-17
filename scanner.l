%{
#include "ast.h"
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
"elsewe"              { return ELSEWE; }
"whilewe"             { return WHILEWE; }
"returnuwu"           { return RETURNUWU; }
"intiwi"              { return INTIWI; }
"funciwi"             { return FUNCIWI; }

\"[^\"]*\" {

    char *text = strdup(yytext);
    text[strlen(text) - 1] = '\0';  // quitar comilla final
    yylval.str = strdup(text + 1);  // salta comilla inicial
    free(text);
    return STRING;
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


.                     { printf("Token desconocido: '%s'\n", yytext); return yytext[0]; }

%%

int yywrap() {
    return 1;
}