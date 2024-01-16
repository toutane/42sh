#include "printers.h"

#include <stdio.h>

#define PRINT_OPTIONS(str)                                                     \
    printf("  " #str ": %s\n", opts->str ? "active" : "inactive")

void options_print(struct options *opts)
{
    printf("Options:\n");

    PRINT_OPTIONS(ast_dot);
    PRINT_OPTIONS(command);
    PRINT_OPTIONS(pretty_print);
    PRINT_OPTIONS(verbose);
}

void input_mode_print();

/*
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
*/

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

    printf("AST (pretty print):\n  ");
    ast_pretty_print_aux(ast);
    printf("\n");
}
