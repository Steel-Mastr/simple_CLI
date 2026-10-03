#pragma once
#include <array>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

#ifdef _WIN32
    constexpr bool isWindows = true;
#else
    constexpr bool isWindows = false;
#endif

#ifdef _WIN32
    #include <conio.h>
    inline void enableRawMode() {}
#else
    #include <termios.h>
    #include <unistd.h>

    // inline: la libreria è header-only, questa variabile deve avere linkage interno
    // (una sola istanza anche con inclusioni multiple)
    inline termios _originalTermios;

    inline void enableRawMode() {
        tcgetattr(STDIN_FILENO, &_originalTermios);
        termios raw = _originalTermios;
        raw.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
        atexit([]() {
            tcsetattr(STDIN_FILENO, TCSANOW, &_originalTermios);
        });
    }
#endif

inline int GETCH() {
    if constexpr (isWindows) {
        return _getch();
    } else {
        return getchar();
    }
}

inline void SETUP() {
    enableRawMode();
}