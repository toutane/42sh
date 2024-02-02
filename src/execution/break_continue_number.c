#include "exec.h"

static int break_number = 0;
static int continue_number = 0;
static int loop_number = 0;

void set_break_number(int new_val)
{
    if (new_val > loop_number)
        break_number = loop_number;
    else
        break_number = new_val;
}

void set_continue_number(int new_val)
{
    if (new_val > loop_number)
        continue_number = loop_number;
    else
        continue_number = new_val;
}

void set_loop_number(int new_val)
{
    loop_number = new_val;
}

int get_break_number(void)
{
    return break_number;
}

int get_continue_number(void)
{
    return continue_number;
}

int get_loop_number(void)
{
    return loop_number;
}
