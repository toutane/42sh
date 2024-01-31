#define _XOPEN_SOURCE 500

#include "ast.h"

void fill_case_item_word(struct ast *ast, char *word)
{
    struct ast_case_item *ast_case_item = (struct ast_case_item *)ast;

    ++ast_case_item->argc;
    ast_case_item->argv =
        realloc(ast_case_item->argv, ast_case_item->argc * sizeof(char *));

    ast_case_item->argv[ast_case_item->argc - 1] = strdup(word);

    return;
}

void fill_case_item_list(struct ast *ast, struct ast *ast_child)
{
    struct ast_case_item *ast_case_item = (struct ast_case_item *)ast;

    ast_case_item->compound_list = ast_child;
    return;
}

void fill_case_case_item(struct ast *ast, struct ast *ast_child)
{
    struct ast_case *ast_case = (struct ast_case *)ast;

    ++ast_case->item_number;
    ast_case->cases_items =
        realloc(ast_case->cases_items,
                ast_case->item_number * sizeof(struct ast_case_item *));
    ast_case->cases_items[ast_case->item_number - 1] = ast_child;

    return;
}
