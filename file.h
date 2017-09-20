/**
 * @file
 * @author Igor Lesik 2017
 *
 */
#pragma once

#include <sys/types.h>
#include <string.h>

typedef struct File
{
    const char* path;

} File;

int File_setXAttrStr(
    const char* file_path,
    const char* attr_name,
    const char* attr_value_str
);

ssize_t File_getXAttrStr(
    const char* file_path,
    const char* attr_name,
    char* value_buffer,
    size_t value_buffer_size
);

typedef void (*DirVisitor)(const char* path);

int Dir_iterate(
    const char* path,
    DirVisitor visitor
);
