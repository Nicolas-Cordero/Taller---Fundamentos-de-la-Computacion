// Autor: Nicolás Cordero
#ifndef AST_H
#define AST_H

typedef enum {
    NODE_NUMBER,
    NODE_VARIABLE,
    NODE_ASSIGN,
    NODE_PRINT,
    NODE_INPUT,
    NODE_BINOP,
    NODE_IF_ELSE,
    NODE_WHILE
} NodeType;

typedef struct ASTNode {
    NodeType type;
    union {
        int number;

        // Nodo variable
        char* variable;

        // Nodo asignación
        struct {
            char* id;
            struct ASTNode* expr;
        } assign;

        // Nodo de impresión/input
        struct ASTNode* single_expr;

        // Nodo operación binaria
        struct {
            char op;
            struct ASTNode* left;
            struct ASTNode* right;
        } binop;

        // Nodo if-else
        struct {
            struct ASTNode* condition;
            struct ASTNode* if_branch;
            struct ASTNode* else_branch;
        } ifelse;

        // Nodo while
        struct {
            struct ASTNode* condition;
            struct ASTNode* body;
        } whileloop;
    };
} ASTNode;

// Constructores de nodos
ASTNode* create_number_node(int value);
ASTNode* create_variable_node(char* name);
ASTNode* create_assign_node(char* name, ASTNode* expr);
ASTNode* create_print_node(ASTNode* expr);
ASTNode* create_input_node(char* name);
ASTNode* create_binop_node(char op, ASTNode* left, ASTNode* right);
ASTNode* create_if_else_node(ASTNode* cond, ASTNode* ifb, ASTNode* elseb);
ASTNode* create_while_node(ASTNode* cond, ASTNode* body);

// Función para ejecutar el AST
void execute(ASTNode* node);

// Liberar memoria
void free_ast(ASTNode* node);

#endif