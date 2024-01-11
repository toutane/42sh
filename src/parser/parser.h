#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <string.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"

enum parser_status
{
    PARSER_OK,
    PARSER_UNEXPECTED_TOKEN,
};

/**
 * @brief Parse either a list, or newline
 *
 * input =     list '\n'
 *          |  list EOF
 *          | '\n'
 *          | EOF
 *          ;
 */
enum parser_status parse(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a and_or
 *
 * list =           and_or ;
 */
enum parser_status parse_list(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a pipeline
 *
 * and_or =         pipeline ;
 */
enum parser_status parse_and_or(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a command
 *
 * pipeline =       command ;
 */
enum parser_status parse_pipeline(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a simple_command
 *
 * command =        simple_command ;
 */
enum parser_status parse_command(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a WORD, followed by any number of element
 *
 * simple_command = WORD { element } ;
 */
enum parser_status parse_simple_command(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a WORD
 *
 * element =        WORD ;
 */
enum parser_status parse_element(struct ast **res, struct lexer *lexer);

#endif /* !PARSER_H */
