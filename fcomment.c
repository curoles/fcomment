/**
 * @file
 * @brief     fcomment API.
 * @author    Igor Lesik 2017
 * @copyright MIT license.
 */
#include "fcomment.h"

//#include <stdio.h>
#include <sys/xattr.h>
#include <pwd.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/utsname.h>

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
    //bool new_file = !File_exist(path);

    ssize_t file_size = File_touch(path);

    MFile_makeHiddenDir(path);

    char* origin_file_name = MFile_originFilePath(path);

    struct stat statbuf;

    if (!File_exist(origin_file_name) && stat(path, &statbuf) == 0)
    {
        char buf[1024];
        FILE* f = fopen(origin_file_name, "w");
        if (f)
        {
            gethostname(buf, sizeof(buf));
            fprintf(f, "{\"host\":\"%s\"", buf);
            struct passwd* pwd;
            if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
                fprintf(f, ",\"user\":\"%s\"", pwd->pw_name);
            else
                fprintf(f, ",\"user\":null");
            struct tm* gm = gmtime(&statbuf.st_mtim.tv_sec);
            if (gm != NULL && strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", gm))
                fprintf(f, ",\"time\":\"%s\"", buf);
            else
                fprintf(f, ",\"time\":null");
            struct utsname uname_data;
            if (uname(&uname_data) == 0) {
                fprintf(f, ",\"system\":\"%s %s\"", uname_data.sysname, uname_data.release);}
            else
                fprintf(f, ",\"system\":null");

            fprintf(f, "}");
            fclose(f);
        }
    }

    free(origin_file_name);

    if (file_size > 0)
    {
        //TODO collect text|bin & charset
    }

    //TODO sync comment

    return true;
}
