// ast.h

#ifndef AST_H
#define AST_H
#define MAX_VARS 256

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

    NODE_STR_LITERAL,
    NODE_IDENTIFICADOR,

    NODE_OPERACION_REL,  

    NODE_INPUT,
    NODE_IF_ELSE,
    NODE_WHILE,

    NODE_FUNCION,
    NODE_FUNCION_LLAMADA,

    NODE_LLAMADO_FUNCION,

} ASTNodeType;

typedef struct {
    VarType tipo;
    int valor_entero;
    char valor_string[256];
} Valor;

typedef enum {
    TYPE_INT,
    TYPE_STRING
} VarType;

typedef struct {
    char name[64];
    VarType type;
    int int_val;         
    char str_val[256];    
} Symbol;

typedef struct {
    char nombre[64];
    Valor valor;
} EntradaTS;

typedef struct {
    EntradaTS entradas[MAX_VARS];
    int cantidad;
} TablaSimbolos;



Valor evaluarAST(ASTNode* nodo);  // nueva función de evaluación unificada

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
    struct {
        ASTNode* mensaje;
    char *identificador;
    VarType tipo; 
} input;
    struct { struct ASTNode *condicion, *bloqueIf, *bloqueElse; } ifelse;
    struct { struct ASTNode *condicion, *bloque; } whili;
    struct { struct ASTNode *expresion; } retorno;
    struct { char operador; struct ASTNode *izq, *der; } operacion;
    struct { int valor; } numero;
    struct { char *nombre; } identificador;

    struct { char *nombre; struct ASTNode *argumentos; } funcion_llamada;
    struct { struct ASTNode *actual; struct ASTNode *siguiente; } lista;

    struct { struct ASTNode *param, *sig; } parametros;
    struct { struct ASTNode *arg, *sig; } argumentos;

    struct {
            char* identificador;
            ASTNode* expr;
            } assign;

    struct {
                char* nombre;
                VarType tipo_retorno;
                ASTNode* parametros;
                ASTNode* cuerpo;
            } funcion_decl;

    struct {
            char* valor;
        } str_literal;

    struct {
                char* nombre;
                ASTNode* parametros;
                ASTNode* cuerpo;
            } funcion;
    struct {
            char operador_rel; // como '<', '==', etc.
            ASTNode* izquierda;
            ASTNode* derecha;
        } operacion_rel;
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
ASTNode* crearNodoOperacionRel(char operador, ASTNode* izquierda, ASTNode* derecha);

ASTNode* crearValorEntero(int val);

ASTNode* buscarFuncion(const char* nombre);



ASTNode* crearValorString(char* texto);


ASTNode* crearNodoVariable(char* nombre);


int evaluar(ASTNode *nodo);
void imprimirAST(ASTNode *nodo, int nivel);
void liberarAST(ASTNode *nodo);
extern ASTNode* listaFunciones;

TablaSimbolos* crearTablaSimbolos();
void insertarVariable(TablaSimbolos* ts, const char* nombre, Valor val);
Valor obtenerVariable(TablaSimbolos* ts, const char* nombre);
void liberarTablaSimbolos(TablaSimbolos* ts);
Valor evaluarASTConContexto(ASTNode* nodo, TablaSimbolos* ts);

ASTNode* crearNodoString(char* texto);
ASTNode* crearNodoInput(char* id, VarType tipo);

#endif