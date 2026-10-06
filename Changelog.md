# Changelog

---

## *- Versione 1.7.1 (patch):*

Aggiunta dell'API a git

##### Patch:
- `src/c_api.h`: file aggiunto a git
- `src/c_api.cpp`: file aggiunto a git

---

## *> Versione 1.7.0 (major):*

Supporto a **C puro**: nuovo livello API compilabile da progetti C.

##### Aggiunte:
- `src/c_api.h`: header C compatibile con handle opachi (`simple_cli_schermata`,
  `simple_cli_selettore`) e solo tipi C nell'interfaccia; nel pacchetto è alla radice degli
  include (`#include "c_api.h"`)
- `src/c_api.cpp`: bridge che implementa l'API con le classi C++ interne — compilato
  automaticamente nei progetti MSBuild tramite `Simple_CLI.targets`, con CMake tramite
  `-DSIMPLE_CLI_BUILD_C_API=ON` (target `simple_cli_c_api`)
- Funzioni: `simple_cli_setup`, `simple_cli_schermata_create/print/aggiorna/get_string/destroy`,
  `simple_cli_selettore_create/render/destroy`, `simple_cli_free`
- Le stringhe restituite sono allocate con `malloc` (liberabili con `simple_cli_free`) e le
  eccezioni C++ non attraversano mai il confine: le create ritornano `NULL` in caso di errore
- `c_api_test.c`: test in C11 compilato con gcc come C puro e linkato al bridge

##### Rimozioni:
- `src/c_include.h` (API C-style, solo C++): superata da `c_api.h`/`c_api.cpp` per il C puro e
  dalle classi C++ (`Simple_CLI.h`) per tutto il resto

---

## *- Versione 1.6.5 (patch):*

Sistemazione dell'API C-style (`src/c_include.h`) e fix minori.

##### Aggiunte:
- Funzioni di liberazione memoria: `utils::freeCharArray`, `utils::freeString`,
  `utils::freeCStringVector`, `destroy_c_formElement`
- Helper `utils::c_string(const char*)` per creare una `cString` da una stringa C

##### Patch:
- `utils::getVector` e la creazione di selettori/questionario da handle raddoppiavano gli
  elementi (pre-allocazione + `emplace_back`): i selettori C-style ora mostrano le opzioni giuste
  e gli indici restituiti da `render` sono corretti
- I `destroy_c_*` invocavano il distruttore sull'indirizzo dell'handle (stack) invece che
  sull'oggetto allocato, senza deallocare la memoria: ora fanno `delete` sull'oggetto
- `getUnderlyingSchermata*` reinterpretavano i primi byte dell'oggetto come handle: ora
  restituiscono l'handle della parte base della schermata (non posseduto, non va liberato)
- Sezione selettore filtrata: handle e tipi di ritorno corretti (per copy-paste erano quelli
  del selettore semplice/large)
- `setContentStringVector` ora ricalcola `maxSize`
- Rimosso il blocco `extern "C"` inerte (typo `_cplusplus`): l'API è in stile C ma C++-only
- `SchermataSelettoreCustom::getResult()` non accede più fuori dai limiti se non c'è risultato
- Cast `static_cast<int>` sui confronti `int`/`size_t` nelle schermate (warning -Wsign-compare)
- `Test.cpp`: `SETUP;` (no-op) → `SETUP();`, puntatori nel costruttore del questionario
- README aggiornato: esempi corretti e nuova sezione sull'API C-style

---

## *- Versione 1.6.4 (patch):*

La libreria è ora completamente header-only (rimosso `Simple_CLI.cpp` e la `.lib` dal pacchetto NuGet).

##### Aggiunte:
- File `Simple_CLI.targets` nel pacchetto NuGet: include gli header automaticamente nei progetti MSBuild
- Supporto del tasto `Canc` (`DEL`) in `readKey` su Windows e Unix

##### Patch:
- Corretti errori di link multi-definizione (ODR) includendo la libreria da più file `.cpp`
  (`_originalTermios` ora è `inline`)
- Aggiunti gli include standard mancanti (`<cstdlib>`, `<algorithm>`, `<stdexcept>`, ecc.) nei vari header
- Corretto potenziale crash/underflow del selettore filtrato e large con liste vuote o corte
  (confronti `size_t`/`int`)
- `goto PIPPO` salta l'inizializzazione di variabili in `Test.cpp`: sostituito con un loop `while`

---

## *- Versione 1.6 (major):*

Sono stati risolti problemi di compatibilità con Linux (supporto Unix/Linux/macOS)
Il rendering ora ha risolto il problema del jittering ed è diventato più efficiente

##### Aggiunte:
- Funzioni di output `print`, `println`, `clear`, `del` nel namespace `schermate`, 
da sostituirsi a clear() e cout

##### Patch:
- Corretti crash su lista filtrata vuota
- Ottimizzazioni: `reserve`, `getMaxSize` fuori dai loop, `clear()` invece di `= {}`

---

## - Versione 1.5 (major):
La versione introduce un miglioramento generico di performance e porta
l'aggiunta di varie nuove features.

##### Aggiunte:
- Bordi per tutte le schermate
- Schermata questionario

---

## - Versione 1.4.2.1 (patch):

Sono state corrette le impostazioni di caricamento di NuGet

<!-------------------------------------------------------------->

## - Versione 1.4.2 (patch):
Il codice è stato reso più semplice e sono state rimosse alcune ridondanze.

La libreria è stata caricata su NuGet ed è stato rifattorizzato completamente
il README.

##### Patch:
- La funzione `calculate()` ora non stampa più il contenuto a schermo, ma
lo carica sulla variabile `string contenuto` della classe genitrice `Schermata`

<!-------------------------------------------------------------->

## - Versione 1.4.1 (minor):
La leggibilità del codice è migliorata concentrandosi sulla cancellazione
del codice ripetuto e sull'aggiunta di commenti

<!-------------------------------------------------------------->

## - Versione 1.4 (major):
Ѐ aggiunta la schermata selettore filtrata ed il codice è stato migliorato
in leggibilità

##### Aggiunte:
- Schermata selettore filtrata

---

## - Versione 1.3 (major):
Ѐ aggiunta la schermata selettore large e migliorato il codice

##### Aggiunte:
- Schermata selettore large

##### Patch:
- La funzione `print()` ora non stampa tutto a schermo insieme, ma lo salva prima
su `string out`

---

## - Versione 1.2 (stable):
La versione 1.1 ha subito miglioramenti e funziona ora in modo più fluente,
inoltre è stata migliorata sensibilmente la leggibilità del codice

---

## - Versione 1.1 (beta):
Sono stati corretti vari errori della versione 1.0 ed ora la libreria funziona

---

## - Versione 1.0 (alpha):
ATTENZIONE: la versione non è stabile e non funziona come desiderato
##### Aggiunte:
- Schermata
- Schermata selettore
- Schermata selettore custom
