/**
 * @file
 * @brief     Information about terminal.
 * @author    Igor Lesik 2017
 * @copyright MIT license.
 */
#include "terminal.h"

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>


static Terminal terminal_instance = {
    .rows = 0,
    .cols = 0,
    .isAtty = false
};

Terminal* Terminal_getInstance() {
    return &terminal_instance;
}

void Terminal_update(Terminal* term)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    term->rows = w.ws_row;
    term->cols = w.ws_col;

    term->isAtty = isatty(fileno(stdout));
}

void Terminal_init()
{
    Terminal_update(&terminal_instance);

    //printf("Terminal size is %zu x %zu, tty=%d \n",
    //    terminal_instance.cols, terminal_instance.rows,
    //    terminal_instance.isAtty);
}
