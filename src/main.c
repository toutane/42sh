#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "io_backend/io_backend.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"

char tab[] = { [TOKEN_SEMICOLON] = ';', [TOKEN_NEWLINE] = 'N' };

int main(void)
{
    struct stream_info *stream = stream_new(NULL, "echo toto; echo tata");

    struct lexer *lexer = lexer_new(stream);
    struct ast *ast = NULL;

    parse(&ast, lexer);

    ast_print(ast);
    ast_eval(ast);

    ast_free(ast);
    lexer_free(lexer);
    stream_free(stream);

    return EXIT_SUCCESS;
}
