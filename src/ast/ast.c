#include "ast.h"

void free_ast_cmd(struct ast *ast)
{
    struct ast_cmd *ast_cmd = (struct ast_cmd *)ast;
    for (int i = 0; i < ast_cmd->argc; ++i)
    {
        if (ast_cmd->argv[i])
            free(ast_cmd->argv[i]);
    }
    free(ast_cmd->argv);

    for (int i = 0; i < ast_cmd->prefix_count; ++i)
    {
        if (ast_cmd->prefix[i])
            free(ast_cmd->prefix[i]);
    }
    free(ast_cmd->prefix);
    free(ast);
    return;
}

void free_ast_cmd_list(struct ast *ast)
{
    struct ast_cmd_list *ast_cmd_list = (struct ast_cmd_list *)ast;
    ast_free(ast_cmd_list->cmd);
    ast_free(ast_cmd_list->next);
    free(ast);
    return;
}

void free_ast_if(struct ast *ast)
{
    struct ast_condition *ast_if = (struct ast_condition *)ast;
    ast_free(ast_if->condition);
    ast_free(ast_if->then_body);
    ast_free(ast_if->else_body);
    free(ast);
    return;
}

void free_ast_pipeline(struct ast *ast)
{
    struct ast_pipeline *ast_pipeline = (struct ast_pipeline *)ast;
    ast_free(ast_pipeline->left);
    ast_free(ast_pipeline->right);
    free(ast);
    return;
}

void free_ast_redir(struct ast *ast)
{
    struct ast_redirection *ast_redirection = (struct ast_redirection *)ast;
    if (ast_redirection->target)
        free(ast_redirection->target);
    if (ast_redirection->next)
        ast_free(ast_redirection->next);
    free(ast);
    return;
}

void free_ast_neg(struct ast *ast)
{
    struct ast_neg *ast_neg = (struct ast_neg *)ast;
    if (ast_neg->data)
        ast_free(ast_neg->data);
    free(ast);
    return;
}

void free_ast_for(struct ast *ast)
{
    struct ast_for *ast_for = (struct ast_for *)ast;
    if (ast_for->condition)
        free(ast_for->condition);
    if (ast_for->data)
        ast_free(ast_for->data);

    for (size_t i = 0; i < ast_for->array_size; ++i)
    {
        free(ast_for->array[i]);
    }
    if (ast_for->array)
    {
        free(ast_for->array);
    }
    free(ast);
    return;
}

typedef void (*free_type)(struct ast *ast);
void ast_free(struct ast *ast)
{
    if (!ast)
    {
        return;
    }

    static const free_type functions[] = {
        [AST_SIMPLE_COMMAND] = &free_ast_cmd,
        [AST_COMMAND_LIST] = &free_ast_cmd_list,
        [AST_CONDITION] = &free_ast_if,
        [AST_REDIRECTION] = &free_ast_redir,
        [AST_PIPELINE] = &free_ast_pipeline,
        [AST_FOR] = &free_ast_for,
        [AST_NEG] = &free_ast_neg,
    };
    (*functions[ast->type])(ast);
}
