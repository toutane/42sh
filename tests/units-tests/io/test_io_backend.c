#include <criterion/criterion.h>

Test(simple_testsuite, simple_correct_test)
{
    cr_expect(1 == 1);
}

Test(simple_testsuite, simple_wrong_test)
{
    cr_expect(1 == 0);
}
