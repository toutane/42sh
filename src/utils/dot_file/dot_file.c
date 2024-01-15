#include "dot_file.h"

#define LIGHT_BLUE "#b0d5f5"
#define DARK_BLUE "#1588ed"
#define LIGHT_GREEN "#b0f5b4"
#define DARK_GREEN "#2b8f31"
#define LIGHT_PINK "#ffd4f6"
#define DARK_PINK "#fa96e6"

#define NODE_FORMAT                                                            \
    "node%p [label=%s, color=\"%s\", style=filled, fillcolor=\"%s\"]\n"

// Lookup table for AST node types
static char *ast_type_str[] = {
    [AST_COMMAND_LIST] = "LIST",
    [AST_SIMPLE_COMMAND] = "CMD",
    [AST_CONDITION] = "IF",
};

/*
 * @brief Write a dot formated ast into a file
 */
static void generate_dot_file(struct ast *node, FILE *file)
{
    if (node == NULL)
    {
        return;
    }

    enum ast_type type = node->type;

    // Print the node
    if (type == AST_SIMPLE_COMMAND)
    {
        fprintf(file, NODE_FORMAT, (void *)node, ast_type_str[type], DARK_BLUE,
                LIGHT_BLUE);

        // Print the argv node
        fprintf(file, "node%pargv [fontsize=17, margin=0.2, label=\"",
                (void *)node);
        for (size_t i = 0; i < node->nb_args; i++)
        {
            fprintf(file, "%s%s", i == 0 ? "" : "|", node->argv[i]);
        }
        fprintf(file, "\"];\n");

        fprintf(file, "node%p -> node%pargv [label=\" Argv\",headport=nw];\n",
                (void *)node, (void *)node);
    }
    else
    {
        char *dark_color = type == AST_COMMAND_LIST ? DARK_GREEN : DARK_PINK;
        char *light_color = type == AST_COMMAND_LIST ? LIGHT_GREEN : LIGHT_PINK;
        fprintf(file, NODE_FORMAT, (void *)node, ast_type_str[type], dark_color,
                light_color);
    }

    // Print the edges to children
    for (size_t i = 0; i < node->nb_child; i++)
    {
        fprintf(file, "node%p -> node%p [label=\"%s\"];\n", (void *)node,
                (void *)node->children[i],
                type == AST_COMMAND_LIST
                    ? " Child"
                    : (i == 0 ? " If" : (i == 1 ? " Then" : " Else")));

        generate_dot_file(node->children[i], file);
    }
}

int create_dot_file(struct ast *ast, char *filename)
{
    printf("Output AST in dot format (in \"%s\")\n", filename);
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        fprintf(stderr, "42sh: create_dot_file: Could not open file %s\n",
                filename);
        return 1;
    }

    fprintf(file, "digraph AST {\n");

    fprintf(file, "splines=false;\n"); // Remove curved edges
    fprintf(file,
            "node [fontname=helvetica, shape=record];\n"); // Set font and shape
                                                           // for nodes
    fprintf(file, "edge [fontname=helvetica];\n"); // Set font for edges

    generate_dot_file(ast, file);

    fprintf(file, "}\n");

    fclose(file);
    return 0;
}
