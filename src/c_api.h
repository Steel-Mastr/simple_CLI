/*
 * Simple_CLI - API C
 *
 * Livello C per la libreria Simple_CLI: questo header e' leggibile da un
 * compilatore C puro e si puo' includere da file .c.
 *
 * L'implementazione e' in c_api.cpp (C++): e' l'unico sorgente da compilare
 * insieme al progetto consumatore. Nei progetti MSBuild viene aggiunto
 * automaticamente da Simple_CLI.targets; con CMake o build manuali basta
 * aggiungerlo alla build e mettere la directory di include nel path.
 *
 * Regole di memoria:
 *  - gli handle restituiti da simple_cli_*_create vanno liberati con la
 *    *_destroy corrispondente;
 *  - le stringhe RESTITUITE dall'API sono allocate con malloc: liberarle
 *    con simple_cli_free (mai con free su memoria di altre funzioni);
 *  - le stringhe PASSATE all'API sono di sola lettura (copiate internamente);
 *  - le eccezioni C++ non attraversano mai il confine: le funzioni di
 *    creazione ritornano NULL in caso di errore.
 */
#ifndef SIMPLE_CLI_C_API_H
#define SIMPLE_CLI_C_API_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Handle opachi: tipi distinti tra loro, il compilatore C segnala gli scambi */
typedef struct simple_cli_schermata_tag* simple_cli_schermata;
typedef struct simple_cli_selettore_tag* simple_cli_selettore;

/* ---- Setup ---- */

/* Solo Unix/Linux/macOS: abilita la raw mode del terminale. Su Windows e' un no-op. */
void simple_cli_setup(void);

/* ---- Schermata ---- */

/* Crea una schermata. contenuto e' una stringa con le righe separate da '\n'.
 * Ritorna NULL in caso di errore (contenuto NULL o memoria esaurita). */
simple_cli_schermata simple_cli_schermata_create(const char* contenuto, int bordi);

/* Distrugge la schermata creata con simple_cli_schermata_create. */
void simple_cli_schermata_destroy(simple_cli_schermata s);

/* Stampa a schermo; se del != 0 sovrascrive il render precedente. */
void simple_cli_schermata_print(simple_cli_schermata s, int del);

/* Aggiorna il contenuto (righe separate da '\n'), con o senza bordi. */
void simple_cli_schermata_aggiorna(simple_cli_schermata s, const char* contenuto, int bordi);

/* Ritorna il contenuto come stringa (righe separate da '\n'), allocata con
 * malloc: liberare con simple_cli_free. Ritorna NULL in caso di errore. */
char* simple_cli_schermata_get_string(simple_cli_schermata s);

/* ---- Selettore ---- */

/* Crea un selettore senza renderizzarlo. opzioni e' un array di
 * opzioni_count stringhe C (copiate internamente). Ritorna NULL in caso di errore. */
simple_cli_selettore simple_cli_selettore_create(const char* titolo,
    const char* const* opzioni, size_t opzioni_count, int bordi);

/* Mostra la schermata e attende l'input dell'utente (interattiva).
 * Ritorna l'indice dell'opzione selezionata, oppure -1 se si preme Esc. */
int simple_cli_selettore_render(simple_cli_selettore s);

/* Distrugge il selettore creato con simple_cli_selettore_create. */
void simple_cli_selettore_destroy(simple_cli_selettore s);

/* ---- Memoria ---- */

/* Libera ogni char* restituito dall'API C (accetta anche NULL). */
void simple_cli_free(void* p);

#ifdef __cplusplus
}
#endif

#endif /* SIMPLE_CLI_C_API_H */
