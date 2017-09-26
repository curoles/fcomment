/**
 * @file
 * @brief     Print Markdown file to terminal.
 * @author    Igor Lesik 2017
 * @copyright MIT license.
 */
#include "markdown.h"

#include <stdbool.h>

struct MarkdownParserState
{
    bool new_line;
    bool end_of_line;
    bool header;
    int strong; int strong_dir;
};

const char
    RESET[]     = "\033[0m",
    BOLD[]      = "\033[1m", NO_BOLD[]      = "\033[21m",
    FAINT[]     = "\033[2m", NO_FAINT[]     = "\033[22m",
    ITALIC[]    = "\033[3m", NO_ITALIC[]    = "\033[23m",
    UNDERLINE[] = "\033[4m", NO_UNDERLINE[] = "\033[24m",
    NEGATIVE[]  = "\033[7m", POSTIVE[]      = "\033[7m",

    //FRAME[] = "\033[51m",
    //ENCIRCLE[] = "\033[52m",

    BLACK[]  = "\033[30m", GRAY[]    = "\033[1;30m",
    CHERRY[] = "\033[31m", RED[]     = "\033[1;31m",
    GREEN[]  = "\033[32m", SALAD[]   = "\033[1;32m",
    BROWN[]  = "\033[33m", YELLOW[]  = "\033[1;33m",
    INDIGO[] = "\033[34m", BLUE[]    = "\033[1;34m",
    PURPLE[] = "\033[35m", MAGENTA[] = "\033[1;35m",
    TEAL[]   = "\033[36m", CYAN[]    = "\033[1;36m",
    SILVER[] = "\033[37m", WHITE[]   = "\033[1;37m"
;

void Markdown_printTerminalColors(FILE* fout)
{
#define pC(color) fprintf(fout, "%s%8s", color, #color);
    pC(BLACK); pC(CHERRY); pC(GREEN); pC(BROWN); pC(INDIGO); pC(PURPLE); pC(TEAL); pC(SILVER); fputs("\n", fout);
printf(FAINT);
    pC(BLACK); pC(CHERRY); pC(GREEN); pC(BROWN); pC(INDIGO); pC(PURPLE); pC(TEAL); pC(SILVER); fputs("\n", fout);
printf(NO_FAINT);
    pC(GRAY);  pC(RED);    pC(SALAD); pC(YELLOW); pC(BLUE); pC(MAGENTA); pC(CYAN); pC(WHITE); fputs("", fout);
    fputs(RESET, fout); fputs("\n", fout);
printf(NEGATIVE);
    pC(BLACK); pC(CHERRY); pC(GREEN); pC(BROWN); pC(INDIGO); pC(PURPLE); pC(TEAL); pC(SILVER); fputs("\n", fout);
printf(FAINT);
    pC(BLACK); pC(CHERRY); pC(GREEN); pC(BROWN); pC(INDIGO); pC(PURPLE); pC(TEAL); pC(SILVER); fputs("\n", fout);
printf(NO_FAINT);
    pC(GRAY);  pC(RED);    pC(SALAD); pC(YELLOW); pC(BLUE); pC(MAGENTA); pC(CYAN); pC(WHITE); fputs("", fout);
    fputs(RESET, fout); fputs("\n", fout);
#undef pC
}

void
Markdown_printColorTerminal(
    const char* fin_name,
    FILE* fout
)
{
    //Markdown_printTerminalColors(fout);

    FILE* fin = fopen(fin_name, "r");

    struct MarkdownParserState state;
    state.new_line = true;
    state.end_of_line = false;
    state.header = false;
    state.strong = 0; state.strong_dir = 1;

    char c;
    char pc = '\n';

#define ps(s) fputs(s, fout)

    while ((c=fgetc(fin)) != EOF)
    {
        if (c == '\n') {state.end_of_line = true;}

        if (state.new_line) {
            if (c == '#') {state.header = true; fputs(RED, fout); fputs(UNDERLINE, fout);}

            state.new_line = state.end_of_line;
        }

        if (c == '_' /*&& !verbatim*/) {
            if (state.strong) {
                if (pc == '_') {
                    if (state.strong_dir > 0) { if (state.strong == 1) {ps(NO_ITALIC); ps(BOLD);}}
                    state.strong += state.strong_dir;
                }
                else {state.strong_dir = -1; state.strong -= 1;}

                if (state.strong == 0){state.strong_dir = 1; ps(NO_ITALIC); ps(NO_BOLD);}
            }
            else {
                state.strong_dir = 1; state.strong += state.strong_dir; ps(ITALIC);
            }
        }

        if (state.end_of_line) {
            if (state.header) {state.header = false; fputs(RESET, fout);}
        }

        fputc(c, fout);

        pc = c;
    }

    fputs(RESET, fout);

#undef ps

    fclose(fin);
}
