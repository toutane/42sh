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

    // Change to locals - we need to allocate here to be able to put all
    // assignements words into sc node
    locals.command = calloc(1, sizeof(struct ast_cmd));
    if (!locals.command)
    {
        err(1, "42sh: Allocation failed\n");
    }
    locals.command->type = AST_SIMPLE_COMMAND;

    char prefixed = 0;
    if (parse_prefix(res, lexer) == PARSER_OK)
    {
        // parse { prefix }
        if (*res == NULL)
        {
            fill_sc_node(locals.command, lexer);
            lexer_pop(lexer);
        }
        else
        {
            // Add parsed prefix to locals
            push_back(&locals.redirs, *res);
        }
        prefixed = 1;

        while (parse_prefix(res, lexer) == PARSER_OK)
        {
            // *res is NULL if the last token is an ASSIGNMENT_WORD
            if (*res == NULL)
            {
                fill_sc_node(locals.command, lexer);
                lexer_pop(lexer);
            }
            else
            {
                // Add parsed prefix to locals
                push_back(&locals.redirs, *res);
                continue;
            }
        }
    }
    if (lexer_peek(lexer).type == TOKEN_WORD)
    {
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
        if (prefixed)
        {
            push_back(&locals.redirs, locals.command);
            *res = locals.redirs;
            return PARSER_OK;
        }
    }
    ast_free(locals.command);
    *res = NULL;
    return PARSER_UNEXPECTED_TOKEN;
}
