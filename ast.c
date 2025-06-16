#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* constructores básicos */
ASTNode* n_int(int v) {
    ASTNode* n = malloc(sizeof(ASTNode));
    n->type = NODE_INT_LITERAL;
    n->int_val = v;
    return n;
}

ASTNode* n_id(char* s) {
    ASTNode* n = malloc(sizeof(ASTNode));
    n->type = NODE_IDENTIFIER;
    n->id = strdup(s);
    return n;
}

ASTNode* n_bin(BinOp op, ASTNode* l, ASTNode* r) {
    ASTNode* n = malloc(sizeof(ASTNode));
    n->type = NODE_BINARY_OP;
    n->bin.op = op;
    n->bin.lhs = l;
    n->bin.rhs = r;
    return n;
}

ASTNode* n_decl(char* id) {
    ASTNode* n = malloc(sizeof(ASTNode));
    n->type = NODE_VAR_DECL;
    n->decl.id = strdup(id);
    return n;
}

ASTNode* n_assign(char* id, ASTNode* e) {
    ASTNode* n = malloc(sizeof(ASTNode));
    n->type = NODE_ASSIGN;
    n->assign.id = strdup(id);
    n->assign.expr = e;
    return n;
}

ASTNode* n_input(char* id) {
    ASTNode* n = malloc(sizeof(ASTNode));
    n->type = NODE_INPUT;
    n->in.id = strdup(id);
    return n;
}

ASTNode* n_output(ASTNode* e) {
    ASTNode* n = malloc(sizeof(ASTNode));
    n->type = NODE_OUTPUT;
    n->out = e;
    return n;
}

ASTNode* n_return(ASTNode* e) {
    ASTNode* n = malloc(sizeof(ASTNode));
    n->type = NODE_RETURN;
    n->ret = e;
    return n;
}

ASTNode* n_if(ASTNode* cond, ASTNode* then_blk, ASTNode* else_blk) {
    ASTNode* n = malloc(sizeof(ASTNode));
    n->type = NODE_IF;
    n->ifs.cond = cond;
    n->ifs.then_blk = then_blk;
    n->ifs.else_blk = else_blk;
    return n;
}

ASTNode* n_while(ASTNode* cond, ASTNode* body) {
    ASTNode* n = malloc(sizeof(ASTNode));
    n->type = NODE_WHILE;
    n->whiles.cond = cond;
    n->whiles.body = body;
    return n;
}

ASTNode* n_func_def(char* name, char** params, int param_count, ASTNode* body) {
    ASTNode* n = malloc(sizeof(ASTNode));
    n->type = NODE_FUNCTION_DEF;
    n->func_def.name = strdup(name);
    n->func_def.param_count = param_count;
    n->func_def.params = malloc(sizeof(char*) * param_count);
    for (int i = 0; i < param_count; i++) {
        n->func_def.params[i] = strdup(params[i]);
    }
    n->func_def.body = body;
    return n;
}

ASTNode* n_func_call(char* name, ASTNode** args, int arg_count) {
    ASTNode* n = malloc(sizeof(ASTNode));
    n->type = NODE_FUNCTION_CALL;
    n->func_call.name = strdup(name);
    n->func_call.arg_count = arg_count;
    n->func_call.args = malloc(sizeof(ASTNode*) * arg_count);
    for (int i = 0; i < arg_count; i++) {
        n->func_call.args[i] = args[i];
    }
    return n;
}

ASTNode* n_block(ASTNode** stmts, int stmt_count) {
    ASTNode* n = malloc(sizeof(ASTNode));
    n->type = NODE_BLOCK;
    n->block.stmts = stmts;
    n->block.stmt_count = stmt_count;
    return n;
}

/* impresión recursiva */
void print_ast(ASTNode* node, int indent) {
    if (!node) return;
    for (int i = 0; i < indent; i++) printf("  ");

    switch (node->type) {
        case NODE_INT_LITERAL:
            printf("Int: %d\n", node->int_val); break;
        case NODE_IDENTIFIER:
            printf("ID: %s\n", node->id); break;
        case NODE_BINARY_OP:
            printf("Op: %d\n", node->bin.op);
            print_ast(node->bin.lhs, indent + 1);
            print_ast(node->bin.rhs, indent + 1);
            break;
        case NODE_VAR_DECL:
            printf("Decl: %s\n", node->decl.id); break;
        case NODE_ASSIGN:
            printf("Assign: %s\n", node->assign.id);
            print_ast(node->assign.expr, indent + 1);
            break;
        case NODE_INPUT:
            printf("Input: %s\n", node->in.id); break;
        case NODE_OUTPUT:
            printf("Output:\n");
            print_ast(node->out, indent + 1); break;
        case NODE_RETURN:
            printf("Return:\n");
            print_ast(node->ret, indent + 1); break;
        case NODE_IF:
            printf("If:\n");
            print_ast(node->ifs.cond, indent + 1);
            printf("%*sThen:\n", indent * 2, "");
            print_ast(node->ifs.then_blk, indent + 2);
            if (node->ifs.else_blk) {
                printf("%*sElse:\n", indent * 2, "");
                print_ast(node->ifs.else_blk, indent + 2);
            }
            break;
        case NODE_WHILE:
            printf("While:\n");
            print_ast(node->whiles.cond, indent + 1);
            print_ast(node->whiles.body, indent + 1);
            break;
        case NODE_FUNCTION_DEF:
            printf("FuncDef: %s(", node->func_def.name);
            for (int i = 0; i < node->func_def.param_count; i++) {
                printf("%s", node->func_def.params[i]);
                if (i < node->func_def.param_count - 1) printf(", ");
            }
            printf(")\n");
            print_ast(node->func_def.body, indent + 1);
            break;
        case NODE_FUNCTION_CALL:
            printf("FuncCall: %s\n", node->func_call.name);
            for (int i = 0; i < node->func_call.arg_count; i++) {
                print_ast(node->func_call.args[i], indent + 1);
            }
            break;
        case NODE_BLOCK:
            printf("Block:\n");
            for (int i = 0; i < node->block.stmt_count; i++) {
                print_ast(node->block.stmts[i], indent + 1);
            }
            break;
    }
}