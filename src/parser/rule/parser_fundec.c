#include "ast/ast.h"
#include "parser.h"

enum parser_status parse_fundec(struct ast **res, struct lexer *lexer)
{
    if (lexer_peek(lexer).type == TOKEN_WORD)
    {
        struct ast *func_node = calloc(1, sizeof(struct ast_func));
        func_node->type = AST_FUNC;

        fill_word_func(func_node, lexer->cur_tok.value);
        lexer_pop(lexer);

        // replace WORD by a RPAR
        if (lexer_peek(lexer).type == TOKEN_LPAREN)
        {
            lexer_pop(lexer);

            if (lexer_peek(lexer).type == TOKEN_RPAREN)
            {
                lexer_pop(lexer);

                while (lexer_peek(lexer).type == TOKEN_NEWLINE)
                {
                    lexer_pop(lexer);
                }

                if (parse_shell_command(res, lexer) == PARSER_OK)
                {
                    fill_func_node(func_node, *res);

                    *res = func_node;
                    return PARSER_OK;
                }
            }
        }

        ast_free(func_node);
        *res = NULL;
    }

    return PARSER_UNEXPECTED_TOKEN;
}
