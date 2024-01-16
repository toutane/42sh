#ifndef OPT_PARSER_H
#define OPT_PARSER_H

#include <stdbool.h>

/*
 * @brief Options structure
 */
struct options
{
    bool ast_dot;
    bool command;
    bool pretty_print;
    bool verbose;
    char *input;
};

/*
 * @brief Parse options. Fill the options structure accordingly.
 */
int parse_options(int argc, char *argv[], struct options *opt);

#endif /* ! OPT_PARSER_H */
