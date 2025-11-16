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
	pipe(test_pipefd);
	serialize_result(test_pipefd[1], write);
	read = deserialize_result(test_pipefd[0]);
	result.description = params->description;
	result.expected = "";
	result.got = "";
	result.success = strequals(write.description, read.description)
		&& strequals(write.expected, read.expected)
		&& strequals(write.got, read.got)
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
	test_result_t					partially_null_test_result;
	test_result_t					fully_null_test_result;

	standard_test_result.description = "Description";
	standard_test_result.expected = "Expected";
	standard_test_result.got = "Got";
	standard_test_result.success = true;

	partially_null_test_result.description = "Description";
	partially_null_test_result.expected = NULL;
	partially_null_test_result.got = NULL;
	partially_null_test_result.success = false;

	fully_null_test_result.description = NULL;
	fully_null_test_result.expected = NULL;
	fully_null_test_result.got = NULL;
	fully_null_test_result.success = false;

	tests = malloc(3 * sizeof(test_t));

	params = malloc(sizeof(params_serialization_utils_t));
	tests[0].do_test = do_test;
	tests[0].expect_sigsegv = false;
	params->description = "Standard test result";
	params->test_result = standard_test_result;
	tests[0].params = params;

	params = malloc(sizeof(params_serialization_utils_t));
	tests[1].do_test = do_test;
	tests[1].expect_sigsegv = false;
	params->description = "Partially NULL result";
	params->test_result = partially_null_test_result;
	tests[1].params = params;

	params = malloc(sizeof(params_serialization_utils_t));
	tests[2].do_test = do_test;
	tests[2].expect_sigsegv = false;
	params->description = "Fully NULL test result";
	params->test_result = fully_null_test_result;
	tests[2].params = params;

	set.name = "tests_serialization_utils";
	set.tests = tests;
	set.total = 3;
	return (set);
}
