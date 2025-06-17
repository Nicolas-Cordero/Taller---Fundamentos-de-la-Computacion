// Autor: Nicolás Cordero
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

// =======================
// Constructores de nodos
// =======================

ASTNode* create_number_node(int value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_NUMBER;
    node->number = value;
    return node;
}

ASTNode* create_variable_node(char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_VARIABLE;
    node->variable = strdup(name);
    return node;
}

ASTNode* create_assign_node(char* name, ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_ASSIGN;
    node->assign.id = strdup(name);
    node->assign.expr = expr;
    return node;
}

ASTNode* create_print_node(ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_PRINT;
    node->single_expr = expr;
    return node;
}

ASTNode* create_input_node(char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_INPUT;
    node->variable = strdup(name);
    return node;
}

ASTNode* create_binop_node(char op, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_BINOP;
    node->binop.op = op;
    node->binop.left = left;
    node->binop.right = right;
    return node;
}

ASTNode* create_if_else_node(ASTNode* cond, ASTNode* ifb, ASTNode* elseb) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_IF_ELSE;
    node->ifelse.condition = cond;
    node->ifelse.if_branch = ifb;
    node->ifelse.else_branch = elseb;
    return node;
}

ASTNode* create_while_node(ASTNode* cond, ASTNode* body) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_WHILE;
    node->whileloop.condition = cond;
    node->whileloop.body = body;
    return node;
}

// =======================
// Tabla de variables
// =======================

#define MAX_VARIABLES 100

typedef struct {
    char* name;
    int value;
} Variable;

Variable variables[MAX_VARIABLES];
int variable_count = 0;

int get_variable_value(char* name) {
    for (int i = 0; i < variable_count; ++i) {
        if (strcmp(variables[i].name, name) == 0)
            return variables[i].value;
    }
    fprintf(stderr, "Error: variable '%s' no definida.\n", name);
    exit(1);
}

void set_variable_value(char* name, int value) {
    for (int i = 0; i < variable_count; ++i) {
        if (strcmp(variables[i].name, name) == 0) {
            variables[i].value = value;
            return;
        }
    }
    if (variable_count >= MAX_VARIABLES) {
        fprintf(stderr, "Error: límite de variables alcanzado.\n");
        exit(1);
    }
    variables[variable_count].name = strdup(name);
    variables[variable_count].value = value;
    variable_count++;
}

// =======================
// Evaluación del AST
// =======================

void execute(ASTNode* node) {
    if (!node) return;

    switch (node->type) {
        case NODE_NUMBER:
            // No hace nada directamente
            break;

        case NODE_VARIABLE:
            // No hace nada directamente
            break;

        case NODE_ASSIGN: {
            int val = evaluate(node->assign.expr);
            set_variable_value(node->assign.id, val);
            break;
        }

        case NODE_PRINT: {
            int val = evaluate(node->single_expr);
            printf("%d\n", val);
            break;
        }

        case NODE_INPUT: {
            int val;
            printf("Ingresa valor para '%s': ", node->variable);
            scanf("%d", &val);
            set_variable_value(node->variable, val);
            break;
        }

        case NODE_BINOP:
            // Evaluación no usada aquí directamente
            break;

        case NODE_IF_ELSE: {
            if (evaluate(node->ifelse.condition))
                execute(node->ifelse.if_branch);
            else if (node->ifelse.else_branch)
                execute(node->ifelse.else_branch);
            break;
        }

        case NODE_WHILE: {
            while (evaluate(node->whileloop.condition))
                execute(node->whileloop.body);
            break;
        }
    }
}

int evaluate(ASTNode* node) {
    switch (node->type) {
        case NODE_NUMBER:
            return node->number;
        case NODE_VARIABLE:
            return get_variable_value(node->variable);
        case NODE_BINOP: {
            int left = evaluate(node->binop.left);
            int right = evaluate(node->binop.right);
            switch (node->binop.op) {
                case '+': return left + right;
                case '-': return left - right;
                case '*': return left * right;
                case '/': return right != 0 ? left / right : 0;
                case '%': return right != 0 ? left % right : 0;
                case '<': return left < right;
                case '>': return left > right;
                case '=': return left == right;
                default:
                    fprintf(stderr, "Operador desconocido: %c\n", node->binop.op);
                    exit(1);
            }
        }
        default:
            fprintf(stderr, "Error: tipo de nodo inválido en evaluate().\n");
            exit(1);
    }
}

// =======================
// Liberación de memoria
// =======================

void free_ast(ASTNode* node) {
    if (!node) return;

    switch (node->type) {
        case NODE_NUMBER:
            break;
        case NODE_VARIABLE:
        case NODE_INPUT:
            free(node->variable);
            break;
        case NODE_ASSIGN:
            free(node->assign.id);
            free_ast(node->assign.expr);
            break;
        case NODE_PRINT:
            free_ast(node->single_expr);
            break;
        case NODE_BINOP:
            free_ast(node->binop.left);
            free_ast(node->binop.right);
            break;
        case NODE_IF_ELSE:
            free_ast(node->ifelse.condition);
            free_ast(node->ifelse.if_branch);
            if (node->ifelse.else_branch)
                free_ast(node->ifelse.else_branch);
            break;
        case NODE_WHILE:
            free_ast(node->whileloop.condition);
            free_ast(node->whileloop.body);
            break;
    }

    free(node);
}