#ifndef OPT_PARSER_H
#define OPT_PARSER_H

/*
 * @brief Options structure
 */
struct options
{
    int ast_dot;
    int command;
    int pretty_print;
    int verbose;
    char *input;
};

/*
 * @brief Parse options. Fill the options structure accordingly.
 */
int parse_options(int argc, char *argv[], struct options *opt);

#endif /* ! OPT_PARSER_H */
