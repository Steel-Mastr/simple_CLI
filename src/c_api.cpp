/*
 * Simple_CLI - Implementazione (bridge) dell'API C
 *
 * Compilare come C++ (e' l'unico sorgente C++ che un progetto C deve
 * aggiungere alla propria build) e linkare insieme agli oggetti C:
 *
 *   gcc -c main.c -I<include>
 *   g++ -std=c++20 -c c_api.cpp -I<include>
 *   g++ main.o c_api.o -o myapp
 *
 * Nei progetti MSBuild questo file viene aggiunto automaticamente da
 * Simple_CLI.targets. Con CMake: target_sources(... c_api.cpp).
 */
#include "c_api.h"

#include <cstdlib>
#include <cstring>
#include <new>
#include <string>
#include <vector>

#include "Simple_CLI.h"

namespace {

    /* Copia una std::string in un buffer malloc'd: tutta la memoria che
     * attraversa il confine C e' allocata con malloc (liberabile con
     * simple_cli_free), mai con new[]. */
    char* allocString(const std::string& s) {
        const size_t n = s.size() + 1;
        char* out = static_cast<char*>(std::malloc(n));
        if (out != nullptr)
            std::memcpy(out, s.c_str(), n);
        return out;
    }

} // namespace

extern "C" {

/* ---- Setup ---- */

void simple_cli_setup(void) {
    SETUP();
}

/* ---- Schermata ---- */

simple_cli_schermata simple_cli_schermata_create(const char* contenuto, int bordi) {
    if (contenuto == nullptr)
        return nullptr;
    try {
        return reinterpret_cast<simple_cli_schermata>(
            new schermate::Schermata(std::string(contenuto), bordi != 0));
    } catch (...) {
        return nullptr;
    }
}

void simple_cli_schermata_destroy(simple_cli_schermata s) {
    delete reinterpret_cast<schermate::Schermata*>(s);
}

void simple_cli_schermata_print(simple_cli_schermata s, int del) {
    if (s == nullptr)
        return;
    reinterpret_cast<schermate::Schermata*>(s)->print(del != 0);
}

void simple_cli_schermata_aggiorna(simple_cli_schermata s, const char* contenuto, int bordi) {
    if (s == nullptr || contenuto == nullptr)
        return;
    try {
        reinterpret_cast<schermate::Schermata*>(s)->aggiorna(std::string(contenuto), bordi != 0);
    } catch (...) {
        /* in caso di errore la schermata resta com'era */
    }
}

char* simple_cli_schermata_get_string(simple_cli_schermata s) {
    if (s == nullptr)
        return nullptr;
    try {
        return allocString(reinterpret_cast<schermate::Schermata*>(s)->getString());
    } catch (...) {
        return nullptr;
    }
}

/* ---- Selettore ---- */

simple_cli_selettore simple_cli_selettore_create(const char* titolo,
    const char* const* opzioni, size_t opzioni_count, int bordi)
{
    if (titolo == nullptr || (opzioni == nullptr && opzioni_count > 0))
        return nullptr;
    try {
        std::vector<std::string> opz;
        opz.reserve(opzioni_count);
        for (size_t i = 0; i < opzioni_count; i++)
            opz.emplace_back(opzioni[i] != nullptr ? opzioni[i] : "");
        return reinterpret_cast<simple_cli_selettore>(
            new schermate::SchermataSelettore(std::string(titolo), opz, bordi != 0));
    } catch (...) {
        return nullptr;
    }
}

int simple_cli_selettore_render(simple_cli_selettore s) {
    if (s == nullptr)
        return -1;
    return reinterpret_cast<schermate::SchermataSelettore*>(s)->render();
}

void simple_cli_selettore_destroy(simple_cli_selettore s) {
    delete reinterpret_cast<schermate::SchermataSelettore*>(s);
}

/* ---- Memoria ---- */

void simple_cli_free(void* p) {
    std::free(p);
}

} // extern "C"
