#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

#define NEW_NODE(t) \
    ASTNode* node = malloc(sizeof(ASTNode)); \
    node->type = t; \
    return node;

ASTNode* make_int_literal(int val) {
    NEW_NODE(NODE_INT_LITERAL);
    node->int_val = val;
}

ASTNode* make_float_literal(float val) {
    NEW_NODE(NODE_FLOAT_LITERAL);
    node->float_val = val;
}

ASTNode* make_string_literal(char* val) {
    NEW_NODE(NODE_STRING_LITERAL);
    node->str_val = strdup(val);
}

ASTNode* make_identifier(char* name) {
    NEW_NODE(NODE_IDENTIFIER);
    node->id.name = strdup(name);
}

ASTNode* make_binary_op(BinaryOpType op, ASTNode* left, ASTNode* right) {
    NEW_NODE(NODE_BINARY_OP);
    node->binary.op = op;
    node->binary.left = left;
    node->binary.right = right;
}

ASTNode* make_assignment(char* id, ASTNode* expr) {
    NEW_NODE(NODE_ASSIGNMENT);
    node->assignment.id = strdup(id);
    node->assignment.expr = expr;
}

ASTNode* make_declaration(char* tipo, char* id) {
    NEW_NODE(NODE_VAR_DECL);
    node->declaration.tipo = strdup(tipo);
    node->declaration.id = strdup(id);
}

ASTNode* make_input(char* id) {
    NEW_NODE(NODE_INPUT);
    node->io_read.id = strdup(id);
}

ASTNode* make_output(ASTNode* expr) {
    NEW_NODE(NODE_OUTPUT);
    node->io_write = expr;
}

ASTNode* make_if(ASTNode* cond, ASTNode* then_branch, ASTNode* else_branch) {
    NEW_NODE(NODE_IF);
    node->if_stmt.condition = cond;
    node->if_stmt.then_branch = then_branch;
    node->if_stmt.else_branch = else_branch;
}

ASTNode* make_while(ASTNode* cond, ASTNode* body) {
    NEW_NODE(NODE_WHILE);
    node->while_stmt.condition = cond;
    node->while_stmt.body = body;
}

ASTNode* make_for(ASTNode* init, ASTNode* cond, ASTNode* step, ASTNode* body) {
    NEW_NODE(NODE_FOR);
    node->for_stmt.init = init;
    node->for_stmt.cond = cond;
    node->for_stmt.step = step;
    node->for_stmt.body = body;
}

ASTNode* make_dowhile(ASTNode* body, ASTNode* cond) {
    NEW_NODE(NODE_DO_WHILE);
    node->dowhile_stmt.body = body;
    node->dowhile_stmt.condition = cond;
}

ASTNode* make_return(ASTNode* expr) {
    NEW_NODE(NODE_RETURN);
    node->return_expr = expr;
}

ASTNode* make_block(ASTNode** stmts, int count) {
    NEW_NODE(NODE_BLOCK);
    node->block.stmts = stmts;
    node->block.stmt_count = count;
}

/* Funciones auxiliares */
void print_indent(int indent) {
    for (int i = 0; i < indent; i++) printf("  ");
}

void print_ast(ASTNode* node, int indent) {
    if (!node) return;
    print_indent(indent);

    switch (node->type) {
        case NODE_INT_LITERAL:
            printf("INT_LITERAL(%d)\n", node->int_val); break;
        case NODE_FLOAT_LITERAL:
            printf("FLOAT_LITERAL(%f)\n", node->float_val); break;
        case NODE_STRING_LITERAL:
            printf("STRING_LITERAL(%s)\n", node->str_val); break;
        case NODE_IDENTIFIER:
            printf("IDENTIFIER(%s)\n", node->id.name); break;
        case NODE_BINARY_OP:
            printf("BINARY_OP(%d)\n", node->binary.op);
            print_ast(node->binary.left, indent + 1);
            print_ast(node->binary.right, indent + 1);
            break;
        case NODE_ASSIGNMENT:
            printf("ASSIGN(%s)\n", node->assignment.id);
            print_ast(node->assignment.expr, indent + 1);
            break;
        case NODE_VAR_DECL:
            printf("DECL(%s %s)\n", node->declaration.tipo, node->declaration.id);
            break;
        case NODE_INPUT:
            printf("INPUT(%s)\n", node->io_read.id); break;
        case NODE_OUTPUT:
            printf("OUTPUT\n");
            print_ast(node->io_write, indent + 1);
            break;
        case NODE_IF:
            printf("IF\n");
            print_ast(node->if_stmt.condition, indent + 1);
            print_ast(node->if_stmt.then_branch, indent + 1);
            if (node->if_stmt.else_branch)
                print_ast(node->if_stmt.else_branch, indent + 1);
            break;
        case NODE_WHILE:
            printf("WHILE\n");
            print_ast(node->while_stmt.condition, indent + 1);
            print_ast(node->while_stmt.body, indent + 1);
            break;
        case NODE_FOR:
            printf("FOR\n");
            print_ast(node->for_stmt.init, indent + 1);
            print_ast(node->for_stmt.cond, indent + 1);
            print_ast(node->for_stmt.step, indent + 1);
            print_ast(node->for_stmt.body, indent + 1);
            break;
        case NODE_DO_WHILE:
            printf("DO_WHILE\n");
            print_ast(node->dowhile_stmt.body, indent + 1);
            print_ast(node->dowhile_stmt.condition, indent + 1);
            break;
        case NODE_RETURN:
            printf("RETURN\n");
            print_ast(node->return_expr, indent + 1);
            break;
        case NODE_BLOCK:
            printf("BLOCK (%d statements)\n", node->block.stmt_count);
            for (int i = 0; i < node->block.stmt_count; i++)
                print_ast(node->block.stmts[i], indent + 1);
            break;
        default:
            printf("UNKNOWN NODE\n"); break;
    }
}
