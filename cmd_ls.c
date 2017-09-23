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
#include "mfile.h"

// Globals for Argp, version and email.
const char* argp_program_version = "fcomment 1.0";
const char* argp_program_bug_address = "<curoles@xxxx.org>";

// Program documentation.
static char ls_argp_doc[] =
    "List files and show comments.";

// A description of the arguments "ls" command accepts.
static char args_doc[] = "PATH";

// The options "ls" command understands.
static struct argp_option options[] = {
    {"verbose",  'v', 0,      0,  "Produce verbose output" },
    { 0 }
};

// Used to communicate with parse_opt.
struct Arguments
{
    char *args[1];  /* path */
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
            if (state->arg_num >= 1) {
                /* Too many arguments. */
                argp_usage (state);
            }
            arguments->args[state->arg_num] = arg;

            break;

        case ARGP_KEY_END:
            if (state->arg_num < 1) {
                /* Not enough arguments. Set default path = "." */
                //argp_usage (state);
                arguments->args[0] = ".";
            }
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

// Argp parser.
static struct argp lsArgParser = { options, parse_opt, args_doc, ls_argp_doc };

//@return true if comment found
static bool getXAComment(
    const char* path,
    char* comment_buf,
    size_t comment_buf_size
)
{
    ssize_t result = File_getXAttrStr(
        path,
        "user.comment",
        comment_buf,
        comment_buf_size
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

    return found_comment;
}

//@return true if comment found
static bool getComment(
    const char* path,
    char* comment_buf,
    size_t comment_buf_size
)
{
    bool found_comment = getXAComment(path, comment_buf, comment_buf_size);

    if (!found_comment) {
        ssize_t bytes_read = MFile_getComment(path, comment_buf, comment_buf_size);
        if (bytes_read > 0) {found_comment = true;}
        //else {printf("read %zd bytes\n", bytes_read);}
    }

    return found_comment;
}

static void dir_visitor(const char* base, const char* filename)
{
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", base, filename);

    char buf[1024];
    bool found_comment = getComment(path, buf, sizeof(buf));

    //printf("%-*s - %s\n", 16, path, buf);
    printf("%s", filename);
    if (found_comment) {printf(" - %s", buf);}
    printf("\n");      
}

void listDirWithComments(const char* path)
{
    Dir_visitAlphaOrder(path, dir_visitor);

}

void showFileComment(const char* path)
{
    char buf[1024];
    bool found_comment = getComment(path, buf, sizeof(buf));

    if (found_comment) {printf("%s\n", buf);}
}

int fcomment_ls(int argc, char* argv[])
{
    struct Arguments arguments = {
        .verbose = false
    };

    argp_parse(&lsArgParser, argc, argv, 0, 0, &arguments);

    const char* path = arguments.args[0];

    if (!File_exist(path)) {
        printf("File %s does not exist\n", path);
        return EXIT_FAILURE;
    }

    if (File_isDirectory(path)) {
        if (arguments.verbose) {printf("Listing directory %s\n", path);}
        listDirWithComments(path);
    }
    else if (File_isRegularFile(path)) {
        showFileComment(path);
    }

    return EXIT_SUCCESS;
}
