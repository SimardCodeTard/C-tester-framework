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
/*    File: test_runner.c                                                    */
/*     Test execution and results logging funcions                           */
/*                                                                           */
/* Free Palestine, fuck fascists                                             */
/*                               Made with love and coffee by SimardCodeTard */
/* ************************************************************************* */

#include "../headers/lib.h"

static test_result_t	ft_run_test(test_t test, uint16_t n)
{
	test_result_t	res;
	int				pipefd[2];

	// TODO, run test.do_test in a forked process and deserialize test result
	// Also handle cases where the fork segfaults, using the bool expect_sigsegv
	if (pipe(pipefd) || pipefd[0] < 0 || pipefd[1] < 0)
		return (res);
	test.do_test(pipefd[1], test.params);
	res = deserialize_result(pipefd[0]);
	if (res.success)
	{
		printf(KGRN"[%2d] %s%-70s%s Success !\n" KNRM, n, KNRM,
			res.description, KGRN);
	}
	else
	{
		printf(KRED
			"[%2d] %s%-70s %sFailure !%s\nExpected: %s%s%s, Got: %s%s\n"
			KNRM, n, KNRM, res.description, KRED, KNRM, KYEL, res.expected,
			KNRM, KYEL, res.got);
	}
	return (res);
}

static void	free_result(test_t *tests, test_result_t *results, uint16_t size)
{
	uint16_t	i;

	i = 0;
	while (i < size)
	{
		if (tests[i].params)
			free(tests[i].params);
		if (results[i].description)
			free(results[i].description);
		if (results[i].expected)
			free(results[i].expected);
		if (results[i].got)
			free(results[i].got);
		i++;
	}
	free(tests);
	free(results);
}

static test_set_result_t	run_test_set(test_set_t test_set)
{
	test_set_result_t	set_result;
	test_result_t		*tests_results;
	uint16_t			i;

	set_result.success_count = 0;
	set_result.failure_count = 0;
	tests_results = malloc(test_set.total * sizeof(test_result_t));
	printf(
		"\n===== Running %s%d%s tests accross for function %s\"%s\"%s =====\n",
		KYEL, test_set.total, KNRM, KBLU, test_set.name, KNRM);
	i = 0;
	while (i < test_set.total)
	{
		tests_results[i] = ft_run_test(test_set.tests[i], i);
		if (tests_results[i].success)
			set_result.success_count++;
		else
			set_result.failure_count++;
		i++;
	}
	i = 0;
	if (!set_result.failure_count)
		printf(KGRN "All tests succeded !✅\n" KNRM);
	else
	{
		printf(KRED"%d/%d test(s) failed !❌\n" KNRM,
			set_result.failure_count,
			set_result.failure_count + set_result.success_count);
	}
	set_result.name = test_set.name;
	free_result(test_set.tests, tests_results, test_set.total);
	return (set_result);
}

static string_t	*build_failed_sets_names_strings(test_set_result_t *results,
					uint16_t failed_sets_count)
{
	string_t	*failed_sets_names;
	uint16_t	i;
	uint16_t	j;

	failed_sets_names = malloc(failed_sets_count * sizeof(string_t));
	if (!failed_sets_count)
		return (NULL);
	i = 0;
	j = 0;
	while (j < failed_sets_count)
	{
		if (results[i].failure_count)
		{
			failed_sets_names[j] = results[i].name;
			j++;
		}
		i++;
	}
	return (failed_sets_names);
}

static string_t	*build_successful_sets_names_strings(test_set_result_t *results,
					uint16_t successful_sets_count)
{
	string_t	*successful_sets_names;
	uint16_t	i;
	uint16_t	j;

	successful_sets_names = malloc(successful_sets_count * sizeof(string_t));
	if (!successful_sets_count)
		return (NULL);
	i = 0;
	j = 0;
	while (j < successful_sets_count)
	{
		if (!results[i].failure_count)
		{
			successful_sets_names[j] = results[i].name;
			j++;
		}
		i++;
	}
	return (successful_sets_names);
}

static void	print_summary(string_t *failed_sets_names,
							string_t *successful_sets_name,
							uint16_t failed_sets_count,
							uint16_t successful_sets_count,
							uint16_t failed_tests_count,
							uint16_t successful_tests_count)
{
	uint16_t	i;

	printf("\n========== Summary ==========\n\n");
	if (!failed_tests_count && !successful_tests_count)
	{
		printf(KYEL "Did not run any tests...\n" KNRM);
		return ;
	}
	if (failed_sets_count)
		printf(KRED "❌ %s%d%s / %s%d%s tests failed across %s%d%s files!\n"
			KNRM, KYEL, failed_tests_count, KRED, KYEL, failed_tests_count
			+ successful_tests_count, KRED, KYEL, failed_sets_count
			+ successful_sets_count, KRED);
	else
		printf(KGRN"✅ Successfuly ran %s%d%s tests across %s%d%s files!\n" KNRM,
			KYEL, successful_tests_count, KGRN, KYEL, successful_sets_count,
			KGRN);
	if (successful_sets_count)
	{
		printf(KGRN "Passed sets :\n");
		i = 0;
		while (i < successful_sets_count)
		{
			printf("%s", successful_sets_name[i]);
			if (i < successful_sets_count - 1)
				printf(", ");
			else
				printf("\n");
			i++;
		}
		printf(KNRM);
	}
	if (failed_sets_count)
	{
		printf(KRED "Failed sets :\n");
		i = 0;
		while (i < failed_sets_count)
		{
			printf("%s", failed_sets_names[i]);
			if (i < failed_sets_count - 1)
				printf(", ");
			else
				printf("\n");
			i++;
		}
		printf(KNRM);
	}
}

int	run_tests(test_set_t *sets, uint16_t sets_size)
{
	test_set_result_t	*set_results;
	uint16_t			i;
	uint16_t			failed_tests_count;
	uint16_t			successful_tests_count;
	uint16_t			failed_sets_count;
	uint16_t			successful_sets_count;
	string_t			*failed_sets_names;
	string_t			*successful_sets_names;

	failed_tests_count = 0;
	successful_tests_count = 0;
	failed_sets_count = 0;
	successful_sets_count = 0;
	set_results = malloc(sets_size * sizeof(test_set_result_t));
	if (!set_results)
		return (EXIT_FAILURE);
	i = 0;
	while (i < sets_size)
	{
		set_results[i] = run_test_set(sets[i]);
		failed_tests_count += set_results[i].failure_count;
		successful_tests_count += set_results[i].success_count;
		if (set_results[i].failure_count)
			failed_sets_count++;
		else
			successful_sets_count++;
		i++;
	}
	failed_sets_names = build_failed_sets_names_strings(set_results,
			failed_sets_count);
	successful_sets_names = build_successful_sets_names_strings(set_results,
			successful_sets_count);
	print_summary(failed_sets_names, successful_sets_names, failed_sets_count,
		successful_sets_count, failed_tests_count, successful_tests_count);
	free(set_results);
	free(failed_sets_names);
	free(successful_sets_names);
	if (failed_sets_count)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
