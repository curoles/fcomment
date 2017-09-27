/**
 * @file
 * @brief     fcomment API.
 * @author    Igor Lesik 2017
 * @copyright MIT license.
 */
#pragma once

#include <stdlib.h>
#include <stdbool.h>

#define FCOMMENT_MAX_SIZE 1024

/**
 * @return Number of bytes written to the buffer, -1 on failure and errno set.
 *         ERANGE The size of the value buffer is too small to hold the result.
 */
ssize_t
FComment_getComment(
    const char* path,       ///< path to file
    char* comment_buf,      ///< buffer to store comment string
    size_t comment_buf_size ///< size of the buffer
);

bool
FComment_setComment(
    const char* path,
    const char* comment,
    bool verbose
);

bool
FComment_copy(
    const char* path_from,
    const char* path_to
);
