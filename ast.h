// ast.h

#ifndef AST_H
#define AST_H

typedef struct ASTNode ASTNode; 

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

    STRING_LITERAL,

} ASTNodeType;

typedef enum {
    TYPE_INT,
    TYPE_STRING
} VarType;

typedef struct {
    char name[64];
    VarType type;
} Symbol;

#define MAX_SYMBOLS 1024
extern Symbol symbol_table[MAX_SYMBOLS];
extern int symbol_count;

int add_symbol(const char* name, VarType type);
VarType get_symbol_type(const char* name);
int symbol_exists(const char* name);



typedef struct ASTNode {
    ASTNodeType tipo;
    VarType tipo_resultado;
    struct { struct ASTNode *instruccion, *programa; } programa;
    struct { struct ASTNode *expresion; } print;
    struct { char *identificador; struct ASTNode *expr; } assign;
    struct { char *identificador; } input;
    struct { struct ASTNode *condicion, *bloqueIf, *bloqueElse; } ifelse;
    struct { struct ASTNode *condicion, *bloque; } whili;
    struct { struct ASTNode *expresion; } retorno;
    struct { char operador; struct ASTNode *izq, *der; } operacion;
    struct { int valor; } numero;
    struct { char* valor; } string_literal;
    struct { char *nombre; } identificador;

    struct { char *nombre; struct ASTNode *parametros; struct ASTNode *cuerpo; } funcion_decl;
    struct { char *nombre; struct ASTNode *argumentos; } funcion_llamada;
    struct { struct ASTNode *actual; struct ASTNode *siguiente; } lista;

    struct { struct ASTNode *param, *sig; } parametros;
    struct { struct ASTNode *arg, *sig; } argumentos;

    struct {
            char* valor;
        } string_literal;

    struct {
                char* nombre;
                ASTNode* parametros;
                ASTNode* cuerpo;
            } funcion;
    
    union {
        int valor;
        char* nombre;
        struct {
            struct ASTNode* izq;
            struct ASTNode* der;
        };
    };
    struct ASTNode* siguiente;
}ASTNode;


// Declaraciones de funciones (prototipos)
ASTNode *crearNodo(ASTNodeType tipo);
ASTNode *crearNodoPrograma(ASTNode *instruccion, ASTNode *programa);
ASTNode *crearNodoPrint(ASTNode *expresion);
ASTNode *crearNodoAsignacion(char *id, ASTNode *expr);
ASTNode *crearNodoInput(char *id);
ASTNode *crearNodoIfElse(ASTNode *cond, ASTNode *bloqueIf, ASTNode *bloqueElse);
ASTNode *crearNodoWhile(ASTNode *cond, ASTNode *bloque);
ASTNode* crearNodoReturn(ASTNode* expr, VarType tipo_esperado);
ASTNode *crearNodoOperacion(char op, ASTNode *izq, ASTNode *der);
ASTNode *crearNodoNumero(int valor);
ASTNode *crearNodoIdentificador(char *id);
ASTNode *crearNodoDeclaracionFuncion(char *nombre, ASTNode *parametros, ASTNode *cuerpo);
ASTNode *crearNodoLlamadoFuncion(char *nombre, ASTNode *argumentos);
ASTNode *crearNodoLista(ASTNode *actual, ASTNode *siguiente);
ASTNode *crearNodoParametros(ASTNode *param, ASTNode *sig);
ASTNode *crearNodoArgumentos(ASTNode *arg, ASTNode *sig);
ASTNode *crearNodoFuncion(char *nombre, ASTNode *parametros, ASTNode *cuerpo);
ASTNode* crearNodoListaParametros(char* nombre, ASTNode* siguiente);
ASTNode* crearNodoListaArgumentos(ASTNode* valor, ASTNode* siguiente);

ASTNode* crearNodoVariable(char* nombre);


int evaluar(ASTNode *nodo);
void imprimirAST(ASTNode *nodo, int nivel);
void liberarAST(ASTNode *nodo);

#endif