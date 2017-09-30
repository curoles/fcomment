/**
 * @file
 * @brief     Create a new empty file and save some info about its origin.
 * @author    Igor Lesik 2017
 * @copyright MIT license.
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <argp.h>
#include <sys/xattr.h>

#include "fcomment.h"

// Program documentation.
static char doc[] =
    "Create new empty file and save information about its origin.";

// A description of the arguments "set" command accepts.
static char args_doc[] = "FILE";

// The options "set" command understands.
static struct argp_option options[] = {
    {"verbose",  'v', 0,      0,  "Produce verbose output" },
    { 0 }
};

// Used to communicate with parse_opt.
struct Arguments
{
    char* args[1];  /* file */
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
        /*case 'o':
            arguments->output_file = arg;
            break;*/

        case ARGP_KEY_ARG:
            if (state->arg_num >= 1) {
                /* Too many arguments. */
                argp_usage (state);
            }
            arguments->args[state->arg_num] = arg;

            break;

        case ARGP_KEY_END:
            if (state->arg_num < 1) {
                /* Not enough arguments. */
                argp_usage (state);
            }
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

// Argp parser.
static struct argp argParser = { options, parse_opt, args_doc, doc };


int fcomment_touch(int argc, char* argv[])
{
    struct Arguments arguments = {
        .verbose = false
    };

    argp_parse(&argParser, argc, argv, 0, 0, &arguments);

    const char* path = arguments.args[0];

    /*if (!File_exist(path)) {
        printf("Error: file %s does not exist!", path);
        return EXIT_FAILURE;
    }*/

    bool touched = FComment_touchFile(
       path,
       arguments.verbose
    );

    return (touched)? EXIT_SUCCESS : EXIT_FAILURE;
}
