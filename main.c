/**
 * @file
 * @brief     fcomment main entry.
 * @author    Igor Lesik 2017
 * @copyright MIT license.
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
    CMD_SET,
    CMD__END,
    CMD__SIZE = CMD__END
} CmdId;

static const char* progName[CMD__SIZE] = {
    "fcomment",
    "fcomment-ls",
    "fcomment-set"
};

static bool isPrgCmd(const char* program_name, CmdId cmdId) {
    assert (cmdId >=0 && cmdId < CMD__END);
    return 0 == strcmp(program_name, progName[cmdId]);
}

int fcomment_ls(int argc, char* argv[]);
int fcomment_set(int argc, char* argv[]);

typedef int (*ProgramFunction)(int argc, char* argv[]);

static ProgramFunction progs[CMD__SIZE] = {
    fcomment_ls,
    fcomment_ls,
    fcomment_set
};

static CmdId detectCommandByProgName(const char* program_name)
{
    CmdId cmdId = CMD__END;

    if (program_name == NULL)                     {cmdId = CMD__END;}
    else if (isPrgCmd(program_name, CMD_GENERIC)) {cmdId = CMD_GENERIC;}
    else if (isPrgCmd(program_name, CMD_LIST))    {cmdId = CMD_LIST;}
    else if (isPrgCmd(program_name, CMD_SET))     {cmdId = CMD_SET;}
    else                                          {cmdId = CMD__END;}

    return cmdId;
}

static CmdId detectCommandBy2ndArg(const char* cmd)
{
    CmdId cmdId = CMD__END;

    if (cmd == NULL)                              {cmdId = CMD__END;}
    else if (0 == strcmp(cmd, "ls") || strcmp(cmd, "list") == 0) {cmdId = CMD_LIST;}
    else if (0 == strcmp(cmd, "set"))             {cmdId = CMD_SET;}
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

    CmdId cmdId2 = CMD__END;

    if (cmdId == CMD_GENERIC && argc > 1) {
        const char* cmd = argv[1];
        if (0 == strcmp(cmd, "--help")) {
            printf("Help!\n");
            return EXIT_SUCCESS;
        }
        else {
            cmdId2 = detectCommandBy2ndArg(cmd);
        }
    }

    if (cmdId == CMD_GENERIC) {
        if (cmdId2 == CMD__END) { cmdId = CMD_LIST; }
        else { cmdId = cmdId2; argv++; argc--; }
    }

    ProgramFunction prog = progs[cmdId];

    int result = prog(argc, argv);

    return result;
}
