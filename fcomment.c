/**
 * @file
 * @brief     fcomment API.
 * @author    Igor Lesik 2017
 * @copyright MIT license.
 */
#include "fcomment.h"

//#include <stdio.h>
#include <sys/xattr.h>

#include "file.h"
#include "mfile.h"

ssize_t
FComment_getXAComment(
    const char* path,
    char* comment_buf,
    size_t comment_buf_size
)
{
    ssize_t bytes_read = File_getXAttrStr(
        path,
        "user.comment",
        comment_buf,
        comment_buf_size
    );

    return bytes_read;
}

/**
 * @return Number of bytes written to the buffer, -1 on failure and errno set.
 *         ERANGE The size of the value buffer is too small to hold the result.
 */
ssize_t
FComment_getComment(
    const char* path,       ///< path to file
    char* comment_buf,      ///< buffer to store comment string
    size_t comment_buf_size ///< size of the buffer
)
{
    ssize_t bytes_read = MFile_getComment(path, comment_buf, comment_buf_size);

    if (bytes_read < 0) {
        bytes_read = FComment_getXAComment(path, comment_buf, comment_buf_size);
        //if (xa_bytes_read < 0 && (errno == ENODATA || errno == ENOTSUP)) {
    }

    return bytes_read;
}

bool
FComment_setComment(
    const char* path,
    const char* comment,
    bool verbose
)
{
    if (!File_exist(path)) {
        printf("Error: file %s does not exist!", path);
        return false;
    }

    if (strlen(comment) > FCOMMENT_MAX_SIZE-1) {
        printf("Error: comment is too long, max length %u\n", FCOMMENT_MAX_SIZE-1);
        return false;
    }

    int xa_result = File_setXAttrStr(
        path,
        "user.comment",
        comment
    );

    if (0 != xa_result && verbose) {
        perror("Failed to set extended file attribute");
    }

    bool mf_result = MFile_setComment(path, comment, verbose);

    return mf_result;
}

bool
FComment_copy(
    const char* path_from,
    const char* path_to
)
{
    if (!File_exist(path_to)) {
        ssize_t bytes_copied = File_copy(path_from, path_to);
        if (bytes_copied < 1) return false;
    }

    char buf[FCOMMENT_MAX_SIZE];
    ssize_t comment_size = FComment_getComment(path_from, buf, sizeof(buf));

    if (comment_size < 0) return false;

    return FComment_setComment(path_to, buf, comment_size);
}

bool
FComment_touchFile(
    const char* path,
    bool verbose
)
{
    bool new_file = !File_exist(path);

    /*ssize_t file_size = */ File_touch(path);

    if (new_file)
    {

    }

    return true;
}
