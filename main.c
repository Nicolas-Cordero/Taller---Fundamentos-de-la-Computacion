#include <stdio.h>
#include "ast.h"

extern int yyparse(void);
extern FILE* yyin;
extern ASTNode* root;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s archivo.cewe\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error al abrir el archivo");
        return 1;
    }

    if (yyparse() == 0) {
        printf("Parseo exitoso.\n");
        printf("Árbol de sintaxis generado:\n");
        print_ast(root, 0);
    } else {
        fprintf(stderr, "Error de sintaxis.\n");
    }

    fclose(yyin);
    return 0;
}