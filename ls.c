/**
 * @file
 * @author Igor Lesik 2017
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <argp.h>

#include "file.h"

// Globals for Argp, version and email.
const char* argp_program_version = "fcomment 1.0";
const char* argp_program_bug_address = "<curoles@xxxx.org>";

// Program documentation.
static char ls_argp_doc[] =
    "List files and show comments";

static struct argp ls_argp = { 0, 0, 0, ls_argp_doc };

void dir_visitor(const char* path)
{
    printf("visiting %s\n", path);
    char buf[1024];
    ssize_t result = File_getXAttrStr(
        path,
        "comment",
        buf,
        sizeof(buf)
    );
    printf("result %zd errno=%d\n", result, errno);
             
}

int fcomment_ls(int argc, char* argv[])
{
    argp_parse(&ls_argp, argc, argv, 0, 0, 0);

    Dir_iterate(".", dir_visitor);

    return EXIT_SUCCESS;
}
