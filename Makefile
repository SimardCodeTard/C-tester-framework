CFLAGS = -Wall -Wextra -Werror

CFLAGS_TESTS = -Wall -Wextra -Werror -g3

CC = cc

SRC_FILES = src/serialization_utils.c \
	src/test_runner.c \
	src/utils.c

TESTS_FILES = tests/utils/string.c \
	tests/tests_serialization_utils.c \
	tests/main.c \

SRCS = $(SRC_FILES)

DFILES = $(ALL_OBJ:.o=.d)

TESTS = $(TESTS_FILES)

SRCS_OBJ = $(SRCS:.c=.o)

TESTS_OBJ = $(TESTS:.c=.o)

ALL_OBJ = $(SRCS_OBJ) $(TESTS_OBJ)

NAME = c-test-framework.a

TESTS_NAME = c-test-framework-tests.a

all: $(NAME)

$(NAME) : $(SRCS_OBJ)
	ar rcs $(NAME) $(SRCS_OBJ)

tests: $(TESTS_NAME)

$(TESTS_NAME): $(NAME)
	$(CC) -o $(TESTS_NAME) $(TESTS_FILES) $^ $(CFLAGS_TESTS) -L -l:$(NAME)

tests/%.o: tests/%.c
	$(CC) $(CFLAGS) -MMD -MP $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) -MMD -MP $< -o $@

clean:
	rm -rf ${SRCS_OBJ} $(TESTS_OBJ) $(DFILES)

fclean: clean
	rm -f $(NAME) $(TESTS_NAME)

re: fclean all

.PHONY: all tests clean fclean re %.o

-include : $(DFILES)
