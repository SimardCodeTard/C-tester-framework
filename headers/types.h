#ifndef TYPES_H
# define TYPES_h
# include "includes.h"

typedef uint8_t	bool_t;

typedef char *	string_t;

typedef struct test_result_s
{
	string_t	description;
	string_t	expected;
	string_t	got;
	bool_t		success;
	void		(*done)(test_result_t);
}	test_result_t;

typedef struct test_s
{
	test_result_t	(*do_test)(void *);
	void			*params;
	bool_t			expect_systerm;
}	test_t;

typedef struct test_set_result_s
{
	uint8_t		success_count;
	uint8_t		failure_count;
}	test_set_result_t;

typedef struct test_set_s
{
	string_t			name;
	test_set_result_t	(*do_test_set)(void);
}	test_set_t;

#endif
