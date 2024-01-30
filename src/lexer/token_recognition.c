#include "lexer/token_recognition.h"

#include <ctype.h>
#include <string.h>

#include "lexer/lexer_error.h"

struct ctx_info
{
    enum QUOTING_CONTEXT *quoting_ctx;
    char cur_char;
    char prev_char;
    size_t token_len;
    int braces_depth;
    int paren_depth;
};

/* Initialize the context information needed by the algorithm. */

static void set_context(struct lexer *lexer, struct ctx_info *ctx)
{
    // Set the current char
    ctx->cur_char = stream_peek(lexer->stream);

    // Set the length of the value of the current token
    ctx->token_len = 0;
    if (lexer->next_tok.type != TOKEN_NONE && lexer->next_tok.value != NULL)
    {
        ctx->token_len = strlen(lexer->next_tok.value);
    }

    // Set the preivous char analyzed if any
    ctx->prev_char = '\0';
    if (lexer->next_tok.type != TOKEN_NONE && ctx->token_len > 0)
    {
        ctx->prev_char = lexer->next_tok.value[ctx->token_len - 1];
    }
}

static void delimit_token(struct lexer *lexer, char delim)
{
    // We discard the current character if it is a <blank> delimiter
    if (isblank(delim))
    {
        stream_pop(lexer->stream);
        return;
    }

    /* If the current token is TOKEN_NONE, we set the current token type to the
     * delimiter. */
    if (lexer->next_tok.type == TOKEN_NONE)
    {
        switch (delim)
        {
        case '\n':
            fill_token(&lexer->next_tok, TOKEN_NEWLINE, NULL);
            break;
        case '(':
            fill_token(&lexer->next_tok, TOKEN_LPAREN, NULL);
            break;
        case ')':
            fill_token(&lexer->next_tok, TOKEN_RPAREN, NULL);
            break;
        case ';':
            fill_token(&lexer->next_tok, TOKEN_SEMICOLON, NULL);
            break;
        case EOF:
            fill_token(&lexer->next_tok, TOKEN_EOF, NULL);
            break;
        default:
            return;
        }
        stream_pop(lexer->stream);
    }
}

static int handle_eof(struct lexer *lexer, struct ctx_info ctx)
{
    /* Token Recognition Algorithm Rule 1
     * If the end of the input stream is encountered, the current token
     * shall be delimited. If an EOF is encountered while looking for
     * matching single quote the lexer returns a TOKEN_ERROR */

    if (ctx.cur_char == EOF)
    {
        delimit_token(lexer, EOF);
        set_lexer_last_error(lexer, ctx.quoting_ctx, ctx.braces_depth,
                             ctx.paren_depth);
        return 1;
    }

    return 0;
}

static int handle_second_operator_char(struct lexer *lexer, struct ctx_info ctx)
{
    enum QUOTING_CONTEXT *quoting_ctx = ctx.quoting_ctx;
    char cur_char = ctx.cur_char;
    char prev_char = ctx.prev_char;
    size_t len = ctx.token_len;

    /* Token Recognition Algorithm Rule 2
     * If the previous character was used as part of an operator and the
     * current character is not quoted and can be used with the previous
     * characters to form an operator, it shall be used as part of that
     * (operator) token. */

    if (lexer->next_tok.type == TOKEN_OPERATOR && !(*quoting_ctx != NONE)
        && len == 1 && can_be_second_in_ope(prev_char, cur_char))
    {
        append_consume(lexer, cur_char);
        return 1;
    }

    /* Token Recognition Algorithm Rule 3 */
    if (lexer->next_tok.type == TOKEN_OPERATOR
        && (!can_be_second_in_ope(prev_char, cur_char) || len == 2))
    {
        return 2;
    }

    return 0;
}

static int handle_quoting(struct lexer *lexer, struct ctx_info ctx)
{
    char cur_char = ctx.cur_char;
    enum QUOTING_CONTEXT *quoting_ctx = ctx.quoting_ctx;

    /* Token Recognition Algorithm Rule 4
     * If the current character is <backslash>, single-quote, or
     * double-quote and it is not quoted, it shall affect quoting for
     * subsequent characters up to the end of the quoted text. */

    switch (cur_char)
    {
    case '\\':
        handle_escape_quote(lexer, quoting_ctx);
        return 1;
    case '\'':
        handle_single_quote(lexer, quoting_ctx);
        return 1;
    case '"':
        handle_double_quote(lexer, quoting_ctx);
        return 1;
    default:
        return 0;
    }
}

static void handle_bad_substitution(struct lexer *lexer, struct ctx_info ctx)
{
    char cur_char = ctx.cur_char;
    int braces_depth = ctx.braces_depth;

    if (braces_depth && !isalnum(cur_char) && cur_char != '_' && cur_char != '}'
        && !is_char_special_variable(cur_char))
    {
        lexer->last_error = BAD_SUBSTITUTION;
    }
}

static int handle_dollar_and_backtick(struct lexer *lexer, struct ctx_info *ctx)
{
    char cur_char = ctx->cur_char;

    /* Token Recognition Algorithm Rule 5
     * If the current character is an unquoted '$', the shell shall identify
     * the start of any candidate for parameter expansion from its
     * introductory unquoted character sequences: '$' or "${", up to the
     * matching '}' character.} */

    if (*(ctx->quoting_ctx) == NONE
        && (cur_char == '$' || cur_char == '}'
            || (cur_char == ')' && ctx->paren_depth > 0)))
    {
        if (lexer->next_tok.type == TOKEN_NONE)
        {
            fill_token(&lexer->next_tok, TOKEN_WORD, NULL);
        }

        if (cur_char == '$')
        {
            stream_pop(lexer->stream);

            if (stream_peek(lexer->stream) == '{')
            {
                ((ctx->braces_depth))++;
                append_char_to_token_value(&lexer->next_tok, '$');
                append_consume(lexer, '{');
            }
            else if (stream_peek(lexer->stream) == '(')
            {
                ((ctx->paren_depth))++;
                append_char_to_token_value(&lexer->next_tok, '$');
                append_consume(lexer, '(');
            }
            else
            {
                append_char_to_token_value(&lexer->next_tok, '$');
            }
        }
        else if (cur_char == '}')
        {
            if (ctx->braces_depth > 0)
            {
                (ctx->braces_depth)--;
            }
            append_consume(lexer, '}');
        }
        else if (cur_char == ')')
        {
            if (ctx->paren_depth > 0)
            {
                (ctx->paren_depth)--;
            }
            append_consume(lexer, ')');
        }

        return 1;
    }

    return 0;
}

static int handle_first_operator_char(struct lexer *lexer, struct ctx_info ctx)
{
    char cur_char = ctx.cur_char;
    enum QUOTING_CONTEXT *quoting_ctx = ctx.quoting_ctx;

    /* Token Recognition Algorithm Rule 6 */

    if (*quoting_ctx == NONE && can_be_first_in_ope(cur_char))
    {
        if (lexer->next_tok.type == TOKEN_NONE)
        {
            set_append_consume(lexer, TOKEN_OPERATOR, cur_char);
            return 1;
        }

        /* If the current token is a word, we check if it is a sequence
         * of digits, if it is the case, we don't
         * consider it as a word anymore and we consider it as an
         * io_number (only if the current char is a '<' or a '>'). */

        if (lexer->next_tok.type == TOKEN_WORD
            && is_str_sequence_of_digits(lexer->next_tok.value)
            && (cur_char == '<' || cur_char == '>'))
        {
            lexer->next_tok.type = TOKEN_IONUMBER;
        }

        return 2;
    }

    return 0;
}

static int handle_delimiter(struct lexer *lexer, struct ctx_info ctx)
{
    char cur_char = ctx.cur_char;
    enum QUOTING_CONTEXT *quoting_ctx = ctx.quoting_ctx;
    int braces_depth = ctx.braces_depth;
    int paren_depth = ctx.paren_depth;

    /* Token Recognition Algorithm Rule 7 (modified)
     * If the current character is an unquoted <blank>, '\n' or ';', any
     * token containing the previous character is delimited and the current
     * character shall be discarded if it is a <blank>, for '\n' and ';',
     * the current token shall be updated to TOKEN_NEWLINE or
     * TOKEN_SEMICOLON*/

    if (*quoting_ctx == NONE && !(braces_depth > 0) && !(paren_depth > 0)
        && is_delimiter(cur_char))
    {
        delimit_token(lexer, cur_char);

        if (isblank(cur_char) && lexer->next_tok.type != TOKEN_WORD)
        {
            return 1;
        }

        // Delimit the current token
        return 2;
    }

    return 0;
}

static void handle_comment(struct lexer *lexer)
{
    struct stream_info *stream = lexer->stream;
    stream_pop(stream);

    while (stream_peek(stream) != '\n' && stream_peek(stream) != EOF)
    {
        stream_pop(stream);
    }
}

/* Updates the current token (inside the lexer struct)
 * according to the input stream.
 * It follows the Token Recognition Algorithm (see the POSIX standard at
 * https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_10)
 * to delimits tokens. */

void recognize_token(struct lexer *lexer, enum QUOTING_CONTEXT *quoting_ctx)
{
    struct ctx_info ctx = { .quoting_ctx = quoting_ctx,
                            .token_len = 0,
                            .braces_depth = 0,
                            .paren_depth = 0 };

    while (1)
    {
        set_context(lexer, &ctx);

        if (handle_eof(lexer, ctx))
            return;

        int status = handle_second_operator_char(lexer, ctx);
        if (status == 1)
            continue;
        if (status == 2)
            return;

        if (handle_quoting(lexer, ctx))
            continue;

        handle_bad_substitution(lexer, ctx);

        if (handle_dollar_and_backtick(lexer, &ctx))
            continue;

        status = handle_first_operator_char(lexer, ctx);
        if (status == 1)
            continue;
        if (status == 2)
            return;

        status = handle_delimiter(lexer, ctx);
        if (status == 1)
            continue;
        if (status == 2)
            return;

        /* Token Recognition Algorithm Rule 8
         * If the previous token is part of a word, the current character is
         * appended to the that word. */

        if (lexer->next_tok.type == TOKEN_WORD)
        {
            append_consume(lexer, ctx.cur_char);
            continue;
        }

        /* Token Recognition Algorithm Rule 9
         * If the current character is a '#', it and all subsequent characters
         * up to, but excluding, the next <newline> shall be discarded as a
         * comment. The <newline> that ends the line is not considered part of
         * the comment. */

        if (ctx.cur_char == '#')
        {
            handle_comment(lexer);
            continue;
        }

        /* Token Recognition Algorithm Rule 10
         * The current character is used as the start of a new word. */
        set_append_consume(lexer, TOKEN_WORD, ctx.cur_char);
    }
}
