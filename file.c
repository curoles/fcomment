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
#include <sys/sendfile.h>
#include <libgen.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <wchar.h>

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

ssize_t File_read(
    const char* path,
    char* buf,
    size_t buf_size,
    off_t pos_in_file
)
{
    int fd = open(path, O_RDONLY);

    if (fd < 0) {
        //perror("Can't open comment file");
        return -1;
    }

    ssize_t bytes_read = pread(fd, buf, buf_size, pos_in_file);

    close(fd);

    return bytes_read;
}

ssize_t
File_copy(
    const char* source_file_name,
    const char* dest_file_name
)
{
    int source = open(source_file_name, O_RDONLY, 0);
    if (source < 0) {
        perror("Can't open source file");
        return -1;
    }

    int dest = open(dest_file_name, O_WRONLY | O_CREAT , 0644);
    if (dest < 0) {
        perror("Can't open destination file");
        return -1;
    }

    struct stat stat_source;
    fstat(source, &stat_source);

    sendfile(dest, source, 0, stat_source.st_size);

    close(source);
    close(dest);

    return stat_source.st_size;
}

void File_print(
    const char* path,
    FILE* outstream
)
{
    FILE* fin = fopen(path, "r");

    if (fin == NULL) {
        return;
    }

    //wint_t wc;
    char c;

    fprintf(outstream, "Doc file:\n");

    /*while ((wc=fgetwc(fin)) != WEOF) {
        fputwc(wc, outstream);
    }*/

    while ((c=fgetc(fin)) != EOF) {
        fputc(c, outstream);
    }

    fclose(fin);
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




/**
 * @return 0 upon success, otherwise -1.
 */
int Dir_make(
    const char* path
)
{
    int status = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    if (status != 0 && errno == EEXIST) {
        status = 0;
    }

    return 0;
}



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
