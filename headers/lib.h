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
/*    File: lib.h                                                            */
/*     Internal functions of the project                                     */
/*                                                                           */
/* Free Palestine, fuck fascists                                             */
/*                               Made with love and coffee by SimardCodeTard */
/* ************************************************************************* */

#ifndef LIB_H
# define LIB_H

# include "colors.h"
# include "defines.h"
# include "includes.h"
# include "messages.h"
# include "types.h"

/* utils.c */
uint8_t					numlen(uint16_t n);
string_t				to_printable(uint8_t c);
string_t				secure_string_to_log(string_t str);

/* serialization_utils.c */
test_result_t			deserialize_result(int pipe_fd);
serialization_result_t	serialize_result(int pipe_fd, test_result_t result);

/* safe_free.c */
void					safe_free(void *ptr);

#endif
