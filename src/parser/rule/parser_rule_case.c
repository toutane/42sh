#define _XOPEN_SOURCE 500

#include "parser.h"

static struct hm *cases;

static enum parser_status parse_case_clause(struct ast **res,
                                            struct lexer *lexer)
{
    cases = hm_new(HM_FUNCTION, 50, ast_free);

    if (parse_case_item(res, lexer) == PARSER_OK)
    {
        // TODO: create ;; TOKEN
        while (lexer_peek())
    }
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

                if (parse_case_clause(res, lexer) == PARSER_OK)
                {
                    // TODO: fill_case_node
                }

                if (lexer_peek(lexer).type == TOKEN_ESAC)
                {
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
