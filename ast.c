#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

// Validar malloc (macro de ayuda)
#define CHECK_ALLOC(ptr) if (!(ptr)) { fprintf(stderr, "Error: memoria insuficiente\n"); exit(EXIT_FAILURE); }

ASTNode* n_int(int value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    CHECK_ALLOC(node);
    node->type = NODE_INT;
    node->value = value;
    return node;
}

ASTNode* n_id(char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    CHECK_ALLOC(node);
    node->type = NODE_ID;
    node->name = strdup(name);
    CHECK_ALLOC(node->name);
    return node;
}

ASTNode* n_binop(OpType op, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    CHECK_ALLOC(node);
    node->type = NODE_BINOP;
    node->binop.op = op;
    node->binop.left = left;
    node->binop.right = right;
    return node;
}

ASTNode* n_assign(char* name, ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    CHECK_ALLOC(node);
    node->type = NODE_ASSIGN;
    node->assign.name = strdup(name);
    CHECK_ALLOC(node->assign.name);
    node->assign.expr = expr;
    return node;
}

ASTNode* n_input(ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    CHECK_ALLOC(node);
    node->type = NODE_INPUT;
    node->expr = expr;
    return node;
}

ASTNode* n_output(ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    CHECK_ALLOC(node);
    node->type = NODE_OUTPUT;
    node->expr = expr;
    return node;
}

ASTNode* n_if(ASTNode* cond, ASTNode* then_branch, ASTNode* else_branch) {
    ASTNode* node = malloc(sizeof(ASTNode));
    CHECK_ALLOC(node);
    node->type = NODE_IF;
    node->if_stmt.cond = cond;
    node->if_stmt.then_branch = then_branch;
    node->if_stmt.else_branch = else_branch;
    return node;
}

ASTNode* n_while(ASTNode* cond, ASTNode* body) {
    ASTNode* node = malloc(sizeof(ASTNode));
    CHECK_ALLOC(node);
    node->type = NODE_WHILE;
    node->while_stmt.cond = cond;
    node->while_stmt.body = body;
    return node;
}

ASTNode* n_return(ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    CHECK_ALLOC(node);
    node->type = NODE_RETURN;
    node->expr = expr;
    return node;
}

ASTNode* n_stmt_list(ASTNode** stmts, int count) {
    ASTNode* node = malloc(sizeof(ASTNode));
    CHECK_ALLOC(node);
    node->type = NODE_STMT_LIST;
    node->stmt_list.stmts = stmts;
    node->stmt_list.count = count;
    return node;
}

void print_ast(ASTNode* node, int indent) {
    if (!node) return;
    for (int i = 0; i < indent; ++i) printf("  ");
    switch (node->type) {
        case NODE_INT:
            printf("INT: %d\n", node->value);
            break;
        case NODE_ID:
            printf("ID: %s\n", node->name);
            break;
        case NODE_BINOP:
            printf("BINOP: %d\n", node->binop.op);
            print_ast(node->binop.left, indent + 1);
            print_ast(node->binop.right, indent + 1);
            break;
        case NODE_ASSIGN:
            printf("ASSIGN: %s\n", node->assign.name);
            print_ast(node->assign.expr, indent + 1);
            break;
        case NODE_INPUT:
            printf("INPUT:\n");
            print_ast(node->expr, indent + 1);
            break;
        case NODE_OUTPUT:
            printf("OUTPUT:\n");
            print_ast(node->expr, indent + 1);
            break;
        case NODE_IF:
            printf("IF:\n");
            print_ast(node->if_stmt.cond, indent + 1);
            print_ast(node->if_stmt.then_branch, indent + 1);
            print_ast(node->if_stmt.else_branch, indent + 1);
            break;
        case NODE_WHILE:
            printf("WHILE:\n");
            print_ast(node->while_stmt.cond, indent + 1);
            print_ast(node->while_stmt.body, indent + 1);
            break;
        case NODE_RETURN:
            printf("RETURN:\n");
            print_ast(node->expr, indent + 1);
            break;
        case NODE_STMT_LIST:
            printf("STATEMENTS:\n");
            for (int i = 0; i < node->stmt_list.count; ++i) {
                print_ast(node->stmt_list.stmts[i], indent + 1);
            }
            break;
    }
}

/*
// Futuro: liberar memoria del AST
void free_ast(ASTNode* node) {
    if (!node) return;
    switch (node->type) {
        case NODE_ID:
            free(node->name);
            break;
        case NODE_BINOP:
            free_ast(node->binop.left);
            free_ast(node->binop.right);
            break;
        case NODE_ASSIGN:
            free(node->assign.name);
            free_ast(node->assign.expr);
            break;
        case NODE_INPUT:
        case NODE_OUTPUT:
        case NODE_RETURN:
            free_ast(node->expr);
            break;
        case NODE_IF:
            free_ast(node->if_stmt.cond);
            free_ast(node->if_stmt.then_branch);
            free_ast(node->if_stmt.else_branch);
            break;
        case NODE_WHILE:
            free_ast(node->while_stmt.cond);
            free_ast(node->while_stmt.body);
            break;
        case NODE_STMT_LIST:
            for (int i = 0; i < node->stmt_list.count; ++i)
                free_ast(node->stmt_list.stmts[i]);
            free(node->stmt_list.stmts);
            break;
        default: break;
    }
    free(node);
}
*/