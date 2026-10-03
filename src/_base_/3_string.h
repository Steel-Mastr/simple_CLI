#pragma once
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include "2_terminal.h"

namespace utils {
    // Gestisce il rendering ripetuto di una stessa stringa
    inline std::string ripeti(const unsigned int rep, const std::string& s) {
        std::string out;
        out.reserve(s.size() * rep);
        for (int i = 0; i < static_cast<int>(rep); i++)
            out += s;
        return out;
    }

    // Restituisce la versione toLowerCase della stringa s
    inline std::string toLower(const std::string& s) {
        std::string out;
        for (const char c : s)
            out += static_cast<char>(tolower(c));
        return out;
    }

    // Nelle schermate di selezione sostituisce selezionato++
    inline void addSelezionato(int& selezionato, const int size) {
        if (selezionato >= size - 1) return;
        selezionato++;
    }

    // Nelle schermate di selezione sostituisce selezionato--
    inline void remSelezionato(int& selezionato) {
        if (selezionato <= 0) return;
        selezionato--;
    }

    // Nelle schermate a scorrimento sostituisce shift++
    inline void addShift(int& shift, const int selezionato, const int size, const int listSize) {
        if (shift + size > listSize) return;        //  outOfList
        if (selezionato <= shift + 1) return;       //  notToMove
        shift++;
    }

    // Nelle schermate a scorrimento sostituisce shift--
    inline void remShift(int& shift, const int selezionato) {
        if (selezionato >= shift + 1 || shift <= 0) return; //  notToMove / già in cima
        shift--;
    }

    // Trasforma un vettore di stringhe in una singola stringa con /n
    inline std::string parseString(const std::vector<std::string>& contenuto) {
        std::string out;
        unsigned int total = 0;
        for (const std::string& s : contenuto)
            total += s.length() + 1;
        out.reserve(total);
        for (const std::string& s : contenuto)
            out += s + '\n';
        return out;
    }

    // Trasforma una stringa in un vettore
    inline std::vector<std::string> parseStringVector(const std::string& stringa) {
        std::vector<std::string> out;
        std::string current;
        for (const char& c : stringa) {
            if (c != '\n') {
                current += c;
                continue;
            }
            out.emplace_back(current);
            current.clear();
        }
        if (!current.empty())
            out.emplace_back(current);
        return out;
    }

    // Massima dimensione del vettore
    inline unsigned int getMaxSize(const std::vector<std::string>& contenuto) {
        unsigned int maxSize = 0;
        for (const std::string& s : contenuto)
            maxSize = std::max(maxSize, static_cast<unsigned int>(s.length()));
        return maxSize;
    }
}
