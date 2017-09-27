/**
 * @file
 * @brief     Copy file comment.
 * @author    Igor Lesik 2017
 * @copyright MIT license.
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <argp.h>
#include <sys/xattr.h>

#include "file.h"
#include "mfile.h"
#include "fcomment.h"
#include "markdown.h"
#include "terminal.h"


// Program documentation.
static char cp_argp_doc[] =
    "Copy file comment.";

// A description of the arguments "cp" command accepts.
static char args_doc[] = "SRC DEST";

// The options "cps" command understands.
static struct argp_option options[] = {
    {"verbose",  'v', 0,      0,  "Produce verbose output" },
    { 0 }
};

// Used to communicate with parse_opt.
struct Arguments
{
    char *args[2];  /* src & dest */
    bool verbose;
};

/* Parse a single option.
 */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
    /* Get the input argument from argp_parse, which we
       know is a pointer to our arguments structure. */
    struct Arguments *arguments = state->input;

    switch (key)
    {
        case 'v':
             arguments->verbose = true;
            break;

        case ARGP_KEY_ARG:
            if (state->arg_num >= 2) {
                /* Too many arguments. */
                argp_usage (state);
            }
            arguments->args[state->arg_num] = arg;

            break;

        case ARGP_KEY_END:
            if (state->arg_num < 2) {
                /* Not enough arguments. Set default path = "." */
                argp_usage (state);
            }
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

// Argp parser.
static struct argp cpArgParser = { options, parse_opt, args_doc, cp_argp_doc };



int fcomment_cp(int argc, char* argv[])
{
    struct Arguments arguments = {
        .verbose = false
    };

    argp_parse(&cpArgParser, argc, argv, 0, 0, &arguments);

    const char* src = arguments.args[0];

    if (!File_exist(src)) {
        printf("File %s does not exist\n", src);
        return EXIT_FAILURE;
    }

    const char* dest = arguments.args[1];

    bool comment_copied = FComment_copy(src, dest);

    return (comment_copied)? EXIT_SUCCESS:EXIT_FAILURE;
}
