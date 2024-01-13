#include "ast.h"

void ast_print(struct ast *ast)
{
    if (!ast)
    {
        printf("NULL");
        return;
    }
    if (ast->type == AST_SIMPLE_COMMAND)
    {
        printf(":%s:", ast->argv[0]);
        for (size_t i = 1; i < ast->nb_args; i++)
        {
            printf(" %s", ast->argv[i]);
        }
    }
    else if (ast->type == AST_COMMAND_LIST)
    {
        printf("List(");
        if (ast->nb_child > 0)
        {
            for (size_t i = 0; i < ast->nb_child; i++)
            {
                ast_print(ast->children[i]);
            }
        }
        printf(")\n");
    }
    else if (ast->type == AST_CONDITION)
    {
        printf("If(\n");
        printf("cond: ");
        ast_print(ast->children[0]);
        printf("then: ");
        ast_print(ast->children[1]);
        printf("else: "); // Potentially put condition to print else if not NULL
        ast_print(ast->children[2]);
        printf(")");
    }
}

void ast_free(struct ast *ast)
{
    if (!ast)
    {
        return;
    }
    if (ast->type == AST_SIMPLE_COMMAND)
    {
        if (ast->nb_args > 0)
        {
            for (size_t i = 0; i < ast->nb_args; i++)
            {
                free(ast->argv[i]);
            }
            free(ast->argv);
            ast->argv = NULL;
        }
    }
    else if (ast->type == AST_COMMAND_LIST)
    {
        if (ast->nb_child > 0)
        {
            for (size_t i = 0; i < ast->nb_child; i++)
            {
                ast_free(ast->children[i]);
            }
            free(ast->children);
            ast->children = NULL;
        }
    }
    else if (ast->type == AST_CONDITION)
    {
        ast_free(ast->children[0]);
        ast_free(ast->children[1]);
        ast_free(ast->children[2]);
        free(ast->children);
        ast->children = NULL;
    }
    free(ast);
}

int eval_sc_node(struct ast *ast)
{
    if (is_builtin_word(ast->argv[0]))
    {
        return (builtin_fun(ast->argv[0]))(ast->nb_args - 1, ast->argv);
    }
    int status = 0;
    int pid = fork();
    if (pid == 0)
    {
        execvp(ast->argv[0], ast->argv);
        fprintf(stderr, "42sh: failed exec\n");
        return 127;
    }
    else
    {
        waitpid(pid, &status, 0);
    }

    return WEXITSTATUS(status);
}

int ast_eval(struct ast *ast)
{
    if (!ast)
    {
        return 0;
    }
    int status = 0;
    if (ast->type == AST_SIMPLE_COMMAND)
    {
        status = eval_sc_node(ast);
    }
    else if (ast->type == AST_COMMAND_LIST)
    {
        for (size_t i = 0; i < ast->nb_child; i++)
        {
            status = ast_eval(ast->children[i]);
        }
    }
    else if (ast->type == AST_CONDITION)
    {
        if (ast_eval(ast->children[0]) == EXIT_SUCCESS)
        {
            status = ast_eval(ast->children[1]);
        }
        else if (ast->children[2] != NULL)
        {
            status = ast_eval(ast->children[2]);
        }
    }
    return status;
}

static void ast_pretty_print_aux(struct ast *ast)
{
    if (ast == NULL)
    {
        fprintf(stderr, "ast_pretty_print: ast is NULL\n");
        return;
    }

    enum ast_type type = ast->type;
    switch (type)
    {
    case AST_SIMPLE_COMMAND:
        printf("command");
        printf(" \"%s\"", ast->argv[0]); // Print command value
        for (size_t i = 1; i < ast->nb_args - 1; i++)
        {
            printf(" \"%s\"", ast->argv[i]); // Print arguments values
        }
        break;
    case AST_COMMAND_LIST:
        if (ast->nb_child > 0)
        {
            ast_pretty_print_aux(ast->children[0]);
            for (size_t i = 1; i < ast->nb_child; i++)
            {
                printf("; ");
                ast_pretty_print_aux(ast->children[i]);
            }
        }
        break;
    case AST_CONDITION:
        if (ast->nb_child > 0)
        {
            printf("if { ");
            ast_pretty_print_aux(ast->children[0]);
            printf(" }; then { ");
            ast_pretty_print_aux(ast->children[1]);
            printf(" }");
            if (ast->nb_child > 2)
            {
                printf("; else { ");
                ast_pretty_print_aux(ast->children[2]);
                printf(" }");
            }
        }
        break;
    default:
        fprintf(stderr, "ast_pretty_print: ast type is invalid\n");
        break;
    }
}

void ast_pretty_print(struct ast *ast)
{
    if (ast == NULL)
    {
        return;
    }

    printf("AST pretty print========================\n\n");
    ast_pretty_print_aux(ast);
    printf("\n\n========================================\n");
}
