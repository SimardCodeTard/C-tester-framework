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
/*                                                                           */
/*                               Made with love and coffee by SimardCodeTard */
/* ************************************************************************* */

#ifndef LIB_H
# define LIB_H

# include "includes.h"
# include "defines.h"
# include "types.h"

/* utils.c */
uint8_t					numlen(uint16_t n);

/* serialization_utils.c */
test_result_t			deserialize_result(uint16_t pipe_fd);
serialization_result_t	serialize_result(uint16_t pipe_fd, test_result_t result);


#endif
