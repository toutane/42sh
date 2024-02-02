#define _XOPEN_SOURCE 500

#include "parser.h"

static enum parser_status parse_case_item(struct ast **res, struct lexer *lexer)
{
    if (lexer_peek(lexer).type == TOKEN_LPAREN)
    {
        lexer_pop(lexer);
    }

    if (lexer_peek(lexer).type == TOKEN_WORD)
    {
        struct ast *case_item_node = calloc(1, sizeof(struct ast_case_item));
        case_item_node->type = AST_CASE_ITEM;

        fill_case_item_word(case_item_node, lexer->cur_tok.value);
        lexer_pop(lexer);

        while (lexer_peek(lexer).type == TOKEN_PIPE)
        {
            lexer_pop(lexer);

            if (lexer_peek(lexer).type == TOKEN_WORD)
            {
                fill_case_item_word(case_item_node, lexer->cur_tok.value);
                lexer_pop(lexer);
            }
            else
            {
                return PARSER_UNEXPECTED_TOKEN;
            }
        }

        if (lexer_peek(lexer).type == TOKEN_RPAREN)
        {
            lexer_pop(lexer);

            while (lexer_peek(lexer).type == TOKEN_NEWLINE)
            {
                lexer_pop(lexer);
            }

            if (parse_compound_list(res, lexer) == PARSER_OK)
            {
                fill_case_item_list(case_item_node, *res);
            }

            *res = case_item_node;
            return PARSER_OK;
        }

        *res = NULL;
        ast_free(case_item_node);
    }

    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status
parse_case_clause(struct ast **res, struct lexer *lexer, struct ast *ast_case)
{
    if (parse_case_item(res, lexer) == PARSER_OK)
    {
        fill_case_case_item(ast_case, *res);

        while (lexer_peek(lexer).type == TOKEN_DSEMI)
        {
            lexer_pop(lexer);

            while (lexer_peek(lexer).type == TOKEN_NEWLINE)
            {
                lexer_pop(lexer);
            }

            if (parse_case_item(res, lexer) == PARSER_OK)
            {
                fill_case_case_item(ast_case, *res);
            }
            else
            {
                return PARSER_OK;
            }
        }
    }

    return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status parse_rule_case(struct ast **res, struct lexer *lexer)
{
    if (lexer_peek(lexer).type == TOKEN_CASE)
    {
        struct ast *case_node = calloc(1, sizeof(struct ast_case));
        case_node->type = AST_CASE;

        lexer_pop(lexer);

        if (lexer_peek(lexer).type == TOKEN_WORD)
        {
            ((struct ast_case *)case_node)->base_name =
                strdup(lexer->cur_tok.value);

            lexer_pop(lexer);

            while (lexer_peek(lexer).type == TOKEN_NEWLINE)
            {
                lexer_pop(lexer);
            }

            if (lexer_peek(lexer).type == TOKEN_IN)
            {
                lexer_pop(lexer);

                while (lexer_peek(lexer).type == TOKEN_NEWLINE)
                {
                    lexer_pop(lexer);
                }

                parse_case_clause(res, lexer, case_node);

                if (lexer_peek(lexer).type == TOKEN_ESAC)
                {
                    lexer_pop(lexer);

                    *res = case_node;
                    return PARSER_OK;
                }
            }
        }

        *res = NULL;
        ast_free(case_node);
    }
    return PARSER_UNEXPECTED_TOKEN;
}
