#ifndef OPT_PARSER_H
#define OPT_PARSER_H

/*
 * @brief Options structure
 */
struct options
{
    int command;
    int pretty_print;
    int verbose;
};

/*
 * @brief Parse options. Fill the options structure accordingly.
 */
void parse_options(int argc, char *argv[], struct options *opt);

#endif /* ! OPT_PARSER_H */
