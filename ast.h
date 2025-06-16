#ifndef AST_H
#define AST_H

typedef enum {
    NODE_INT_LITERAL,
    NODE_FLOAT_LITERAL,
    NODE_STRING_LITERAL,
    NODE_IDENTIFIER,
    NODE_BINARY_OP,
    NODE_ASSIGNMENT,
    NODE_VAR_DECL,
    NODE_INPUT,
    NODE_OUTPUT,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    NODE_DO_WHILE,
    NODE_RETURN,
    NODE_BLOCK
} NodeType;

typedef enum {
    OP_PLUS,
    OP_MINUS,
    OP_MUL,
    OP_DIV,
    OP_EQ,
    OP_NEQ,
    OP_LT,
    OP_GT,
    OP_LEQ,
    OP_GEQ
} BinaryOpType;

typedef struct ASTNode {
    NodeType type;

    union {
        int int_val;
        float float_val;
        char* str_val;

        struct { char* name; } id;

        struct {
            BinaryOpType op;
            struct ASTNode* left;
            struct ASTNode* right;
        } binary;

        struct {
            char* id;
            struct ASTNode* expr;
        } assignment;

        struct {
            char* tipo;
            char* id;
        } declaration;

        struct { char* id; } io_read;
        struct ASTNode* io_write;

        struct {
            struct ASTNode* condition;
            struct ASTNode* then_branch;
            struct ASTNode* else_branch;
        } if_stmt;

        struct {
            struct ASTNode* condition;
            struct ASTNode* body;
        } while_stmt;

        struct {
            struct ASTNode* init;
            struct ASTNode* cond;
            struct ASTNode* step;
            struct ASTNode* body;
        } for_stmt;

        struct {
            struct ASTNode* body;
            struct ASTNode* condition;
        } dowhile_stmt;

        struct ASTNode* return_expr;

        struct {
            struct ASTNode** stmts;
            int stmt_count;
        } block;
    };
} ASTNode;

ASTNode* make_int_literal(int val);
ASTNode* make_float_literal(float val);
ASTNode* make_string_literal(char* val);
ASTNode* make_identifier(char* name);
ASTNode* make_binary_op(BinaryOpType op, ASTNode* left, ASTNode* right);
ASTNode* make_assignment(char* id, ASTNode* expr);
ASTNode* make_declaration(char* tipo, char* id);
ASTNode* make_input(char* id);
ASTNode* make_output(ASTNode* expr);
ASTNode* make_if(ASTNode* cond, ASTNode* then_branch, ASTNode* else_branch);
ASTNode* make_while(ASTNode* cond, ASTNode* body);
ASTNode* make_for(ASTNode* init, ASTNode* cond, ASTNode* step, ASTNode* body);
ASTNode* make_dowhile(ASTNode* body, ASTNode* cond);
ASTNode* make_return(ASTNode* expr);
ASTNode* make_block(ASTNode** stmts, int count);

void print_ast(ASTNode* node, int indent);

#endif
