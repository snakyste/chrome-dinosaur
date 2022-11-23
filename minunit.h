/* file: minunit.h taken from by Jera design - https://jera.com/techinfo/jtns/jtn002 and modified */

#ifndef MINUNIT_H
#define MINUNIT_H

#include <string.h>

extern int tests_run;
extern int num_tests;

int run_all_tests(int argc, char **argv);

#define mu_assert(message, test) do \
{ \
	if (!(test)) \
		return message; \
	} \
while (0)
 
#define mu_run_test(test) do \
{ \
	char *message = test(); \
	tests_run++; \
	if (message) \
		return message; \
} \
while (0)

#endif
