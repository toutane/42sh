#include "exec.h"

int eval_subshell(struct ast *ast, struct mem *mem)
{
    struct ast_subshell *ast_subshell = (struct ast_subshell *)ast;
    // TODO: Copy mem to mem2
    struct mem *mem2 = mem;
    int ret_val = eval_ast(ast_subshell->compound_list, mem2);
    // Destroy mem2
    return ret_val;
}
