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

#include "tests.h"

bool_t	do_test(test_result_t out)
{
	test_result_t	in;
	int				fd;

	fd = open(SERIALIZATION_FILE, O_CREAT | O_WRONLY, 0777);
	if (fd == -1)
	{
		fprintf(stderr, KRED
			"ERROR: Failed to open file in tests_serialization.\n" KNRM);
		return (false);
	}
	serialize_result(fd, out);
	close(fd);
	fd = open(SERIALIZATION_FILE, O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, KRED
			"ERROR: Failed to open file in tests_serialization.\n" KNRM);
		return (false);
	}
	in = deserialize_result(fd);
	close(fd);
	return (strequals(in.description, out.description)
		&& strequals(in.expected, out.expected) && strequals(in.got, out.got)
		&& in.success == out.success);
}

void	tests_serialization(void)
{
	bool_t			ok;
	test_result_t	standard_test_result;
	test_result_t	partially_null_test_result;
	test_result_t	fully_null_test_result;

	ok = true;
	printf("Running serialization tests\n");
	standard_test_result.description = "Description";
	standard_test_result.expected = "Expected";
	standard_test_result.got = "Got";
	standard_test_result.success = true;
	if (!do_test(standard_test_result))
	{
		printf(KRED "Test with standard result failed !\n" KNRM);
		ok = false;
	}
	else
		printf(KGRN "Test with standard result succeded !\n" KNRM);
	partially_null_test_result.description = "Description";
	partially_null_test_result.expected = NULL;
	partially_null_test_result.got = NULL;
	partially_null_test_result.success = false;
	if (do_test(partially_null_test_result))
		printf(KGRN "Test with partially NULL result succeded !\n" KNRM);
	else
	{
		ok = false;
		printf(KRED "Test with partially NULL result failed !\n" KNRM);
	}
	fully_null_test_result.description = NULL;
	fully_null_test_result.expected = NULL;
	fully_null_test_result.got = NULL;
	fully_null_test_result.success = false;
	if (!do_test(fully_null_test_result))
	{
		printf(KRED "Test with fully NULL result failed !\n" KNRM);
		ok = false;
	}
	else
		printf(KGRN "Test with fully NULL result succeded !\n" KNRM);
	printf("------ Summary ------\n");
	if (ok)
		printf(KGRN "Serialize: ✅ All tests succeeded !\n" KNRM);
	else
		printf(KRED "Serialize: ❌ Some tests failed !\n" KNRM);
}
