#include "exec.h"

static int break_number = 0;
static int continue_number = 0;

void set_break_number(int new_val)
{
    break_number = new_val;
}

void set_continue_number(int new_val)
{
    continue_number = new_val;
}

int get_break_number(void)
{
    return break_number;
}

int get_continue_number(void)
{
    return continue_number;
}
