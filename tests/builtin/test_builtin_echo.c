#include <builtin.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

TestSuite(builtin_echo, .init = cr_redirect_stdout);

Test(builtin_echo, simple_string)
{
    char *cmd[] = { "echo", "hello", NULL };
    int actual = echo(cmd, stdout);
    char *str_expected = "hello\n";
    int expected = 0;
    fflush(NULL);
    cr_expect_stdout_eq_str(str_expected);
    cr_expect_eq(actual, expected, "Expec: %d. Got: %d", expected, actual);
}

Test(builtin_echo, no_newline)
{
    char *cmd[] = { "echo", "-n", "world", NULL };
    int actual = echo(cmd, stdout);
    char *str_expected = "world";
    int expected = 0;
    fflush(NULL);
    cr_expect_stdout_eq_str(str_expected);
    cr_expect_eq(actual, expected, "Expec: %d. Got: %d", expected, actual);
}

Test(builtin_echo, interpret_escape_characters)
{
    char *cmd[] = { "echo", "-e", "line1\nline2\ttab", NULL };
    int actual = echo(cmd, stdout);
    char *str_expected = "line1\nline2\ttab\n";
    int expected = 0;
    fflush(NULL);
    cr_expect_stdout_eq_str(str_expected);
    cr_expect_eq(actual, expected, "Expec: %d. Got: %d", expected, actual);
}

Test(builtin_echo, no_newline_multiple_arguments)
{
    char *cmd[] = { "echo", "-n", "multiple", "arguments", NULL };
    int actual = echo(cmd, stdout);
    char *str_expected = "multiple arguments";
    int expected = 0;
    fflush(NULL);
    cr_expect_stdout_eq_str(str_expected);
    cr_expect_eq(actual, expected, "Expec: %d. Got: %d", expected, actual);
}

Test(builtin_echo, interpret_escape_multiple_arguments)
{
    char *cmd[] = { "echo",  "-e",  "multiple", "\\t",
                    "words", "\\n", "escape",   NULL };
    int actual = echo(cmd, stdout);
    char *str_expected = "multiple \t words \n escape\n";
    int expected = 0;
    fflush(NULL);
    cr_expect_stdout_eq_str(str_expected);
    cr_expect_eq(actual, expected, "Expec: %d. Got: %d", expected, actual);
}

Test(builtin_echo, no_interpret_escape)
{
    char *cmd[] = { "echo", "-E", "line1\\nline2\\ttab", NULL };
    int actual = echo(cmd, stdout);
    char *str_expected = "line1\\nline2\\ttab\n";
    int expected = 0;
    fflush(NULL);
    cr_expect_stdout_eq_str(str_expected);
    cr_expect_eq(actual, expected, "Expec: %d. Got: %d", expected, actual);
}

Test(builtin_echo, combined_options)
{
    char *cmd[] = { "echo", "-n", "-e", "-E", "combined", "options", NULL };
    int actual = echo(cmd, stdout);
    char *str_expected = "combined options";
    int expected = 0;
    fflush(NULL);
    cr_expect_stdout_eq_str(str_expected);
    cr_expect_eq(actual, expected, "Expec: %d. Got: %d", expected, actual);
}

Test(builtin_echo, backslashes_and_tabs)
{
    char *cmd[] = { "echo", "text with \\\\ backslashes and \\t tab", NULL };
    int actual = echo(cmd, stdout);
    char *str_expected = "text with \\\\ backslashes and \\t tab\n";
    int expected = 0;
    fflush(NULL);
    cr_expect_stdout_eq_str(str_expected);
    cr_expect_eq(actual, expected, "Expec: %d. Got: %d", expected, actual);
}

Test(builtin_echo, escape_special_characters)
{
    char *cmd[] = {
        "echo", "-e",
        "words with \\\\\\\\ backslashes, \\\\t tabs, and \\\\n newlines", NULL
    };
    int actual = echo(cmd, stdout);
    char *str_expected =
        "words with \\\\ backslashes, \\t tabs, and \\n newlines\n";
    int expected = 0;
    fflush(NULL);
    cr_expect_stdout_eq_str(str_expected);
    cr_expect_eq(actual, expected, "Expec: %d. Got: %d", expected, actual);
}

Test(builtin_echo, mixed_options_and_special_characters)
{
    char *cmd[] = { "echo", "-e", "-n",
                    "mixing \\\\ options with \\\\ backslashes \\\\n", NULL };
    int actual = echo(cmd, stdout);
    char *str_expected = "mixing \\ options with \\ backslashes \\n";
    int expected = 0;
    fflush(NULL);
    cr_expect_stdout_eq_str(str_expected);
    cr_expect_eq(actual, expected, "Expec: %d. Got: %d", expected, actual);
}

Test(builtin_echo, composed_flags_1)
{
    char *cmd[] = { "echo", "-ene", "-nnnn",
                    "mixing \\\\ options with \\\\ backslashes \\\\n", NULL };
    int actual = echo(cmd, stdout);
    char *str_expected = "mixing \\ options with \\ backslashes \\n";
    int expected = 0;
    fflush(NULL);
    cr_expect_stdout_eq_str(str_expected);
    cr_expect_eq(actual, expected, "Expec: %d. Got: %d", expected, actual);
}

Test(builtin_echo, composed_flags_2)
{
    char *cmd[] = { "echo", "-eneE", "-nnnn",
                    "mixing \\\\ options with \\\\ backslashes \\\\n", NULL };
    int actual = echo(cmd, stdout);
    char *str_expected = "mixing \\\\ options with \\\\ backslashes \\\\n";
    int expected = 0;
    fflush(NULL);
    cr_expect_stdout_eq_str(str_expected);
    cr_expect_eq(actual, expected, "Expec: %d. Got: %d", expected, actual);
}

Test(builtin_echo, composed_flags_3)
{
    char *cmd[] = { "echo", "-ene", "-nEnnen",
                    "mixing \\\\ options with \\\\ backslashes \\\\n", NULL };
    int actual = echo(cmd, stdout);
    char *str_expected = "mixing \\ options with \\ backslashes \\n";
    int expected = 0;
    fflush(NULL);
    cr_expect_stdout_eq_str(str_expected);
    cr_expect_eq(actual, expected, "Expec: %d. Got: %d", expected, actual);
}

Test(builtin_echo, composed_flags_4)
{
    char *cmd[] = { "echo", "-eE", "-e",
                    "mixing \\\\ options with \\\\ backslashes \\\\n", NULL };
    int actual = echo(cmd, stdout);
    char *str_expected = "mixing \\ options with \\ backslashes \\n\n";
    int expected = 0;
    fflush(NULL);
    cr_expect_stdout_eq_str(str_expected);
    cr_expect_eq(actual, expected, "Expec: %d. Got: %d", expected, actual);
}
