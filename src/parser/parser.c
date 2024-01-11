#define _XOPEN_SOURCE 500

#include "parser.h"

void fill_sc_node(struct ast *sc_node, struct lexer *lexer)
{
    if (sc_node->nb_args == 0)
    {
        // Init simple-command node
        sc_node->type = AST_SIMPLE_COMMAND;
        sc_node->nb_args = 2;
        sc_node->argv = malloc(sc_node->nb_args * sizeof(char *));
        // TODO: Check for NULL after allocation try
        sc_node->argv[0] = strdup(lexer->cur_tok.value);
        sc_node->argv[sc_node->nb_args - 1] = NULL;
    }
    else
    {
        // Append element to simple-command node
        sc_node->nb_args += 1;
        sc_node->argv =
            realloc(sc_node->argv, sc_node->nb_args * sizeof(char *));
        // TODO: Check for NULL after allocation try
        sc_node->argv[sc_node->nb_args - 2] = strdup(lexer->cur_tok.value);
        sc_node->argv[sc_node->nb_args - 1] = NULL;
    }
}

void fill_list_node(struct ast *list_node, struct ast *sc_node)
{
    list_node->nb_child += 1;
    list_node->children = realloc(list_node->children,
                                  sizeof(struct ast *) * list_node->nb_child);
    // TODO: Check for NULL after allocation try
    list_node->children[list_node->nb_child - 1] = sc_node;
}

enum parser_status parse(struct ast **res, struct lexer *lexer)
{
    lexer_peek(lexer);
    // | '\n'
    // | EOF
    if (lexer->cur_tok.type == TOKEN_NEWLINE
        || lexer->cur_tok.type == TOKEN_EOF)
    {
        return PARSER_OK;
    }
    // | list '\n'
    // | list EOF
    if (parse_list(res, lexer) == PARSER_OK)
    {
        // Checked popped
        lexer_peek(lexer);
        if (lexer->cur_tok.type == TOKEN_NEWLINE
            || lexer->cur_tok.type == TOKEN_EOF)
        {
            return PARSER_OK;
        }
    }
    return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status parse_list(struct ast **res, struct lexer *lexer)
{
    // | and_or
    if (parse_and_or(res, lexer) == PARSER_OK)
    {
        // Create list node for AST
        struct ast *list_node = calloc(1, sizeof(struct ast));
        list_node->type = AST_COMMAND_LIST;

        // Add previously parsed `and_or` to AST
        fill_list_node(list_node, *res);

        // { ';' and_or } [ ';' ] ;
        while (lexer_peek(lexer).type == TOKEN_SEMICOLON)
        {
            // Consume ';' token
            lexer_pop(lexer);

            if (parse_and_or(res, lexer) == PARSER_OK)
            {
                // { ';' and_or } case
                // Add previously parsed `and_or` to AST
                fill_list_node(list_node, *res);
            }
            else
            {
                // [ ';' ] case
                break;
            }
        }
        // Replace AST by list node
        *res = list_node;

        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status parse_and_or(struct ast **res, struct lexer *lexer)
{
    // | pipeline
    if (parse_pipeline(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status parse_pipeline(struct ast **res, struct lexer *lexer)
{
    // | command
    if (parse_command(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status parse_command(struct ast **res, struct lexer *lexer)
{
    // | simple-command
    if (parse_simple_command(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status parse_simple_command(struct ast **res, struct lexer *lexer)
{
    // | WORD { element }
    lexer_peek(lexer);
    if (lexer->cur_tok.type == TOKEN_WORD)
    {
        struct ast *sc_node = calloc(1, sizeof(struct ast));
        // TODO: Check for NULL after allocation try

        // Fill node
        fill_sc_node(sc_node, lexer);
        // replace AST
        *res = sc_node;

        // Pop first WORD
        lexer_pop(lexer);

        // { element }
        while (parse_element(res, lexer) == PARSER_OK)
        {
            continue;
        }
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status parse_element(struct ast **res, struct lexer *lexer)
{
    // | WORD
    lexer_peek(lexer);
    if (lexer->cur_tok.type == TOKEN_WORD)
    {
        // Append cur_tok to AST simple-command node
        fill_sc_node(*res, lexer);

        // Pop element
        lexer_pop(lexer);

        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
