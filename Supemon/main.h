#ifndef MAIN_H
#define MAIN_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <locale.h>

void setup_utf8_console(void);

static void flush_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

#endif // MAIN_H
