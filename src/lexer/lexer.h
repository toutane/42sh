#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

#include "../io/io.h"
#include "../options/opt_parser.h"
#include "../utils/strings/strings.h"
#include "../utils/token/token.h"

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
    struct token cur_tok; // The next token, if processed
    int must_parse_next_tok; // 1 if the next token must be parsed, 0 otherwise
};

/**
 * @brief Creates a new lexer given an input string.
 */
struct lexer *lexer_new(struct stream_info *stream, struct options *opts);

/*
 * @brief The different contexts in which a quote can be found.
 */
enum QUOTING_CONTEXT
{
    NONE,
    SINGLE_QUOTE,
    DOUBLE_QUOTE
};

/**
 ** @brief Free the given lexer, but not its input.
 */
void lexer_free(struct lexer *lexer);

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

/*
 * @brief Single quote expansion.
 * If the current token is word token and it contains single quotes,
 * the single quotes are removed. This is a very basic expansion that seems to
 * work for step 1.
 */
void single_quote_expansion(struct lexer *lexer);

#endif /* ! LEXER_H */
