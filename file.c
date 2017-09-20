/**
 * @file
 * @author Igor Lesik 2017
 *
 */
#include "file.h"


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

#include <dirent.h>


int Dir_iterate(
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
