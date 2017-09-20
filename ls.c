/**
 * @file
 * @author Igor Lesik 2017
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <argp.h>

// Globals for Argp, version and email.
const char* argp_program_version = "fcomment 1.0";
const char* argp_program_bug_address = "<curoles@xxxx.org>";

// Program documentation.
static char ls_argp_doc[] =
    "List files and show comments";

static struct argp ls_argp = { 0, 0, 0, ls_argp_doc };

int fcomment_ls(int argc, char* argv[])
{
    argp_parse(&ls_argp, argc, argv, 0, 0, 0);

    return EXIT_SUCCESS;
}
