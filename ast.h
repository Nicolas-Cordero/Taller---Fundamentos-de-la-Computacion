// ast.h

#ifndef AST_H
#define AST_H

typedef enum {
    PROGRAMA,
    PRINT,
    ASIGNACION,
    INPUT,
    IF_ELSE,
    WHILE,
    RETURN,
    OPERACION,
    NUMERO,
    IDENTIFICADOR
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType tipo;
    union {
        struct { struct ASTNode *instruccion, *programa; } programa;
        struct { struct ASTNode *expresion; } print;
        struct { char *identificador; struct ASTNode *expr; } assign;
        struct { char *identificador; } input;
        struct { struct ASTNode *condicion, *bloqueIf, *bloqueElse; } ifelse;
        struct { struct ASTNode *condicion, *bloque; } whili;
        struct { struct ASTNode *expresion; } retorno;
        struct { char operador; struct ASTNode *izq, *der; } operacion;
        struct { int valor; } numero;
        struct { char *nombre; } identificador;
    };
} ASTNode;

// Declaraciones de funciones (prototipos)
ASTNode *crearNodo(ASTNodeType tipo);
ASTNode *crearNodoPrograma(ASTNode *instruccion, ASTNode *programa);
ASTNode *crearNodoPrint(ASTNode *expresion);
ASTNode *crearNodoAsignacion(char *id, ASTNode *expr);
ASTNode *crearNodoInput(char *id);
ASTNode *crearNodoIfElse(ASTNode *cond, ASTNode *bloqueIf, ASTNode *bloqueElse);
ASTNode *crearNodoWhile(ASTNode *cond, ASTNode *bloque);
ASTNode *crearNodoReturn(ASTNode *expr);
ASTNode *crearNodoOperacion(char op, ASTNode *izq, ASTNode *der);
ASTNode *crearNodoNumero(int valor);
ASTNode *crearNodoIdentificador(char *id);

int evaluar(ASTNode *nodo);
void imprimirAST(ASTNode *nodo, int nivel);
void liberarAST(ASTNode *nodo);

#endif