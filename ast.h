#ifndef AST_H
#define AST_H

/* Enumeraciones primero (¡muy importante!) */
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

typedef enum {
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_EQ, OP_NEQ
} BinOp;

/* Declaración del tipo principal */
typedef struct ASTNode ASTNode;

struct ASTNode {
    NodeType type;
    union {
        int int_val;
        char *id;

        struct {
            BinOp op;
            ASTNode *lhs, *rhs;
        } bin;

        struct { char *id; } decl;
        struct { char *id; ASTNode *expr; } assign;
        struct { char *id; } in;
        ASTNode *out;
        ASTNode *ret;

        struct {
            ASTNode *cond;
            ASTNode *then_blk;
            ASTNode *else_blk;
        } ifs;

        struct {
            ASTNode *cond;
            ASTNode *body;
        } whiles;

        struct {
            char *name;
            char **params;
            int param_count;
            ASTNode *body;
        } func_def;

        struct {
            char *name;
            ASTNode **args;
            int arg_count;
        } func_call;

        struct {
            ASTNode **stmts;
            int stmt_count;
        } block;
    };
};

/* Constructores */
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

/* Debug */
void print_ast(ASTNode* node, int indent);

#endif