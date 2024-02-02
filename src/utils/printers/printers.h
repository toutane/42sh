#ifndef PRINTERS_H
#define PRINTERS_H

#include "ast/ast.h"
#include "options/opt_parser.h"
#include "utils/hash_map/hash_map.h"

/*
 * @brief Prints set options (called when verbose mode is enabled)
 */
void options_print(struct options *opts);

/*
 * @brief Prints the input mode (called when verbose mode is enabled)
 */
void input_mode_print(void);

/*
 * @brief Prints the AST (not called, has been replaced by ast_pretty_print)
 */
void ast_print(struct ast *ast);

/*
 * @brief Prints the AST in a pretty way (called when verbose mode is enabled)
 */
void ast_pretty_print(struct ast *ast);

void hm_print(struct hm *hm);

#endif /* ! PRINTERS_H */
