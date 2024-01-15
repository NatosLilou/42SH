bin_PROGRAMS = \
	test_builtin_boolean \
	test_builtin_echo

SUBDIRS = ../../src/builtin

test_builtin_boolean_SOURCES = test_builtin_boolean.c
test_builtin_boolean_CPPFLAGS = -I$(top_srcdir)/src/builtin
test_builtin_boolean_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic -lcriterion
test_builtin_boolean_LDADD = \
	$(top_builddir)/src/builtin/libbuiltin.a

test_builtin_echo_SOURCES = test_builtin_echo.c
test_builtin_echo_CPPFLAGS = -I$(top_srcdir)/src/builtin
test_builtin_echo_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic -lcriterion
test_builtin_echo_LDADD = \
	$(top_builddir)/src/builtin/libbuiltin.a

check-local: $(bin_PROGRAMS)
	./test_builtin_boolean
	./test_builtin_echo
