/**
 * @file
 * @brief     Set file comment.
 * @author    Igor Lesik 2017
 * @copyright MIT license.
 */
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <argp.h>
#include <sys/xattr.h>
#include <libgen.h>
#include <assert.h>

#include "file.h"

// Program documentation.
static char doc[] =
    "Attach a comment to a file.";

// A description of the arguments "set" command accepts.
static char args_doc[] = "FILE COMMENT";

// The options "set" command understands.
static struct argp_option options[] = {
    {"verbose",  'v', 0,      0,  "Produce verbose output" },
    { 0 }
};

// Used to communicate with parse_opt.
struct Arguments
{
    char* args[2];  /* file & comment */
    bool verbose;
    //char *output_file;
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
            if (state->arg_num >= 2) {
                /* Too many arguments. */
                argp_usage (state);
            }
            arguments->args[state->arg_num] = arg;

            break;

        case ARGP_KEY_END:
            if (state->arg_num < 2) {
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

static bool setCommentInHiddenFile(
    const char* path,
    const char* comment,
    bool verbose
)
{
    char* path_dir_copy = strdup(path);
    char* path_file_copy = strdup(path);

    char* file_name = basename(path_file_copy);
    char* dir_name = dirname(path_dir_copy);


    char* hidden_dir = NULL;
    int hidden_dir_name_size = asprintf(&hidden_dir, "%s/.meta-fcomment", dir_name);

    assert(hidden_dir_name_size > 0);

    char* meta_dir = NULL;
    int meta_dir_name_size = asprintf(&meta_dir, "%s/%s", hidden_dir, file_name);

    assert(meta_dir_name_size > 0);

    if (verbose) {printf("Hidden directory is %s\n", meta_dir);}

    if (!File_exist(hidden_dir)) {
        int hidden_dir_created = Dir_make(hidden_dir);
        if (0 != hidden_dir_created) {perror("Can't created hidden directory");}
    }

    if (File_exist(hidden_dir) && !File_exist(meta_dir)) {
        int meta_dir_created = Dir_make(meta_dir);
        if (0 != meta_dir_created) {perror("Can't created hidden directory");}
    }

    if (File_exist(meta_dir)) {
        //TODO make file name asprintf(&comment_file_name, "%s/comment.txt", meta_dir);
        //open(comment_file_name)
        //write comment
        //close(fd)
        //free(comment_file_name);
    }

    free(meta_dir);
    free(hidden_dir);
    free(path_dir_copy);
    free(path_file_copy);

    return true;
}

int fcomment_set(int argc, char* argv[])
{
    struct Arguments arguments = {
        .verbose = false
    };

    argp_parse(&argParser, argc, argv, 0, 0, &arguments);

    const char* path = arguments.args[0];
    const char* comment = arguments.args[1];

    if (arguments.verbose) {
        printf("PATH=%s COMMENT=%s\n",
            path, comment);
    }

    if (!File_exist(path)) {
        printf("Error: file %s does not exist!", path);
        return EXIT_FAILURE;
    }

    int result = File_setXAttrStr(
        path,
        "user.comment",
        comment
    );

    if (0 != result) {
        if (arguments.verbose) {
            perror("Failed to set extended file attribute");
        }
        setCommentInHiddenFile(path, comment, arguments.verbose);
    }

    return EXIT_SUCCESS;
}
