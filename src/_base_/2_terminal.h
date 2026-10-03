#pragma once
#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include "1_prep.h"

namespace terminale {
    enum Key { UP = 256, DOWN, LEFT, RIGHT, ENTER, ESC, BACKSPACE, DEL, CTRLC };

    inline std::string CURSOR_UP(const unsigned int n) {
        return "\033[" + std::to_string(n) + "A";
    }
    inline std::string CURSOR_DOWN(const unsigned int n) {
        return "\033[" + std::to_string(n) + "B";
    }
    inline constexpr std::string_view CLEAR_LINE = "\033[2K";
    inline constexpr std::string_view DELETE_LINE = "\033[M";
    inline constexpr std::string_view DELETE_LOWER = "\033[J";

    inline int readKey() {
        const int c = GETCH();
        if constexpr (isWindows) {
            if (c == 224 || c == 0) {
                switch (GETCH()) {
                    case 72: return UP;
                    case 80: return DOWN;
                    case 75: return LEFT;
                    case 77: return RIGHT;
                    case 83: return DEL;   // tasto Canc
                    default:
                        return c;
                }
            }
        }
        else {
            if (c == 27) {
                const int c2 = GETCH();
                if (c2 == '[') {
                    switch (GETCH()) {
                        case 'A': return UP;
                        case 'B': return DOWN;
                        case 'C': return RIGHT;
                        case 'D': return LEFT;
                        case '3': {          // tasto Canc (sequenza ESC [ 3 ~)
                            GETCH();         // consuma '~'
                            return DEL;
                        }
                    }
                }
                return ESC;
            }
        }
        if (c == '\r' || c == '\n') return ENTER;
        if (c == 3)   return CTRLC;
        if (c == 27)  return ESC;
        if (c == 8 || c == 127) return BACKSPACE;
        return c;
    }
}

namespace schermate {
    inline unsigned int righeStampate = 0;

    // Si sostituisce a cout
    inline void print(const std::string& contenuto) {
        std::cout << contenuto << std::flush;
        righeStampate += std::count(contenuto.begin(), contenuto.end(), '\n');
    }
    // Si sostituisce a cout (ed inizia in una nuova riga)
    inline void println(const std::string& contenuto) {
        std::cout << '\n' << contenuto << std::flush;
        righeStampate += std::count(contenuto.begin(), contenuto.end(), '\n') + 1;
    }

    // Si sostituisce a cin
    inline std::string read() {
        ++righeStampate;
        std::string contenuto;
        std::cin >> contenuto;
        return contenuto;
    }

    // Si sostituisce a cin (ed inizia in una nuova riga)
    inline std::string readln() {
        righeStampate += 2;
        std::string contenuto;
        std::cout << '\n';
        std::cin >> contenuto;
        return contenuto;
    }

    // Cancella n righe
    inline void del(const unsigned int count) {
        const unsigned int toDel = std::min(righeStampate, count);
        if (toDel == 0) [[unlikely]]
            return;
        std::cout << terminale::CURSOR_UP(toDel)
            << '\r' << terminale::DELETE_LOWER << std::flush;
        righeStampate -= toDel;
    }
    // Cancella tutto
    inline void clear() {
        if (righeStampate > 0) [[likely]]
            std::cout << terminale::CURSOR_UP(righeStampate)
                << '\r' << terminale::DELETE_LOWER << std::flush;
        righeStampate = 0;
    }
}