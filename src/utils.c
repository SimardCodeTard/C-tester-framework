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
/*    File: utils.c                                                          */
/*     General util functions                                                */
/*                                                                           */
/* Free Palestine, fuck fascists                                             */
/*                               Made with love and coffee by SimardCodeTard */
/* ************************************************************************* */

#include "../headers/lib.h"

uint8_t	numlen(uint16_t n)
{
	uint8_t	len;

	len = 1;
	while (n > 9)
	{
		n /= 10;
		len++;
	}
	return (len);
}

string_t	to_printable(uint8_t c)
{
	string_t	buffer;

	if (isprint(c))
	{
		buffer = calloc(2, sizeof(char));
		buffer[0] = c;
	}
	else
	{
		buffer = calloc(6, sizeof(char));
		sprintf((string_t) buffer, "\\%03d", c);
	}
	return (buffer);
}

/**
 * Safely frees allocated memory by trying first in a child process
 * We also redirect STDERR to a temporary pipe to silence error messages in the child process
 * This is kinda dumb and I hate it, please open a pull request if you have a better idea
 */
void	safe_free(void *ptr)
{
	int	pid;
	int	status;

	if (!ptr)
		return ;
	pid = fork();
	if (pid == 0)
	{
		close(STDERR_FILENO);
		free(ptr);
		exit(EXIT_SUCCESS);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
		free(ptr);
}

string_t	secure_string_to_log(string_t str)
{
	if (!str)
		return ("(NULL)");
	if (str[0] == '\0')
		return ("(empty string)");
	return str;
}
