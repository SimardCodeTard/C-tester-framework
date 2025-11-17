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
/*    File: tests_result_tostring.c                                          */
/*     Stringify functions for test_result_t                                 */
/*                                                                           */
/* Free Palestine, fuck fascists                                             */
/*                               Made with love and coffee by SimardCodeTard */
/* ************************************************************************* */

#include "../headers/lib.h"

string_t	test_result_to_string(test_result_t result)
{
	string_t	to_string;
	string_t	previous_to_string;

	to_string = "";
	to_string = ft_strjoin(to_string, "{ Got: ");
	previous_to_string = to_string;
	to_string = ft_strjoin(to_string, secure_string_to_log(result.got));
	free(previous_to_string);
	previous_to_string = to_string;
	to_string = ft_strjoin(to_string, ", sucess: ");
	free(previous_to_string);
	previous_to_string = to_string;
	if (result.success)
		to_string = ft_strjoin(to_string, "true");
	else
		to_string = ft_strjoin(to_string, "false");
	free(previous_to_string);
	previous_to_string = to_string;
	to_string = ft_strjoin(to_string, " }");
	free(previous_to_string);
	return to_string;
}
