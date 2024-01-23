#include "../parser.h"

static void init_locals(struct ast **redirs, struct ast **command,
                        struct ast **res)
{
    *redirs = NULL;
    *command = *res;
}

/**
 * @brief Parse a simple_command
 *
 * command =        simple_command
                  | shell_command
                  ;
 */
enum parser_status parse_command(struct ast **res, struct lexer *lexer)
{
    // simple_command
    // | shell_command { redirecton }
    if (parse_simple_command(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    else if (parse_shell_command(res, lexer) == PARSER_OK)
    {
        struct
        {
            struct ast *redirs;
            struct ast *command;
        } locals;
        init_locals(&locals.redirs, &locals.command, res);

        while (parse_redirection(res, lexer) == PARSER_OK)
        {
            // Add it to locals
            // fill_locals not necessary here (1line)
            push_back(&locals.redirs, *res);
        }
        // build_locals not necessary here (1line)
        push_back(&locals.redirs, locals.command);

        *res = locals.redirs;
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
