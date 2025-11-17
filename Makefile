CFLAGS = -Wall -Wextra -Werror -g3 -Iinclude

CFLAGS_TESTS = -Wall -Wextra -Werror -g3 -Iinclude

CC = cc

SRC_FILES = src/serialization_utils.c \
	src/test_runner.c \
	src/utils.c

TESTS_FILES = tests/utils/string.c \
	tests/test_serialization_utils/test_serialization_utils.c \
	tests/utils/test_result_tostring.c \
	tests/main.c \

SRCS = $(SRC_FILES)

TESTS = $(TESTS_FILES)

DFILES = $(ALL_OBJ:.o=.d)

SRCS_OBJ = $(SRCS:.c=.o)

TESTS_OBJ = $(TESTS:.c=.o)

ALL_OBJ = $(SRCS_OBJ) $(TESTS_OBJ)

NAME = c-test-framework.a

TESTS_NAME = c-test-framework-tests.o

VERSION = 1.0.0

RELEASE_NAME = c-test-framework-$(VERSION)

RELEASE_FILE_NAME = $(RELEASE_NAME).tar.gz

all: $(NAME)

$(NAME) : $(SRCS_OBJ)
	ar rcs $(NAME) $(SRCS_OBJ)

tests: $(TESTS_NAME)

$(TESTS_NAME): $(NAME)
	$(CC) -o $(TESTS_NAME) $(TESTS_FILES) $^ $(CFLAGS_TESTS) -L -l:$(NAME) -Llib -lft

%.o: %.c
	$(CC) -c $(CFLAGS) -MMD -MP $< -o $@

release : all
	mkdir -p $(RELEASE_NAME)/include/headers
	mkdir -p $(RELEASE_NAME)/lib
	cp $(NAME) $(RELEASE_NAME)/lib/
	cp c_test_framework.h $(RELEASE_NAME)/include/
	cp headers/types.h $(RELEASE_NAME)/include/headers/
	cp headers/defines.h $(RELEASE_NAME)/include/headers/
	cp headers/includes.h $(RELEASE_NAME)/include/headers/
	tar -czvf $(RELEASE_FILE_NAME) $(RELEASE_NAME)
	rm -rf $(RELEASE_NAME)

clean:
	rm -rf ${SRCS_OBJ} $(TESTS_OBJ) $(DFILES) $(RELEASE_FILE_NAME)

fclean: clean
	rm -f $(NAME) $(TESTS_NAME)

re: fclean all

.PHONY: all tests clean fclean re %.o

-include : $(DFILES)
