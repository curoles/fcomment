/**
 * @file
 * @author Igor Lesik 2017
 *
 */
#pragma once

#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

//typedef struct File
//{
//    const char* path;
//} File;

bool File_exist(const char* path);

bool File_isDirectory(const char* path);
bool File_isRegularFile(const char* path);
bool File_isSymbolicLink(const char* path);

void File_print(
    const char* path,
    FILE* outstream
);

ssize_t
File_copy(
    const char* source_file_name,
    const char* dest_file_name
);

int File_setXAttrStr(
    const char* file_path,
    const char* attr_name,
    const char* attr_value_str
);

/**
 * @return On success, return a nonnegative value which is the size
 *         in bytes of the extended attribute value.  On failure, -1 is
 *         returned and errno is set appropriately.
 */
ssize_t File_getXAttrStr(
    const char* file_path,
    const char* attr_name,
    char* value_buffer,
    size_t value_buffer_size
);

typedef void (*DirVisitor)(const char* base, const char* filename);

int Dir_visit(
    const char* path,
    DirVisitor visitor
);


int Dir_visitAlphaOrder(
    const char* path,
    DirVisitor visitor
);

/**
 * @return 0 upon success, otherwise -1.
 */
int Dir_make(
    const char* path
);
