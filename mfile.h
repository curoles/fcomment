/**
 * @file
 * @brief     Interface to hidden files with meta-data.
 * @author    Igor Lesik 2017
 * @copyright MIT license.
 */
#pragma once
#include <stdbool.h>
#include <unistd.h>

bool MFile_setComment(
    const char* path,
    const char* comment,
    bool verbose
);

ssize_t MFile_getComment(
    const char* path,
    char* comment_buf,
    size_t comment_buf_size
);

char* MFile_docFilePath(const char* path);
char* MFile_originFilePath(const char* path);

bool MFile_makeHiddenDir(const char* path);

