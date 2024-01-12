#define _XOPEN_SOURCE 500

#include "parser.h"

/**
 * @brief Parse a and_or
 *
 * list =           and_or { ';' and_or } [ ';' ] ;
 */
static enum parser_status parse_list(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a pipeline
 *
 * and_or =         pipeline ;
 */
static enum parser_status parse_and_or(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a command
 *
 * pipeline =       command ;
 */
static enum parser_status parse_pipeline(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a simple_command
 *
 * command =        simple_command
                  | shell_command
                  ;
 */
static enum parser_status parse_command(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a shell_command
 *
 * shell_command =  rule_if ;
 */
static enum parser_status parse_shell_command(struct ast **res,
                                              struct lexer *lexer);

/**
 * @brief Parse a rule_if
 *
 * rule_if =        'if' compound_list 'then' compound_list [else_clause] 'fi' ;
 */
static enum parser_status parse_rule_if(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a else_clause
 *
 * else_clause =    'else' compound_list
 *                | 'elif' compound_list 'then' compound_list [else_clause]
 *                ;
 */
static enum parser_status parse_else_clause(struct ast **res,
                                            struct lexer *lexer);

/**
 * @brief Parse a else_clause
 *
 * compound_list =  and_or [';'] {'\n'} ;
 */
static enum parser_status parse_compound_list(struct ast **res,
                                              struct lexer *lexer);

/**
 * @brief Parse a WORD, followed by any number of element
 *
 * simple_command = WORD { element } ;
 */
static enum parser_status parse_simple_command(struct ast **res,
                                               struct lexer *lexer);

/**
 * @brief Parse a WORD
 *
 * element =        WORD ;
 */
static enum parser_status parse_element(struct ast **res, struct lexer *lexer);

//////////////
//// CODE ////
//////////////

static void fill_sc_node(struct ast *sc_node, struct lexer *lexer)
{
    if (sc_node->nb_args == 0)
    {
        // Init simple-command node
        sc_node->type = AST_SIMPLE_COMMAND;
        sc_node->nb_args = 2;
        sc_node->argv = malloc(sc_node->nb_args * sizeof(char *));
        // TODO: Check for NULL after allocation try
        sc_node->argv[0] = strdup(lexer->cur_tok.value);
        sc_node->argv[sc_node->nb_args - 1] = NULL;
    }
    else
    {
        // Append element to simple-command node
        sc_node->nb_args += 1;
        sc_node->argv =
            realloc(sc_node->argv, sc_node->nb_args * sizeof(char *));
        // TODO: Check for NULL after allocation try
        sc_node->argv[sc_node->nb_args - 2] = strdup(lexer->cur_tok.value);
        sc_node->argv[sc_node->nb_args - 1] = NULL;
    }
}

static void fill_list_node(struct ast *list_node, struct ast *sc_node)
{
    list_node->nb_child += 1;
    list_node->children = realloc(list_node->children,
                                  sizeof(struct ast *) * list_node->nb_child);
    // TODO: Check for NULL after allocation try
    list_node->children[list_node->nb_child - 1] = sc_node;
}

static void fill_if_node(struct ast *if_node, struct ast *node)
{
    if_node->nb_child += 1;
    if_node->children =
        realloc(if_node->children, sizeof(struct ast *) * if_node->nb_child);
    // TODO: Check for NULL after allocation try
    if_node->children[if_node->nb_child - 1] = node;
}

enum parser_status parse(struct ast **res, struct lexer *lexer)
{
    lexer_peek(lexer);
    // | '\n'
    // | EOF
    if (lexer->cur_tok.type == TOKEN_NEWLINE
        || lexer->cur_tok.type == TOKEN_EOF)
    {
        return PARSER_OK;
    }
    // | list '\n'
    // | list EOF
    if (parse_list(res, lexer) == PARSER_OK)
    {
        // Checked popped
        lexer_peek(lexer);
        if (lexer->cur_tok.type == TOKEN_NEWLINE
            || lexer->cur_tok.type == TOKEN_EOF)
        {
            return PARSER_OK;
        }
    }
    fprintf(stderr, "Syntax Error !\n");
    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_list(struct ast **res, struct lexer *lexer)
{
    // | and_or
    if (parse_and_or(res, lexer) == PARSER_OK)
    {
        // Create list node for AST
        struct ast *list_node = calloc(1, sizeof(struct ast));
        list_node->type = AST_COMMAND_LIST;

        // Add previously parsed `and_or` to AST
        fill_list_node(list_node, *res);

        // { ';' and_or } [ ';' ] ;
        while (lexer_peek(lexer).type == TOKEN_SEMICOLON)
        {
            // Consume ';' token
            lexer_pop(lexer);

            if (parse_and_or(res, lexer) == PARSER_OK)
            {
                // { ';' and_or } case
                // Add previously parsed `and_or` to AST
                fill_list_node(list_node, *res);
            }
            else
            {
                // [ ';' ] case
                break;
            }
        }
        // Replace AST by list node
        *res = list_node;

        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_and_or(struct ast **res, struct lexer *lexer)
{
    // | pipeline
    if (parse_pipeline(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_pipeline(struct ast **res, struct lexer *lexer)
{
    // | command
    if (parse_command(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_command(struct ast **res, struct lexer *lexer)
{
    // simple_command
    // | shell_command
    if (parse_simple_command(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    else if (parse_shell_command(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_shell_command(struct ast **res,
                                              struct lexer *lexer)
{
    // rule_if
    if (parse_rule_if(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_rule_if(struct ast **res, struct lexer *lexer)
{
    // 'if' compound_list 'then' compound_list [else_clause] 'fi'
    if (lexer_peek(lexer).type == TOKEN_IF)
    {
        // Create IF node
        struct ast *if_node = calloc(1, sizeof(struct ast));
        if_node->type = AST_CONDITION;

        // Pop 'if'
        lexer_pop(lexer);

        if (parse_compound_list(res, lexer) == PARSER_OK
            && (lexer_peek(lexer).type == TOKEN_THEN))
        {
            fill_if_node(if_node, *res);

            // Pop 'then'
            lexer_pop(lexer);

            if (parse_compound_list(res, lexer) == PARSER_OK)
            {
                fill_if_node(if_node, *res);

                if (parse_else_clause(res, lexer) == PARSER_OK)
                {
                    fill_if_node(if_node, *res);
                }
                if (lexer_peek(lexer).type == TOKEN_FI)
                {
                    // Pop 'fi'
                    lexer_pop(lexer);

                    *res = if_node;
                    return PARSER_OK;
                }
            }
        }
        // ast_free(if_node);
        *res = if_node;
    }
    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_else_clause(struct ast **res,
                                            struct lexer *lexer)
{
    // 'else' compound_list
    // | 'elif' compound_list 'then' compound_list [else_clause]
    lexer_peek(lexer);
    if (lexer->cur_tok.type == TOKEN_ELSE)
    {
        // Pop 'else'
        lexer_pop(lexer);

        if (parse_compound_list(res, lexer) == PARSER_OK)
        {
            return PARSER_OK;
        }
    }
    else if (lexer->cur_tok.type == TOKEN_ELIF)
    {
        // Create IF node
        struct ast *elif_node = calloc(1, sizeof(struct ast));
        elif_node->type = AST_CONDITION;

        // Pop 'elif'
        lexer_pop(lexer);

        if (parse_compound_list(res, lexer) == PARSER_OK
            && lexer_peek(lexer).type == TOKEN_THEN)
        {
            fill_if_node(elif_node, *res);

            // Pop 'then'
            lexer_pop(lexer);

            if (parse_compound_list(res, lexer) == PARSER_OK)
            {
                fill_if_node(elif_node, *res);

                if (parse_else_clause(res, lexer) == PARSER_OK)
                {
                    fill_if_node(elif_node, *res);
                }

                *res = elif_node;
                return PARSER_OK;
            }
        }
        // ast_free(elif_node);
        *res = elif_node;
    }
    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_compound_list(struct ast **res,
                                              struct lexer *lexer)
{
    // RULE:
    // compound_list = {'\n'} and_or { ( ';' | '\n' ) {'\n'} and_or }[';']{'\n'}

    // {'\n'} case - if the and_or is not found afer {'\n'}, all the {'\n'} are
    // already poped, so CHECK if the grammar is deteministic
    while (lexer_peek(lexer).type == TOKEN_NEWLINE)
    {
        lexer_pop(lexer);
    }

    // and_or
    if (parse_and_or(res, lexer) == PARSER_OK)
    {
        // Create list node for AST
        struct ast *list_node = calloc(1, sizeof(struct ast));
        list_node->type = AST_COMMAND_LIST;

        // Add previously parsed `and_or` to AST
        fill_list_node(list_node, *res);

        //  ( ';' | '\n' )
        while (lexer_peek(lexer).type == TOKEN_SEMICOLON
               || lexer_peek(lexer).type == TOKEN_NEWLINE)
        {
            lexer_pop(lexer);

            // {'\n'}
            while (lexer_peek(lexer).type == TOKEN_NEWLINE)
            {
                lexer_pop(lexer);
            }

            // and_or
            if (parse_and_or(res, lexer) == PARSER_OK)
            {
                fill_list_node(list_node, *res);
            }
            // end of rule
            else
            {
                break;
            }
        }

        *res = list_node;
        return PARSER_OK;
    }

    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_simple_command(struct ast **res,
                                               struct lexer *lexer)
{
    // | WORD { element }
    lexer_peek(lexer);
    if (lexer->cur_tok.type == TOKEN_WORD)
    {
        struct ast *sc_node = calloc(1, sizeof(struct ast));
        // TODO: Check for NULL after allocation try

        // Fill node
        fill_sc_node(sc_node, lexer);
        // replace AST
        *res = sc_node;

        // Pop first WORD
        lexer_pop(lexer);

        // { element }
        while (parse_element(res, lexer) == PARSER_OK)
        {
            continue;
        }
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_element(struct ast **res, struct lexer *lexer)
{
    // | WORD
    lexer_peek(lexer);
    if (lexer->cur_tok.type == TOKEN_WORD || is_reserved_word(lexer->cur_tok))
    {
        // Append cur_tok to AST simple-command node
        fill_sc_node(*res, lexer);

        // Pop element
        lexer_pop(lexer);

        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
