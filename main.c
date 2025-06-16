#include <stdio.h>
#include "ast.h"

extern int yyparse();
extern ASTNode* root; // Definido en parser.y

int main() {
    printf("Parsing...\n");
    if (yyparse() == 0) {
        printf("AST generado exitosamente:\n");
        print_ast(root, 0);
    } else {
        printf("Error durante el parseo.\n");
    }
    return 0;
}
