#ifndef AST_H
#define AST_H

typedef struct ASTNode ASTNode;

typedef struct ASTNode {
    NodeType type;
    union {
        int int_val;               // literales enteros
        char *id;                  // identificadores (nombres de variable)

        struct {                   // binarios
            BinOp op;
            struct ASTNode *lhs, *rhs;
        } bin;

        struct {                   // declaración de variable
            char *id;
        } decl;

        struct {                   // asignación
            char *id;
            struct ASTNode *expr;
        } assign;

        struct { char *id; } in;   // input
        struct ASTNode *out;       // output

        struct ASTNode *ret;       // return expr

        struct {                   // if
            struct ASTNode *cond;
            struct ASTNode *then_blk;
            struct ASTNode *else_blk;
        } ifs;

        struct {                   // while
            struct ASTNode *cond;
            struct ASTNode *body;
        } whiles;

        struct {                   // función definida por usuario
            char *name;
            char **params;
            int param_count;
            struct ASTNode *body;
        } func_def;

        struct {                   // llamada a función
            char *name;
            struct ASTNode **args;
            int arg_count;
        } func_call;

        struct {                   // bloque de código
            struct ASTNode **stmts;
            int stmt_count;
        } block;
    };
} ASTNode;

#endif
/* tipos de nodos disponibles */
typedef enum {
    NODE_INT_LITERAL,
    NODE_IDENTIFIER,
    NODE_BINARY_OP,
    NODE_VAR_DECL,
    NODE_ASSIGN,
    NODE_INPUT,
    NODE_OUTPUT,
    NODE_RETURN,
    NODE_IF,
    NODE_WHILE,
    NODE_FUNCTION_DEF,
    NODE_FUNCTION_CALL,
    NODE_BLOCK
} NodeType;

/* operadores binarios */
typedef enum {
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_EQ, OP_NEQ
} BinOp;

/* nodo AST general */


/* constructores de nodos */
ASTNode* n_int(int v);
ASTNode* n_id(char* s);
ASTNode* n_bin(BinOp op, ASTNode* l, ASTNode* r);
ASTNode* n_decl(char* id);
ASTNode* n_assign(char* id, ASTNode* e);
ASTNode* n_input(char* id);
ASTNode* n_output(ASTNode* e);
ASTNode* n_return(ASTNode* e);
ASTNode* n_if(ASTNode* cond, ASTNode* then_blk, ASTNode* else_blk);
ASTNode* n_while(ASTNode* cond, ASTNode* body);
ASTNode* n_func_def(char* name, char** params, int param_count, ASTNode* body);
ASTNode* n_func_call(char* name, ASTNode** args, int arg_count);
ASTNode* n_block(ASTNode** stmts, int stmt_count);

/* impresión del AST */
void print_ast(ASTNode* node, int indent);
