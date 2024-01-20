#include "../parser.h"

void push_back(struct ast **list, struct ast *to_push)
{
    struct ast_redirection *tmp = (struct ast_redirection *)*list;
    if (!tmp)
    {
        *list = to_push;
    }
    else
    {
        while (tmp->next)
        {
            tmp = (struct ast_redirection *)tmp->next;
        }
        tmp->next = to_push;
    }
}

/**
 * @brief Parse any prefix, possibly followed by a WORD and by any number
 * of element
 *
 * simple_command = prefix { prefix }
 *                  | { prefix } WORD { element }
 *                  ;
 */
enum parser_status parse_simple_command(struct ast **res, struct lexer *lexer)
{
    // prefix { prefix }
    // | { prefix } WORD { element }

    struct
    {
        struct ast *redirs;
        struct ast *command;
    } locals;
    locals.redirs = NULL;
    locals.command = NULL;

    char prefixed = 0;
    if (parse_prefix(res, lexer) == PARSER_OK)
    {
        // parse { prefix }
        // Add parsed prefix to locals
        push_back(&locals.redirs, *res);
        prefixed = 1;
        while (parse_prefix(res, lexer) == PARSER_OK)
        {
            // Add parsed prefix to locals
            push_back(&locals.redirs, *res);
            continue;
        }
    }
    if (lexer_peek(lexer).type == TOKEN_WORD)
    {
        // Change to locals
        locals.command = calloc(1, sizeof(struct ast_cmd));
        if (!locals.command)
        {
            err(1, "42sh: Allocation failed\n");
        }
        locals.command->type = AST_SIMPLE_COMMAND;

        // Fill node
        fill_sc_node(locals.command, lexer);

        // replace AST
        *res = locals.command;

        // Pop first WORD
        lexer_pop(lexer);

        // { element }
        while (parse_element(res, lexer) == PARSER_OK)
        {
            if (*res != locals.command)
            {
                // Add it to locals
                push_back(&locals.redirs, *res);
                // replace AST
                *res = locals.command;
            }
        }
        // Add locals.commant to the end of locals.redirs
        push_back(&locals.redirs, locals.command);
        // Put is on AST
        *res = locals.redirs;
        return PARSER_OK;
    }
    else
    {
        return (prefixed ? PARSER_OK : PARSER_UNEXPECTED_TOKEN);
    }
    return PARSER_UNEXPECTED_TOKEN;
}
