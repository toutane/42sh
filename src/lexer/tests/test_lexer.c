#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <stdio.h>

#include "../lexer.h"

Test(try, test1) { cr_expect(add(3, 4) == 7); }
