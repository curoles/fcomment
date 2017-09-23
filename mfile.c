/**
 * @file
 * @brief     Interface to hidden files with meta-data.
 * @author    Igor Lesik 2017
 * @copyright MIT license.
 */
#define _GNU_SOURCE
#include "mfile.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <libgen.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

#include "file.h"

static const char HIDDEN_DIR[] = ".meta-fcomment";
static const char COMMENT_FILE[] = "comment.txt";

static void ptr_ptr_free(char** allocated_resource) {
    //printf("free %s\n", *allocated_resource);
    free(*allocated_resource);
}
#define AUTO_FREE __attribute__ ((__cleanup__(ptr_ptr_free)))

//@return caller must FREE allocated string!
char* MFile_hiddenDirPath(
    const char* path,
    bool full    
)
{
    char* path_dir_copy AUTO_FREE = strdup(path);
    char* path_base_copy AUTO_FREE = strdup(path);

    char* base_name = basename(path_base_copy);
    char* dir_name = dirname(path_dir_copy);

    char* hidden_dir = NULL;
    int hidden_dir_name_size = (full) ?
        asprintf(&hidden_dir, "%s/%s/%s", dir_name, HIDDEN_DIR, base_name):
        asprintf(&hidden_dir, "%s/%s", dir_name, HIDDEN_DIR);

    assert(hidden_dir_name_size > 0);

    return hidden_dir;
}


bool MFile_isHiddenDirExist(
    const char* path,
    bool full
)
{
    char* hidden_dir AUTO_FREE = MFile_hiddenDirPath(path, full);

    if (hidden_dir == NULL) return false;

    return File_exist(hidden_dir);
}

bool MFile_makeHiddenDir(const char* path)
{
    if (!MFile_isHiddenDirExist(path, false)) {
        char* hidden_dir AUTO_FREE = MFile_hiddenDirPath(path, false);
        int hidden_dir_created = Dir_make(hidden_dir);
        if (0 != hidden_dir_created) {
            perror("Can't created hidden directory");
            return false;
        }
    }

    if (!MFile_isHiddenDirExist(path, true)) {
        char* hidden_dir AUTO_FREE = MFile_hiddenDirPath(path, true);
        int hidden_dir_created = Dir_make(hidden_dir);
        if (0 != hidden_dir_created) {
            perror("Can't created hidden directory");
            return false;
        }
    }

    return true;
}

bool MFile_setComment(
    const char* path,
    const char* comment,
    bool verbose
)
{
    if (!MFile_makeHiddenDir(path)) {
        // printf("Error: Can't make hidden directory\n");
        return false;
    }

    char* meta_dir AUTO_FREE = MFile_hiddenDirPath(path, true);


    char* comment_file_name AUTO_FREE = NULL;
    asprintf(&comment_file_name, "%s/%s", meta_dir, COMMENT_FILE);

    int fd = open(comment_file_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0) {
        perror("Can't open comment file");
        return false;
    }

    ssize_t bytes_written = write(fd, comment, strlen(comment) + 1);

    if (bytes_written < 0) {
        perror("Can't write into comment file");
    }
    else if (verbose) {
        printf("%zd bytes written to %s\n", bytes_written, comment_file_name);
    }

    close(fd);

    return bytes_written > 0;
}


ssize_t MFile_getComment(
    const char* path,
    char* comment_buf,
    size_t comment_buf_size
)
{
    char* meta_dir AUTO_FREE = MFile_hiddenDirPath(path, true);

    char* comment_file_name AUTO_FREE = NULL;
    asprintf(&comment_file_name, "%s/%s", meta_dir, COMMENT_FILE);

    int fd = open(comment_file_name, O_RDONLY);
    if (fd < 0) {
        //perror("Can't open comment file");
        return -1;
    }

    ssize_t bytes_read = read(fd, comment_buf, comment_buf_size);

    close(fd);

    return bytes_read;
}
