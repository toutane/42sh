#include "exec.h"

// eval or node
int eval_and(struct ast *ast, struct hash_map *gv_hash_map)
{
    struct ast_and_or *ast_and_or = (struct ast_and_or *)ast;

    return (!eval_ast(ast_and_or->left, gv_hash_map))
        && (!eval_ast(ast_and_or->right, gv_hash_map));
}

// eval or node
int eval_or(struct ast *ast, struct hash_map *gv_hash_map)
{
    struct ast_and_or *ast_and_or = (struct ast_and_or *)ast;

    return (!eval_ast(ast_and_or->left, gv_hash_map))
        || (!eval_ast(ast_and_or->right, gv_hash_map));
}
