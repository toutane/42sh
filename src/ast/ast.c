#include "ast.h"

void free_ast_cmd(struct ast *ast)
{
    struct ast_cmd *ast_cmd = (struct ast_cmd *)ast;
    for (int i = 0; i < ast_cmd->argc; ++i)
    {
        free(ast_cmd->argv[i]);
    }
    free(ast_cmd->argv);
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
    if (ast_redirection->data)
        free(ast_redirection->data);
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
    };
    (*functions[ast->type])(ast);
}
