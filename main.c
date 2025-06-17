#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

// Prototipos externos
extern int yyparse(void);
extern FILE* yyin;
extern ASTNode* root;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s archivo.cewe\n", argv[0]);
        return EXIT_FAILURE;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error al abrir el archivo");
        return EXIT_FAILURE;
    }

    if (yyparse() == 0) {
        printf("Parseo exitoso.\n\n");
        print_ast(root, 0);
    } else {
        fprintf(stderr, "Error durante el análisis sintáctico.\n");
    }

    fclose(yyin);
    return EXIT_SUCCESS;
}