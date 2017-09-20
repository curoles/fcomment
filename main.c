/**
 * @file
 * @author Igor Lesik 2017
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include <libgen.h>

typedef enum CmdId {
    CMD_GENERIC = 0,
    CMD_LIST,
    CMD__END,
    CMD__SIZE = CMD__END
} CmdId;

static const char* progName[CMD__SIZE] = {
    "fcomment",
    "fcomment-ls",
};

static bool isPrgCmd(const char* program_name, CmdId cmdId) {
    assert (cmdId >=0 && cmdId < CMD__END);
    return 0 == strcmp(program_name, progName[cmdId]);
}

int fcomment_ls(int argc, char* argv[]);

typedef int (*ProgramFunction)(int argc, char* argv[]);

static ProgramFunction progs[CMD__SIZE] = {
    fcomment_ls,
    fcomment_ls
};

static CmdId detectCommandByProgName(const char* program_name)
{
    CmdId cmdId = CMD__END;

    if (program_name == NULL)                     {cmdId = CMD__END;}
    else if (isPrgCmd(program_name, CMD_GENERIC)) {cmdId = CMD_GENERIC;}
    else if (isPrgCmd(program_name, CMD_LIST))    {cmdId = CMD_LIST;}
    else                                          {cmdId = CMD__END;}

    return cmdId;
}

int main(int argc, char* argv[])
{
    assert (argc > 0);
    const char* program_name = basename(argv[0]);

    CmdId cmdId = detectCommandByProgName(program_name);

    if (cmdId == CMD__END) {
        printf("Error: unknown command \"%s\"\n", program_name);
        printf("Legal names:\n");
        for (int i = 0; i < CMD__SIZE; i++) {printf("  %s\n", progName[i]);}
        return EXIT_FAILURE;
    }

    if (cmdId == CMD_GENERIC) {
        cmdId = CMD_LIST;
        if (argc > 1) {
            const char* cmd = argv[1];
            if (0 == strcmp(cmd, "--help")) {
                printf("Help!\n");
                return EXIT_SUCCESS;
            }
            else if (0 == strcmp(cmd, "ls") == 0 || strcmp(cmd, "list") == 0) {
                cmdId = CMD_LIST; argv[0] = "list"; argv[1] = "";
            }
        }
    }

    ProgramFunction prog = progs[cmdId];

    int result = prog(argc, argv);

    return result;
}
