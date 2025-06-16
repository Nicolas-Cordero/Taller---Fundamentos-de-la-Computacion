%{
#include "ast.h"
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>
%}

%%

"mainiwi"        { return MAIN; }
"intiwi"         { return TIPO_INT; }
"returnuwu"      { return RETURN; }
"inputuwu"       { return INPUT; }
"outputuwu"      { return OUTPUT; }
"ifiwi"          { return IF; }
"elsewe"         { return ELSE; }
"whiliwi"        { return WHILE; }
"funciwi"        { return FUNC; }

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

[0-9]+           { yylval.ival = atoi(yytext); return NUMBER; }

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.sval = strdup(yytext);
    return ID;
}

[ \t\r\n]+       { /* ignorar espacios */ }

"//".*           { /* ignorar comentarios */ }

.                { printf("Caracter no reconocido: %s\\n", yytext); }

%%

int yywrap() { return 1; }