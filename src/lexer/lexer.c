#define _XOPEN_SOURCE 500

#include "lexer.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/lexer_error.h"
#include "lexer/token_recognition.h"

#define PRINT_TOKEN(verbose, tok, action, order)                               \
    if (verbose)                                                               \
    {                                                                          \
        printf("[LEXER] (" order ") " action " token: %s: %s\n",               \
               token_type_to_str(tok.type), tok.value);                        \
    }

struct token parse_input_for_tok(struct lexer *lexer)
{
    enum QUOTING_CONTEXT quoting_context = NONE;

    // Reset current token
    lexer->next_tok.type = TOKEN_NONE;

    // Reset last error
    lexer->last_error = NO_ERROR;

    recognize_token(lexer, &quoting_context);

    categorize_token(&(lexer->next_tok));

    return lexer->next_tok;
}

struct token lexer_peek(struct lexer *lexer)
{
    // If the current token is TOKEN_ERROR, we shall not parse other tokens
    if (lexer->last_error != NO_ERROR)
    {
        lexer->cur_tok.type = TOKEN_ERROR;

        // Free the previous token value if any
        free(lexer->cur_tok.value);
        lexer->cur_tok.value = NULL;

        return lexer->cur_tok;
    }

    int is_verbose = lexer->opts->verbose;

    if (lexer->must_parse_next_tok)
    {
        if (lexer->cur_tok.value != NULL)
        {
            // Free the previous token value if any
            free(lexer->cur_tok.value);
        }

        lexer->must_parse_next_tok = 0;
        // Update the current token with the next token
        lexer->cur_tok.type = lexer->next_tok.type;
        lexer->cur_tok.value = lexer->next_tok.value;
        // Update the next token
        parse_input_for_tok(lexer);

        // If this is the first token, we put the next token in the current and
        // we update again the next token
        if (lexer->cur_tok.type == TOKEN_NONE)
        {
            lexer->cur_tok.type = lexer->next_tok.type;
            lexer->cur_tok.value = lexer->next_tok.value;
            parse_input_for_tok(lexer);
        }

        if (lexer->last_error != NO_ERROR)
        {
            fprintf(stderr, "42sh: %s: %s\n", lexer->cur_tok.value,
                    get_lexer_error_msg(lexer->last_error));
        }

        PRINT_TOKEN(is_verbose, lexer->cur_tok, "Peek", "current");
        return lexer->cur_tok;
    }

    PRINT_TOKEN(is_verbose, lexer->cur_tok, "Peek", "current");

    return lexer->cur_tok;
}

struct token lexer_pop(struct lexer *lexer)
{
    lexer_peek(lexer);

    // Print the token if verbose mode is enabled
    PRINT_TOKEN(lexer->opts->verbose, lexer->cur_tok, "Pop", "current");

    lexer->must_parse_next_tok = 1;
    return lexer->cur_tok;
}

/* Check if the given TOKEN_WORD can describe an assignment word. This is
 * specified in the Rule 7 of the algorithm described in section 2.2.10 of the
 * SCL
 * (https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_10_02)
 */

int is_assignment_word(struct token *token, int is_the_first_word)
{
    // An assignment must derive from a TOKEN_WORD
    if (token->type != TOKEN_WORD)
    {
        return 0;
    }

    // Get the position of the '=' character if any, NULL otherwise
    char *equal_sign = strchr(token->value, '=');

    /* Rule 7.a */
    if (is_the_first_word)
    {
        if (equal_sign == NULL)
        {
            return 0;
        }
    }

    /* Rule 7.b */
    // TODO: check if the '=' is not quoted
    // First paragraph of the Rule 7.b

    // Check if the first character is '=', if so 0 is returned
    if (token->value != NULL && token->value[0] == '=')
    {
        return 0;
    }

    // Check that all the characters before '=' form a valid name
    if (equal_sign != NULL)
    {
        char *name = malloc(sizeof(char) * (equal_sign - token->value + 1));
        strncpy(name, token->value, equal_sign - token->value);
        name[equal_sign - token->value] = '\0';

        if (!is_name(name))
        {
            free(name);
            return 0;
        }

        free(name);
        return 1;
    }

    return 0;
}
