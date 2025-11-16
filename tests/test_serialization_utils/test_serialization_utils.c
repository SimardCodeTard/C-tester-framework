/* ************************************************************************* */
/*                                                                           */
/*      /$$$$$$  /$$$$$$ /$$      /$$  /$$$$$$  /$$$$$$$  /$$$$$$$           */
/*     /$$__  $$|_  $$_/| $$$    /$$$ /$$__  $$| $$__  $$| $$__  $$          */
/*    | $$  \__/  | $$  | $$$$  /$$$$| $$  \ $$| $$  \ $$| $$  \ $$          */
/*    |  $$$$$$   | $$  | $$ $$/$$ $$| $$$$$$$$| $$$$$$$/| $$  | $$          */
/*     \____  $$  | $$  | $$  $$$| $$| $$__  $$| $$__  $$| $$  | $$          */
/*     /$$  \ $$  | $$  | $$\  $ | $$| $$  | $$| $$  \ $$| $$  | $$          */
/*    |  $$$$$$/ /$$$$$$| $$ \/  | $$| $$  | $$| $$  | $$| $$$$$$$/          */
/*     \______/ |______/|__/     |__/|__/  |__/|__/  |__/|_______/           */
/*                                                                           */
/*    File: tests_serialization_utils.c                                      */
/*     Unit tests for the serilization utils                                 */
/*                                                                           */
/* Free Palestine, fuck fascists                                             */
/*                               Made with love and coffee by SimardCodeTard */
/* ************************************************************************* */

#include "test_serialization_utils.h"

test_result_t	do_test(int pipefd, void *p)
{
	test_result_t						result;
	test_result_t						write;
	test_result_t						read;
	const params_serialization_utils_t	*params =
		(params_serialization_utils_t *)p;
	int									test_pipefd[2];

	write = params->test_result;
	printf("Got: %s\n", params->test_result.got ? params->test_result.got[0] ? params->test_result.got : "(empty string)" : "(null)");
	pipe(test_pipefd);
	serialize_result(test_pipefd[1], write);
	read = deserialize_result(test_pipefd[0]);
	result.got = test_result_to_string(read);
	write.got = secure_string_to_log(write.got);
	result.success = strequals(write.got, read.got)
		&& write.success == read.success;
	serialize_result(pipefd, result);
	return (result);
}

test_set_t	tests_serialization(void)
{
	test_set_t						set;
	test_t							*tests;
	params_serialization_utils_t	*params;
	test_result_t					standard_test_result;
	test_result_t					null_test_result;

	standard_test_result.got = "Got";
	standard_test_result.success = true;


	null_test_result.got = NULL;
	null_test_result.success = false;

	tests = malloc(2 * sizeof(test_t));

	params = malloc(sizeof(params_serialization_utils_t));
	tests[0].do_test = do_test;
	tests[0].expect_sigsegv = false;
	params->test_result = standard_test_result;
	tests[0].params = params;
	tests[0].description = "Standard test result";
	tests[0].expected = test_result_to_string(standard_test_result);

	params = malloc(sizeof(params_serialization_utils_t));
	tests[1].do_test = do_test;
	tests[1].expect_sigsegv = false;
	params->test_result = null_test_result;
	tests[1].params = params;
	tests[1].description = "NULL test result";
	tests[1].expected = test_result_to_string(null_test_result);

	set.name = "tests_serialization_utils";
	set.tests = tests;
	set.total = 2;
	return (set);
}
