#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode *crearNodo(ASTNodeType tipo) {
    ASTNode *nuevo = (ASTNode *)malloc(sizeof(ASTNode));
    if (!nuevo) {
        fprintf(stderr, "Error de memoria al crear nodo\n");
        exit(EXIT_FAILURE);
    }
    memset(nuevo, 0, sizeof(ASTNode));
    nuevo->tipo = tipo;
    return nuevo;
}

ASTNode *crearNodoPrograma(ASTNode *instruccion, ASTNode *programa) {
    ASTNode *nodo = crearNodo(PROGRAMA);
    nodo->programa.instruccion = instruccion;
    nodo->programa.programa = programa;
    return nodo;
}

ASTNode *crearNodoPrint(ASTNode *expresion) {
    ASTNode *nodo = crearNodo(PRINT);
    nodo->print.expresion = expresion;
    return nodo;
}

ASTNode *crearNodoAsignacion(char *id, ASTNode *expr) {
    ASTNode *nodo = crearNodo(ASIGNACION);
    nodo->assign.identificador = strdup(id);
    nodo->assign.expr = expr;
    return nodo;
}

ASTNode *crearNodoInput(char *id) {
    ASTNode *nodo = crearNodo(INPUT);
    nodo->input.identificador = strdup(id);
    return nodo;
}

ASTNode *crearNodoIfElse(ASTNode *cond, ASTNode *bloqueIf, ASTNode *bloqueElse) {
    ASTNode *nodo = crearNodo(IF_ELSE);
    nodo->ifelse.condicion = cond;
    nodo->ifelse.bloqueIf = bloqueIf;
    nodo->ifelse.bloqueElse = bloqueElse;
    return nodo;
}

ASTNode *crearNodoWhile(ASTNode *cond, ASTNode *bloque) {
    ASTNode *nodo = crearNodo(WHILE);
    nodo->whili.condicion = cond;
    nodo->whili.bloque = bloque;
    return nodo;
}

ASTNode *crearNodoReturn(ASTNode *expr) {
    ASTNode *nodo = crearNodo(RETURN);
    nodo->retorno.expresion = expr;
    return nodo;
}

ASTNode *crearNodoOperacion(char op, ASTNode *izq, ASTNode *der) {
    ASTNode *nodo = crearNodo(OPERACION);
    nodo->operacion.operador = op;
    nodo->operacion.izq = izq;
    nodo->operacion.der = der;
    return nodo;
}

ASTNode *crearNodoNumero(int valor) {
    ASTNode *nodo = crearNodo(NUMERO);
    nodo->numero.valor = valor;
    return nodo;
}

ASTNode *crearNodoIdentificador(char *id) {
    ASTNode *nodo = crearNodo(IDENTIFICADOR);
    nodo->identificador.nombre = strdup(id);
    return nodo;
}

// Evaluación para pruebas simples
int evaluar(ASTNode *nodo) {
    if (!nodo) return 0;

    switch (nodo->tipo) {
        case NUMERO:
            return nodo->numero.valor;
        case OPERACION: {
            int izq = evaluar(nodo->operacion.izq);
            int der = evaluar(nodo->operacion.der);
            switch (nodo->operacion.operador) {
                case '+': return izq + der;
                case '-': return izq - der;
                case '*': return izq * der;
                case '/': return der != 0 ? izq / der : 0;
                default: return 0;
            }
        }
        default:
            return 0;
    }
}

void imprimirAST(ASTNode *nodo, int nivel) {
    if (!nodo) return;

    for (int i = 0; i < nivel; i++) printf("  ");

    switch (nodo->tipo) {
        case PROGRAMA:
            printf("PROGRAMA\n");
            imprimirAST(nodo->programa.instruccion, nivel + 1);
            imprimirAST(nodo->programa.programa, nivel + 1);
            break;
        case PRINT:
            printf("PRINT\n");
            imprimirAST(nodo->print.expresion, nivel + 1);
            break;
        case ASIGNACION:
            printf("ASIGNACION %s\n", nodo->assign.identificador);
            imprimirAST(nodo->assign.expr, nivel + 1);
            break;
        case INPUT:
            printf("INPUT %s\n", nodo->input.identificador);
            break;
        case IF_ELSE:
            printf("IF_ELSE\n");
            imprimirAST(nodo->ifelse.condicion, nivel + 1);
            imprimirAST(nodo->ifelse.bloqueIf, nivel + 1);
            imprimirAST(nodo->ifelse.bloqueElse, nivel + 1);
            break;
        case WHILE:
            printf("WHILE\n");
            imprimirAST(nodo->whili.condicion, nivel + 1);
            imprimirAST(nodo->whili.bloque, nivel + 1);
            break;
        case RETURN:
            printf("RETURN\n");
            imprimirAST(nodo->retorno.expresion, nivel + 1);
            break;
        case OPERACION:
            printf("OPERACION %c\n", nodo->operacion.operador);
            imprimirAST(nodo->operacion.izq, nivel + 1);
            imprimirAST(nodo->operacion.der, nivel + 1);
            break;
        case NUMERO:
            printf("NUMERO %d\n", nodo->numero.valor);
            break;
        case IDENTIFICADOR:
            printf("IDENTIFICADOR %s\n", nodo->identificador.nombre);
            break;
        default:
            printf("Nodo desconocido\n");
    }
}

void liberarAST(ASTNode *nodo) {
    if (!nodo) return;

    switch (nodo->tipo) {
        case PROGRAMA:
            liberarAST(nodo->programa.instruccion);
            liberarAST(nodo->programa.programa);
            break;
        case PRINT:
            liberarAST(nodo->print.expresion);
            break;
        case ASIGNACION:
            free(nodo->assign.identificador);
            liberarAST(nodo->assign.expr);
            break;
        case INPUT:
            free(nodo->input.identificador);
            break;
        case IF_ELSE:
            liberarAST(nodo->ifelse.condicion);
            liberarAST(nodo->ifelse.bloqueIf);
            liberarAST(nodo->ifelse.bloqueElse);
            break;
        case WHILE:
            liberarAST(nodo->whili.condicion);
            liberarAST(nodo->whili.bloque);
            break;
        case RETURN:
            liberarAST(nodo->retorno.expresion);
            break;
        case OPERACION:
            liberarAST(nodo->operacion.izq);
            liberarAST(nodo->operacion.der);
            break;
        case IDENTIFICADOR:
            free(nodo->identificador.nombre);
            break;
        default: break;
    }

    free(nodo);
}