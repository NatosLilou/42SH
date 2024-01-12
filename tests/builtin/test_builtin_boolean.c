#include <builtin.h>
#include <criterion/criterion.h>

Test(builtin_boolean, test_true)
{
    int actual = my_true();
    int expected = 0;
    cr_expect_eq(actual, expected, "Expec: %d. Got: %d", expected, actual);
}

Test(builtin_boolean, test_false)
{
    int actual = my_false();
    int expected = 1;
    cr_expect_eq(actual, expected, "Expec: %d. Got: %d", expected, actual);
}
