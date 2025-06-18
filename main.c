#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern ASTNode* listaFunciones;          // lista global de funciones (definida en ast.c)
ASTNode* buscarFuncion(const char* nombre); // prototipo de búsqueda (definida en ast.c)
int evaluar(ASTNode* nodo);                 // prototipo del evaluador

extern int yyparse(void);
extern FILE *yyin;
extern ASTNode *raiz;

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s archivo.cewe\n", argv[0]);
        return EXIT_FAILURE;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error al abrir el archivo");
        return EXIT_FAILURE;
    }

    printf("Iniciando análisis del archivo '%s'...\n", argv[1]);

    if (yyparse() == 0) {
        printf("Análisis sintáctico completado con éxito.\n");
        if (raiz) {
            printf("Resultado del AST:\n");
            imprimirAST(raiz, 0);

            // Ejecutar función mainiwi() como punto de entrada
            ASTNode* actual = listaFunciones;
            while (actual) {
                if (strcmp(actual->funcion_decl.nombre, "mainiwi") == 0) {
                    printf("Ejecutando mainiwi()...\n");
                    evaluar(actual->funcion_decl.cuerpo);
                    liberarAST(raiz);
                    fclose(yyin);
                    return EXIT_SUCCESS;
                }
                actual = actual->siguiente;
            }

            fprintf(stderr, "Error: función mainiwi() no definida.\n");
            liberarAST(raiz);
            fclose(yyin);
            return EXIT_FAILURE;
        } else {
            printf("No se generó AST.\n");
        }
    } else {
        fprintf(stderr, "Error durante el análisis sintáctico.\n");
    }

    fclose(yyin);
    return EXIT_SUCCESS;
}
