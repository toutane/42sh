#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"

char tab[] = { [TOKEN_SEMICOLON] = ';', [TOKEN_NEWLINE] = 'N' };

int main(void)
{
    // Read input from stdin
    char *input = NULL;
    size_t len = 0;
    getline(&input, &len, stdin);

    struct lexer *lexer = lexer_new(input);
    struct ast *ast = NULL;
    parse(&ast, lexer);
    ast_print(ast);
    ast_eval(ast);

    /*
    struct token token = lexer_pop(lexer);

    while (token.type != TOKEN_EOF)
    {
        if (token.type == TOKEN_WORD)
            printf("%s\n", token.value);
        else
            printf("%c\n", tab[token.type]);
        token = lexer_pop(lexer);
    }

    if (token.type == TOKEN_EOF)
        printf("EOF\n");

    */

    ast_free(ast);
    lexer_free(lexer);
    free(input);

    return EXIT_SUCCESS;
}
