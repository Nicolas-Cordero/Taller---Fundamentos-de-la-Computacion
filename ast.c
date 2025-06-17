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

ASTNode *crearNodoString(char* texto) {
    ASTNode *nodo = crearNodo(STRING_LITERAL);
    nodo->str = texto;
    return nodo;
}

ASTNode *crearNodoPrograma(ASTNode *instruccion, ASTNode *programa) {
    ASTNode *nodo = crearNodo(PROGRAMA);
    nodo->programa.instruccion = instruccion;
    nodo->programa.programa = programa;
    return nodo;
}

ASTNode *crearNodoFuncion(char *nombre, ASTNode *parametros, ASTNode *cuerpo) {
    ASTNode *nodo = crearNodo(FUNCION);
    nodo->funcion.nombre = strdup(nombre);
    nodo->funcion.parametros = parametros;
    nodo->funcion.cuerpo = cuerpo;
    return nodo;
}

ASTNode *crearNodoPrint(ASTNode *expr) {
    ASTNode *nodo = crearNodo(PRINT);
    nodo->print.expresion = expr;
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

ASTNode* crearNodoOperacion(ASTNodeType op, ASTNode *izq, ASTNode *der) {
    ASTNode* nodo = crearNodo(OPERACION);
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

ASTNode *crearNodoDeclaracionFuncion(char *nombre, ASTNode *parametros, ASTNode *cuerpo) {
    ASTNode *nodo = crearNodo(DECLARACION_FUNCION);
    nodo->funcion_decl.nombre = strdup(nombre);
    nodo->funcion_decl.parametros = parametros;
    nodo->funcion_decl.cuerpo = cuerpo;
    return nodo;
}

ASTNode *crearNodoLlamadoFuncion(char *nombre, ASTNode *argumentos) {
    ASTNode *nodo = crearNodo(LLAMADO_FUNCION);
    nodo->funcion_llamada.nombre = strdup(nombre);
    nodo->funcion_llamada.argumentos = argumentos;
    return nodo;
}

ASTNode *crearNodoLista(ASTNode *actual, ASTNode *siguiente) {
    ASTNode *nodo = crearNodo(LISTA_PARAMETROS); // o LISTA_ARGUMENTOS, es el mismo struct
    nodo->lista.actual = actual;
    nodo->lista.siguiente = siguiente;
    return nodo;
}

ASTNode* crearNodoListaParametros(char* nombre, ASTNode* siguiente) {
    ASTNode* nodo = crearNodo(LISTA_PARAMETROS);
    ASTNode* identificador = crearNodoIdentificador(nombre);
    nodo->lista.actual = identificador;
    nodo->lista.siguiente = siguiente;
    return nodo;
}

ASTNode* crearNodoListaArgumentos(ASTNode* valor, ASTNode* siguiente) {
    ASTNode* nodo = crearNodo(LISTA_ARGUMENTOS);
    nodo->lista.actual = valor;
    nodo->lista.siguiente = siguiente;
    return nodo;
}

ASTNode* buscarFuncion(ASTNode* nodo, const char* nombre) {
    if (!nodo) return NULL;

    if (nodo->tipo == DECLARACION_FUNCION && strcmp(nodo->funcion_decl.nombre, nombre) == 0)
        return nodo;

    ASTNode* resultado = NULL;

    if (nodo->tipo == PROGRAMA) {
        resultado = buscarFuncion(nodo->programa.instruccion, nombre);
        if (resultado) return resultado;
        return buscarFuncion(nodo->programa.programa, nombre);
    }

    return NULL;
}

// Evaluación para pruebas simples
int evaluar(ASTNode *n) {
    if (!n) return 0;
    switch (n->tipo) {
        case NUMERO:
            return n->numero.valor;
        case IDENTIFICADOR:
            return n->identificador.valor;
        case OPERACION:
            switch (n->operacion.operador) {
                case OP_ADD:
                    return evaluar(n->operacion.izq) + evaluar(n->operacion.der);
                case OP_SUB:
                    return evaluar(n->operacion.izq) - evaluar(n->operacion.der);
                case OP_MUL:
                    return evaluar(n->operacion.izq) * evaluar(n->operacion.der);
                case OP_DIV:
                    return evaluar(n->operacion.izq) / evaluar(n->operacion.der);
                case OP_LT:
                    return evaluar(n->operacion.izq) < evaluar(n->operacion.der);
                case OP_LE:
                    return evaluar(n->operacion.izq) <= evaluar(n->operacion.der);
                case OP_GT:
                    return evaluar(n->operacion.izq) > evaluar(n->operacion.der);
                case OP_GE:
                    return evaluar(n->operacion.izq) >= evaluar(n->operacion.der);
                case OP_EQ:
                    return evaluar(n->operacion.izq) == evaluar(n->operacion.der);
                case OP_NE:
                    return evaluar(n->operacion.izq) != evaluar(n->operacion.der);
                default:
                    return 0;
            }
        case LLAMADO_FUNCION: {
            /* evalúa argumentos en un arreglo */
            int vals[10];         /* limite simple */
            int i = 0;
            for (ASTNode *arg = n->funcion_llamada.argumentos; arg; arg = arg->lista.siguiente)
                vals[i++] = evaluar(arg->lista.actual);
            extern ASTNode *raiz;
            ASTNode *fn = buscarFuncion(raiz, n->funcion_llamada.nombre);
            return ejecutarFuncion(fn, vals, i);
        }
        case RETURN:
            return evaluar(n->retorno.expresion);
        default:
            return 0;
    }
}

int evaluarOperacion(int izq, int der, char op) {
    switch (op) {
        case '+': return izq + der;
        case '-': return izq - der;
        case '*': return izq * der;
        case '/': return der != 0 ? izq / der : 0;
        default:
            fprintf(stderr, "Operador no soportado: %c\n", op);
            return 0;
    }
}

int ejecutarFuncion(ASTNode *fn, int *args, int n_args) {
    if (!fn) {
        fprintf(stderr, "Error: función no encontrada.\n");
        return 0;
    }
    ASTNode *param = fn->funcion.parametros;
    for (int i = 0; i < n_args && param; ++i, param = param->lista.siguiente)
        if (param->lista.actual->tipo == IDENTIFICADOR)
            param->lista.actual->identificador.valor = args[i];

    ASTNode *inst = fn->funcion.cuerpo;
    while (inst) {
        if (inst->tipo == RETURN)
            return evaluar(inst->retorno.expresion);
        inst = inst->programa.programa;
    }
    return 0;
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
        case OP_ADD:
            printf("OPERACION +\n");
            imprimirAST(nodo->operacion.izq, nivel + 1);
            imprimirAST(nodo->operacion.der, nivel + 1);
            break;
        case OP_SUB:
            printf("OPERACION -\n");
            imprimirAST(nodo->operacion.izq, nivel + 1);
            imprimirAST(nodo->operacion.der, nivel + 1);
            break;
        case OP_MUL:
            printf("OPERACION *\n");
            imprimirAST(nodo->operacion.izq, nivel + 1);
            imprimirAST(nodo->operacion.der, nivel + 1);
            break;
        case OP_DIV:
            printf("OPERACION /\n");
            imprimirAST(nodo->operacion.izq, nivel + 1);
            imprimirAST(nodo->operacion.der, nivel + 1);
            break;
        case OP_LE:
            printf("OPERACION <=\n");
            imprimirAST(nodo->operacion.izq, nivel + 1);
            imprimirAST(nodo->operacion.der, nivel + 1);
            break;
        case OP_GE:
            printf("OPERACION >=\n");
            imprimirAST(nodo->operacion.izq, nivel + 1);
            imprimirAST(nodo->operacion.der, nivel + 1);
            break;
        case OP_EQ:
            printf("OPERACION ==\n");
            imprimirAST(nodo->operacion.izq, nivel + 1);
            imprimirAST(nodo->operacion.der, nivel + 1);
            break;
        case OP_NE:
            printf("OPERACION !=\n");
            imprimirAST(nodo->operacion.izq, nivel + 1);
            imprimirAST(nodo->operacion.der, nivel + 1);
            break;
        case OP_LT:
            printf("OPERACION <\n");
            imprimirAST(nodo->operacion.izq, nivel + 1);
            imprimirAST(nodo->operacion.der, nivel + 1);
            break;
        case OP_GT:
            printf("OPERACION >\n");
            imprimirAST(nodo->operacion.izq, nivel + 1);
            imprimirAST(nodo->operacion.der, nivel + 1);
            break;
        
        case NUMERO:
            printf("NUMERO %d\n", nodo->numero.valor);
            break;
        case IDENTIFICADOR:
            printf("IDENTIFICADOR %s\n", nodo->identificador.nombre);
            break;
        case FUNCION:
            printf("FUNCION %s\n", nodo->funcion.nombre);
            imprimirAST(nodo->funcion.parametros, nivel + 1);
            imprimirAST(nodo->funcion.cuerpo, nivel + 1);
            break;

        case DECLARACION_FUNCION:
            printf("DECLARACION_FUNCION %s\n", nodo->funcion_decl.nombre);
            imprimirAST(nodo->funcion_decl.parametros, nivel + 1);
            imprimirAST(nodo->funcion_decl.cuerpo, nivel + 1);
            break;

        case LLAMADO_FUNCION:
            printf("LLAMADO_FUNCION %s\n", nodo->funcion_llamada.nombre);
            imprimirAST(nodo->funcion_llamada.argumentos, nivel + 1);
            break;

        case LISTA_PARAMETROS:
            printf("LISTA_PARAMETROS\n");
            imprimirAST(nodo->lista.actual, nivel + 1);
            imprimirAST(nodo->lista.siguiente, nivel + 1);
            break;
        
        case LISTA_ARGUMENTOS:
            printf("LISTA_ARGUMENTOS\n");
            imprimirAST(nodo->lista.actual, nivel + 1);
            imprimirAST(nodo->lista.siguiente, nivel + 1);
            break;
        case STRING_LITERAL:
            printf("STRING %s\n", nodo->str);
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
        case OP_ADD:
        case OP_SUB:
        case OP_LE:
        case OP_GE:
        case OP_EQ:
        case OP_NE:
        case OP_LT:
        case OP_GT:
            liberarAST(nodo->operacion.izq);
            liberarAST(nodo->operacion.der);
            break;
        case OP_MUL:
        case OP_DIV:
            liberarAST(nodo->operacion.izq);
            liberarAST(nodo->operacion.der);
            break;
        case IDENTIFICADOR:
            free(nodo->identificador.nombre);
            break;
        case FUNCION:
            free(nodo->funcion.nombre);
            liberarAST(nodo->funcion.parametros);
            liberarAST(nodo->funcion.cuerpo);
            break;
        case DECLARACION_FUNCION:
            free(nodo->funcion_decl.nombre);
            liberarAST(nodo->funcion_decl.parametros);
            liberarAST(nodo->funcion_decl.cuerpo);
            break;

        case LLAMADO_FUNCION:
            free(nodo->funcion_llamada.nombre);
            liberarAST(nodo->funcion_llamada.argumentos);
            break;

        case LISTA_PARAMETROS:
            liberarAST(nodo->lista.actual);
            liberarAST(nodo->lista.siguiente);
            break;
        case LISTA_ARGUMENTOS:
            liberarAST(nodo->lista.actual);
            liberarAST(nodo->lista.siguiente);
            break;
        case STRING_LITERAL:
            break;
        default: break;
    }

    free(nodo);
}

int evaluarAST(ASTNode *nodo) {
    if (!nodo) return 0;

    switch (nodo->tipo) {
        case PROGRAMA:
            for (ASTNode *prog = nodo; prog; prog = prog->programa.programa)
                evaluarAST(prog->programa.instruccion);
            return 0;

        case NUMERO:
            return nodo->numero.valor;

        case IDENTIFICADOR:
            return nodo->identificador.valor;

        case ASIGNACION:
            nodo->identificador.valor = evaluarAST(nodo->assign.expr);
            return nodo->identificador.valor;

        case PRINT:
            if (nodo->print.expresion->tipo == STRING_LITERAL)
                printf("%s\n", nodo->print.expresion->str);
            else
                printf("%d\n", evaluarAST(nodo->print.expresion));
            return 0;

        case RETURN:
            return evaluarAST(nodo->retorno.expresion);

        case OPERACION:
            return evaluarOperacion(
                evaluarAST(nodo->operacion.izq),
                evaluarAST(nodo->operacion.der),
                nodo->operacion.operador
            );

        case LLAMADO_FUNCION: {
            int args[10];
            int i = 0;
            for (ASTNode *arg = nodo->funcion_llamada.argumentos; arg; arg = arg->lista.siguiente)
                args[i++] = evaluarAST(arg->lista.actual);

            extern ASTNode *raiz;
            ASTNode *fn = buscarFuncion(raiz, nodo->funcion_llamada.nombre);
            return ejecutarFuncion(fn, args, i);
        }
        case DECLARACION_FUNCION:
            return 0;

        case FUNCION:
            return 0;

        case LISTA_PARAMETROS:
        case LISTA_ARGUMENTOS:
            evaluarAST(nodo->lista.actual);
            evaluarAST(nodo->lista.siguiente);
            return 0;

        case IF_ELSE:
            if (evaluarAST(nodo->ifelse.condicion))
                evaluarAST(nodo->ifelse.bloqueIf);
            else
                evaluarAST(nodo->ifelse.bloqueElse);
            return 0;

case STRING_LITERAL:
    printf("%s\n", nodo->str);
    return 0;

        case WHILE:
            while (evaluarAST(nodo->whili.condicion))
                evaluarAST(nodo->whili.bloque);
            return 0;

        default:
            return evaluar(nodo);
    }
}