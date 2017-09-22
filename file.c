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
#include <sys/stat.h>

bool File_exist(const char* path)
{
    struct stat path_stat;
    return stat(path, &path_stat) == 0;
}

bool File_isDirectory(const char* path)
{
    struct stat path_stat;
    return stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode);
}

bool File_isRegularFile(const char* path)
{
    struct stat path_stat;
    return stat(path, &path_stat) == 0 && S_ISREG(path_stat.st_mode);
}

bool File_isSymbolicLink(const char* path)
{
    struct stat path_stat;
    return stat(path, &path_stat) == 0 && S_ISLNK(path_stat.st_mode);
}

/**
 *
 * @return On success, zero is returned. On failure, -1 is returned.
 */
int File_setXAttrStr(
    const char* file_path,
    const char* attr_name,
    const char* attr_value_str
)
{
    /* If flags is zero, the extended attribute will be
     * created if it does not exist, or the value will be replaced if the
     * attribute already exists.
     */
    int result = setxattr(
        file_path,
        attr_name,
        attr_value_str,
        strlen(attr_value_str) + 1,
        0 /*XATTR_CREATE | XATTR_REPLACE*/
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
            visitor(path, dir_entry->d_name);
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
            visitor(path, namelist[i]->d_name);
            free(namelist[i]);
        }
        free(namelist);
    }

    return 0;
}
