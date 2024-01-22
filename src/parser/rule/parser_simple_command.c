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

static void init_locals(struct ast **redirs, struct ast **command)
{
    *redirs = NULL;
    // Change to locals - we need to allocate here to be able to put all
    // assignements words into sc node
    *command = calloc(1, sizeof(struct ast_cmd));
    if (!*command)
    {
        err(1, "42sh: Allocation failed\n");
    }
    (*command)->type = AST_SIMPLE_COMMAND;
}

static struct ast *build_locals(struct ast **redirs, struct ast **command)
{
    if (((struct ast_cmd *)*command)->argc > 0)
    {
        push_back(redirs, *command);
    }
    else
    {
        ast_free(*command);
    }
    return *redirs;
}

static void fill_locals(struct ast **redirs, struct ast **command,
                        struct ast **res)
{
    if (*res != *command)
    {
        // Add parsed prefix to locals
        push_back(redirs, *res);
        *res = *command;
    }
}

static void free_locals(struct ast **redirs, struct ast **command)
{
    ast_free(*command);
    ast_free(*redirs);
    *command = NULL;
    *redirs = NULL;
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
    init_locals(&locals.redirs, &locals.command);

    *res = locals.command;

    char prefixed = 0;
    if (parse_prefix(res, lexer) == PARSER_OK)
    {
        // parse { prefix }
        prefixed = 1;

        // fill_locals
        fill_locals(&locals.redirs, &locals.command, res);

        while (parse_prefix(res, lexer) == PARSER_OK)
        {
            // fill_locals
            fill_locals(&locals.redirs, &locals.command, res);
        }
    }
    if (lexer_peek(lexer).type == TOKEN_WORD)
    {
        // Fill node and Pop
        fill_sc_node(locals.command, lexer);
        lexer_pop(lexer);

        // replace AST
        *res = locals.command;

        // { element }
        while (parse_element(res, lexer) == PARSER_OK)
        {
            // fill_locals
            fill_locals(&locals.redirs, &locals.command, res);
        }

        // build_locals before returning
        *res = build_locals(&locals.redirs, &locals.command);
        return PARSER_OK;
    }
    else
    {
        if (prefixed)
        {
            // build_locals before returning
            *res = build_locals(&locals.redirs, &locals.command);
            return PARSER_OK;
        }
    }
    free_locals(&locals.redirs, &locals.command);
    *res = NULL;
    return PARSER_UNEXPECTED_TOKEN;
}
