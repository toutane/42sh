#include "parser.h"

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
        //Checked popped
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
        // TODO: CREATE NODE with cur_token
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
        // TODO: Append cur_tok to AST
        lexer_pop(lexer);
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
