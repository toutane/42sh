#ifndef LEXER_SETUP_H
#define LEXER_SETUP_H

#include "../io/io.h"
#include "../lexer/lexer.h"
#include "../options/opt_parser.h"

/**
 * @brief Creates a new lexer given an input string.
 */
struct lexer *lexer_new(struct stream_info *stream, struct options *opts,
                        struct hm *hm_alias);

/**
 ** @brief Free the given lexer, but not its input.
 */
void lexer_free(struct lexer *lexer);

#endif /* ! LEXER_SETUP_H */
