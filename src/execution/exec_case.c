#define _XOPEN_SOURCE 500

#include <fnmatch.h>

#include "execution/exec.h"

static int is_in_argv(int argc, char **argv, char *word, struct mem *mem)
{
    char *expanded_word = expand_string(&(word), mem);

    char *temp;
    for (int i = 0; i < argc; ++i)
    {
        temp = expand_string(&(argv[i]), mem);

        if (fnmatch(temp, expanded_word, 0) == 0)
        {
            free(expanded_word);
            free(temp);
            return 1;
        }

        free(temp);
    }

    free(expanded_word);
    return 0;
}

int eval_case(struct ast *ast, struct mem *mem)
{
    struct ast_case *ast_case = (struct ast_case *)ast;

    // check if the word match a case_item
    struct ast_case_item *temp;
    for (int i = 0; i < ast_case->item_number; ++i)
    {
        temp = ((struct ast_case_item *)ast_case->cases_items[i]);
        if (is_in_argv(temp->argc, temp->argv, ast_case->base_name, mem))
        {
            return eval_ast(temp->compound_list, mem);
        }
    }

    return 0;
}
