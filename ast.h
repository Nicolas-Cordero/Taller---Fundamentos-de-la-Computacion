// ast.h

#ifndef AST_H
#define AST_H

typedef struct ASTNode ASTNode; 

typedef enum {
    STRING_LITERAL,
    PROGRAMA,
    PRINT,
    ASIGNACION,
    INPUT,
    IF_ELSE,
    WHILE,
    RETURN,
    OPERACION,
    NUMERO,
    IDENTIFICADOR,
    DECLARACION_FUNCION,
    LLAMADO_FUNCION,
    PARAMETROS,
    ARGUMENTOS,
    LISTA_PARAMETROS,
    LISTA_ARGUMENTOS,
    FUNCION,
    TIPO_FUNCION,
    NODO_PARAMETRO,
    NODO_ARGUMENTO,
    NODO_FUNCION,

    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_LE,
    OP_GE,
    OP_EQ,
    OP_NE,
    OP_LT,
    OP_GT

} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType tipo;
    union {
        struct { struct ASTNode *instruccion, *programa; } programa;
        struct { struct ASTNode *expresion; } print;
        struct {
    char *identificador;
    ASTNode *expr;
    int valor;  // <<<<< Agregado aquí
} assign;
        struct { char *identificador; } input;
        struct { struct ASTNode *condicion, *bloqueIf, *bloqueElse; } ifelse;
        struct { struct ASTNode *condicion, *bloque; } whili;
        struct { struct ASTNode *expresion; } retorno;
        struct { ASTNodeType operador; struct ASTNode *izq, *der; } operacion;
        struct { int valor; } numero;

        struct {
            char *nombre;
            int valor;
        } identificador;

        struct { char *nombre; struct ASTNode *parametros; struct ASTNode *cuerpo; } funcion_decl;
        struct { char *nombre; struct ASTNode *argumentos; } funcion_llamada;
        struct { struct ASTNode *actual; struct ASTNode *siguiente; } lista;

        struct { struct ASTNode *param, *sig; } parametros;
        struct { struct ASTNode *arg, *sig; } argumentos;
        
        struct {
                char* nombre;
                ASTNode* parametros;
                ASTNode* cuerpo;
            } funcion;
    };
    char *str;
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
ASTNode *crearNodoOperacion(ASTNodeType op, ASTNode *izq, ASTNode *der);
ASTNode *crearNodoNumero(int valor);
ASTNode *crearNodoIdentificador(char *id);
ASTNode *crearNodoDeclaracionFuncion(char *nombre, ASTNode *parametros, ASTNode *cuerpo);
ASTNode *crearNodoLlamadoFuncion(char *nombre, ASTNode *argumentos);
ASTNode *crearNodoLista(ASTNode *actual, ASTNode *siguiente);
ASTNode *crearNodoFuncion(char *nombre, ASTNode *parametros, ASTNode *cuerpo);
ASTNode* crearNodoListaParametros(char* nombre, ASTNode* siguiente);
ASTNode* crearNodoListaArgumentos(ASTNode* valor, ASTNode* siguiente);
ASTNode* crearNodoString(char* valor);


ASTNode *buscarFuncion(ASTNode *programa, const char *nombre);
int      ejecutarFuncion(ASTNode *fn, int *args, int n_args);

int evaluarAST(ASTNode *nodo);
void imprimirAST(ASTNode *nodo, int nivel);
void liberarAST(ASTNode *nodo);

int evaluarOperacion(int izq, int der, char op);

#endif