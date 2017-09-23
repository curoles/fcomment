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
};

void
Markdown_print(
    const char* fin_name,
    FILE* fout
)
{
    FILE* fin = fopen(fin_name, "r");

    struct MarkdownParserState state;
    state.new_line = true;
    state.end_of_line = false;
    state.header = false;

    char c;

    while ((c=fgetc(fin)) != EOF) {
        if (c == '\n') state.end_of_line = true;

        if (state.new_line) {
            if (c == '#') {state.header = true; fputs("\033[4;31m", fout); }

            state.new_line = state.end_of_line;
        }

        if (state.end_of_line) {
            state.header = false; fputs("\033[0m", fout);
        }

        fputc(c, fout);
    }

    fclose(fin);
}
