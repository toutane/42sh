#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

#include "io/io.h"
#include "options/opt_parser.h"
#include "utils/hash_map/hash_map.h"
#include "utils/strings/strings.h"
#include "utils/token/token.h"

enum LEXER_ERROR
{
    NO_ERROR,
    UNMATCHED_SINGLE_QUOTE,
    UNMATCHED_DOUBLE_QUOTE,
    UNMATCHED_BRACE,
    BAD_SUBSTITUTION,
};

/**
 * @file lexer.h
 * @brief Structures and functions to create and use a lexer.
 */

/*
 * @brief lexer struct
 * The lexer cuts some input text into block called tokens.

 * This process is done **on demand**: the lexer doesn't read the
 * input more than it needs, and only creates tokens when lexer_peek
 * or lexer_pop is called, and no token is available.
 */

struct lexer
{
    struct options *opts; // The options of the program
    struct stream_info *stream; // The input stream
    struct token cur_tok; // The current token, if processed
    struct token next_tok; // The next token, if processed
    int must_parse_next_tok; // 1 if the next token must be parsed, 0 otherwise
    enum LEXER_ERROR last_error; // The last error that occured
};

/*
 * @brief The different contexts in which a quote can be found.
 */
enum QUOTING_CONTEXT
{
    NONE,
    SINGLE_QUOTE,
    DOUBLE_QUOTE,
};

/**
 * @brief Returns a token from the input stream
 * This function goes through the input string character by character and
 * builds a token.
 *
 * It implements the Token Recognition Algorithm from the SCL standard:
 * https://pubs.opengroup.org/onlinepubs/009604499/utilities/xcu_chap02.html#tag_02_
 *
 * The rule 7 is modified to recognize newlines as tokens.
 * The rule 7.1 is added to recognize semicolons as tokens.
 *
 **/
struct token parse_input_for_tok(struct lexer *lexer);
/**
 * @brief Returns the next token, but doesn't move forward: calling lexer_peek
 * multiple times in a row always returns the same result. This functions is
 * meant to help the parser check if the next token matches some rule.
 */
struct token lexer_peek(struct lexer *lexer);

/**
 * @brief Returns the next token, and removes it from the stream:
 *   calling lexer_pop in a loop will iterate over all tokens until EOF.
 */
struct token lexer_pop(struct lexer *lexer);

int is_assignment_word(struct token *token, int is_the_first_word);

#endif /* ! LEXER_H */
