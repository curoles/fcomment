/**
 * @file
 * @brief     Show file comment.
 * @author    Igor Lesik 2017
 * @copyright MIT license.
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <argp.h>
#include <sys/xattr.h>

#include "file.h"

// Globals for Argp, version and email.
const char* argp_program_version = "fcomment 1.0";
const char* argp_program_bug_address = "<curoles@xxxx.org>";

// Program documentation.
static char ls_argp_doc[] =
    "List files and show comments";

static struct argp ls_argp = { 0, 0, 0, ls_argp_doc };

static void dir_visitor(const char* path)
{
    char buf[1024] = "";
    ssize_t result = File_getXAttrStr(
        path,
        "user.comment",
        buf,
        sizeof(buf)
    );

    bool found_comment = false;

    if (result == -1) {
        //perror("Can't read extended file attribute\n");
        if (errno == ENODATA || errno == ENOTSUP) {
            //TODO read from file
        }
    }
    else {
        found_comment = true;
    }

    //printf("%-*s - %s\n", 16, path, buf);
    printf("%s", path);
    if (found_comment) {printf(" - %s", buf);}
    printf("\n");
      
}

int fcomment_ls(int argc, char* argv[])
{
    argp_parse(&ls_argp, argc, argv, 0, 0, 0);

    Dir_visitAlphaOrder(".", dir_visitor);

    return EXIT_SUCCESS;
}
