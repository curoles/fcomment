/**
 * @file
 * @brief     Information about terminal.
 * @author    Igor Lesik 2017
 * @copyright MIT license.
 */
#pragma once

#include <string.h>
#include <stdbool.h>

typedef struct Terminal {

    size_t rows;
    size_t cols;

    bool isAtty;

} Terminal;

Terminal* Terminal_getInstance();
void Terminal_update(Terminal* term);
void Terminal_init();

