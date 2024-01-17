#include "parser.h"

enum parser_status parse_pipeline(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse one or more pipelines separated by operators && or ||
 *
 * and_or =         pipeline { ( '&&' | '||' ) {'\n'} pipeline } ;
 */
enum parser_status parse_and_or(struct ast **res, struct lexer *lexer)
{
    // | pipeline
    if (parse_pipeline(res, lexer) == PARSER_OK)
    {
        // { ( '&&' | '||' ) {'\n'} pipeline }
        while (lexer_peek(lexer).type == TOKEN_AND
               || lexer_peek(lexer).type == TOKEN_OR)
        {
            // Create AND/OR node
            lexer_pop(lexer);

            while (lexer_peek(lexer).type == TOKEN_NEWLINE)
            {
                lexer_pop(lexer);
            };

            if (parse_pipeline(res, lexer) == PARSER_OK)
            {
                continue;
            }
            // Free node(s)
            return PARSER_UNEXPECTED_TOKEN;
        }
        return PARSER_OK;
    }
    // Free node(s)
    return PARSER_UNEXPECTED_TOKEN;
}
