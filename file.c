/**
 * @file
 * @author Igor Lesik 2017
 *
 */
#include "file.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/xattr.h>

int File_setXAttrStr(
    const char* file_path,
    const char* attr_name,
    const char* attr_value_str
)
{
    int result = setxattr(
        file_path,
        attr_name,
        attr_value_str,
        strlen(attr_value_str) + 1,
        XATTR_CREATE | XATTR_REPLACE
    );


    return result;
}

ssize_t File_getXAttrStr(
    const char* file_path,
    const char* attr_name,
    char* value_buffer,
    size_t value_buffer_size
)
{
    ssize_t result = getxattr(
        file_path,
        attr_name,
        value_buffer,
        value_buffer_size
    );

    return result;
}

//#define _GNU_SOURCE
#include <dirent.h>


int Dir_visit(
    const char* path,
    DirVisitor visitor
)
{
    DIR* dir;
    if ((dir = opendir(path)) == NULL) {
        printf("Error: can't open %s\n", path);
        return 1;
    }
    else {
        struct dirent* dir_entry;
        while ((dir_entry = readdir(dir)) != NULL) {
            visitor(dir_entry->d_name);
        }
    }

    closedir(dir);

    return 0;
}

int Dir_visitAlphaOrder(
    const char* path,
    DirVisitor visitor
)
{
    struct dirent **namelist;

    int size = scandir(path, &namelist, NULL, alphasort);

    if (size < 0) {
        perror("scandir");
        return -1;
    }
    else {
        for (int i = 0; i < size; i++) {
            visitor(namelist[i]->d_name);
            free(namelist[i]);
        }
        free(namelist);
    }

    return 0;
}
