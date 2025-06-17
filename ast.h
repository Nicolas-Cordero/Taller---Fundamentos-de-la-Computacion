#ifndef AST_H
#define AST_H

// Enumeración para tipos de nodos
typedef enum {
    NODE_INT,
    NODE_ID,
    NODE_BINOP,
    NODE_ASSIGN,
    NODE_INPUT,
    NODE_OUTPUT,
    NODE_IF,
    NODE_WHILE,
    NODE_RETURN,
    NODE_STMT_LIST
} NodeType;

// Enumeración para operadores
typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_POW,
    OP_GT,
    OP_LT,
    OP_EQ
} OpType;

// Estructura de un nodo del AST
typedef struct ASTNode {
    NodeType type;
    union {
        int value;                 // NODE_INT
        char* name;               // NODE_ID
        struct {
            OpType op;
            struct ASTNode* left;
            struct ASTNode* right;
        } binop;
        struct {
            char* name;
            struct ASTNode* expr;
        } assign;
        struct {
            struct ASTNode* cond;
            struct ASTNode* then_branch;
            struct ASTNode* else_branch;
        } if_stmt;
        struct {
            struct ASTNode* cond;
            struct ASTNode* body;
        } while_stmt;
        struct ASTNode* expr;     // NODE_INPUT, NODE_OUTPUT, RETURN
        struct {
            struct ASTNode** stmts;
            int count;
        } stmt_list;
    };
} ASTNode;

// Funciones de construcción
ASTNode* n_int(int value);
ASTNode* n_id(char* name);
ASTNode* n_binop(OpType op, ASTNode* left, ASTNode* right);
ASTNode* n_assign(char* name, ASTNode* expr);
ASTNode* n_input(ASTNode* expr);
ASTNode* n_output(ASTNode* expr);
ASTNode* n_if(ASTNode* cond, ASTNode* then_branch, ASTNode* else_branch);
ASTNode* n_while(ASTNode* cond, ASTNode* body);
ASTNode* n_return(ASTNode* expr);
ASTNode* n_stmt_list(ASTNode** stmts, int count);

void print_ast(ASTNode* node, int indent);

// Futuro: liberación de memoria
// void free_ast(ASTNode* node);

#endif