# Simple CLI

Simple CLI è una soluzione semplice per l'utilizzo di interfacce CLI in C++ senza la fatica di doverne creare di nuove da zero.

## Requisiti

- C++20 o superiore
- Compilatore compatibile (GCC, Clang, MSVC)
- Windows, Linux o macOS

## Installazione

### NuGet

Installazione con .NET CLI:
```
dotnet add package Simple_CLI --version 1.7.1
```

Installazione con PMC:
```
NuGet\Install-Package Simple_CLI -Version 1.7.1
```

Con il pacchetto NuGet (progetti Visual Studio/MSBuild) la directory degli header viene
configurata automaticamente: basta `#include "Simple_CLI.h"`.

### Manuale

La libreria è **header-only**: copia `Simple_CLI.h` insieme alla cartella `src/` nel tuo progetto
(mantienendo la struttura) e includi il solo `Simple_CLI.h`:
```
g++ -std=c++20 main.cpp -o myapp
```

## Setup

Su **Unix/Linux/macOS**, aggiungi `SETUP();` all'inizio del `main()` per abilitare la modalità raw del terminale:

```cpp
#include "Simple_CLI.h"

int main() {
    SETUP();
    // ...
}
```

Su **Windows** `SETUP` non è necessario ma può essere chiamato senza problemi.

---

## API C (`c_api.h`) — supporto a C puro

Per usare la libreria da **C puro** esiste un livello dedicato:

- `src/c_api.h`: header **C compatibile** (includibile da file `.c`), con handle opachi
  (`simple_cli_schermata`, `simple_cli_selettore`, ...) e solo tipi C;
- `src/c_api.cpp`: il *bridge* che implementa l'API con le classi C++ interne. È l'unico sorgente
  C++ da compilare: nei progetti MSBuild viene aggiunto **automaticamente** da
  `Simple_CLI.targets`; con CMake basta `cmake -DSIMPLE_CLI_BUILD_C_API=ON` (target
  `simple_cli_c_api`) oppure aggiungerlo manualmente alla build.

Regole di memoria:

- gli handle restituiti da `simple_cli_*_create` vanno liberati con la `*_destroy` corrispondente;
- le stringhe **restituite** dall'API sono allocate con `malloc`: liberarle con `simple_cli_free`;
- le stringhe **passate** all'API sono di sola lettura (copiate internamente): niente da liberare;
- le eccezioni C++ non attraversano mai il confine: le funzioni di creazione ritornano `NULL`
  in caso di errore, le altre sono no-op.

Esempio (file `.c`):
```c
#include "c_api.h"
#include <stdio.h>

int main(void) {
    simple_cli_schermata s = simple_cli_schermata_create("Riga 1\nRiga 2", 1);
    if (s == NULL) return 1;
    simple_cli_schermata_print(s, 0);

    const char* opzioni[] = { "Prima", "Seconda", "Terza" };
    simple_cli_selettore sel = simple_cli_selettore_create("Scegli:", opzioni, 3, 1);
    if (sel != NULL) {
        const int scelta = simple_cli_selettore_render(sel); /* interattiva */
        printf("Scelto: %d\n", scelta);
        simple_cli_selettore_destroy(sel);
    }
    simple_cli_schermata_destroy(s);
    return 0;
}
```

> **Nota:** nel pacchetto NuGet l'header è alla radice degli include (`#include "c_api.h"`);
> con la copia manuale della cartella `src/` si include `src/c_api.h`.

Compilazione manuale (il bridge è l'unico sorgente C++):
```
gcc -c main.c -I<pacchetto>/include
g++ -std=c++20 -c <pacchetto>/build/native/src/c_api.cpp -I<pacchetto>/include
g++ main.o c_api.o -o myapp
```

Il test `c_api_test.c` (C11, compilato con gcc e linkato al bridge) copre creazione, stampa,
`get_string`, aggiornamento, distruzione e gestione degli input invalidi.

---

## Nota sull'API C-style rimossa

> La precedente API C-style (`src/c_include.h`, solo C++) è stata **rimossa**: per C puro usa il
> livello `c_api.h` + `c_api.cpp` descritto sopra; per C++ usa direttamente le classi di
> `Simple_CLI.h` (quell'header non era comunque utilizzabile da un compilatore C: `namespace`,
> classi, overload e `std::string` nelle firme, e `extern "C"` cambia solo il name mangling).

---

## Lista delle schermate

### Semplici

#### Schermata base — `schermate::Schermata`
```
------------------
| Hello World!   |
------------------
```

#### Schermata scorrevole — `in arrivo`
```
---------------
| Intestazione|
|             |
| Riga1       |
| Riga2       |
| Riga3       |
| Riga4       |
| Riga5       |
| Riga6       |
| v           |
---------------
```

---

### Selettori

#### Selettore semplice — `schermate::SchermataSelettore`
```
Titolo:
> Opzione 1
- Opzione 2
- Opzione 3
```

#### Selettore scorrevole — `schermate::SchermataSelettoreLarge`
```
Titolo:
^
- Opzione 4
> Opzione 5
- Opzione 6
- Opzione 7
- Opzione 8
v
```

#### Selettore filtrato — `schermate::SchermataSelettoreFiltrata`
```
Titolo:
filtro
- opzioneFiltro1
- opzioneFiltro2
> opzioneFiltro3
- filtro5
v
```

#### Selettore custom — `schermate::SchermataSelettoreCustom`
```
Titolo:
1: Opzione 1
2: Opzione 2
a: Opzione a
x: Opzione x
```

---

### Questionari

#### Schermata questionario — `schermate::SchermataQuestionario`
```
Titolo:
> Questo è un selettore booleano: [X]

- Questo è un altro selettore booleano: [ ]

- Selettore di scala:
  [X----]

- Casella di testo: Hello World!
```

---

## Funzioni delle schermate

### Schermata base

| Funzione | Descrizione |
|----------|-------------|
| `Schermata(string, bool)` | Costruttore con contenuto e bordi |
| `Schermata()` | Costruttore vuoto senza bordi |
| `aggiorna(string, bool)` | Aggiorna il contenuto con o senza bordi |
| `print(bool)` | Stampa a schermo; se `true` sovrascrive il render precedente |
| `setContenuto(string, bool)` | Aggiorna il contenuto |
| `getContenuto()` | Restituisce il contenuto come `Content` |
| `getString()` | Restituisce il contenuto come `string` |

La variabile pubblica `printables` (`std::array<char, COUNT>`, indicizzata dai valori di `PrintableTypes`) mappa i tipi grafici ai caratteri usati. Può essere modificata per personalizzare l'aspetto delle schermate.

### Selettori

| Classe | Costruttore |
|--------|-------------|
| `SchermataSelettore` | `(titolo, opzioni, bordi = false)` |
| `SchermataSelettoreCustom` | `(titolo, titoliOpzioni, opzioni, autoRender = true, bordi = false)` |
| `SchermataSelettoreLarge` | `(titolo, opzioni, size, bordi)` |
| `SchermataSelettoreFiltrata` | `(titolo, opzioni, size, bordi)` |

| Funzione | Descrizione |
|----------|-------------|
| `render()` | Mostra la schermata e restituisce l'indice dell'opzione selezionata, oppure `-1` se si preme Escape |

> **Nota:** i selettori vengono creati **senza** renderizzarli. Eccezione: `SchermataSelettoreCustom` con `autoRender = true` (predefinito) stampa la schermata e **attende un tasto già nel costruttore**; passare `autoRender = false` per chiamare `render()` al momento giusto.

`SchermataSelettoreCustom` espone inoltre:

| Funzione | Descrizione |
|----------|-------------|
| `getResult()` | Restituisce il `char` associato all'opzione selezionata (`'\0'` se nessuna) |
| `getResultNumber()` | Restituisce l'indice dell'opzione selezionata (`-1` se nessuna) |

### Questionario

Il questionario usa oggetti `formElement` per rappresentare le opzioni:

| Campo | Tipo | Descrizione |
|-------|------|-------------|
| `titolo` | `string` | Intestazione dell'opzione |
| `questionType` | `questionTypes` | Tipo: `BOOLEANO`, `SCALA`, `INSERIMENTO` |
| `size` | `int` | Dimensione della scala (solo per `SCALA`) |
| `outInt` | `int*` | Output: `0`/`1` per booleani, indice per scale |
| `outString` | `std::string*` | Output: testo inserito per `INSERIMENTO` |

Esempio:
```cpp
int booleano = 0;
int scala = 0;
std::string testo;

SchermataQuestionario q("Titolo:", {
    {"Attiva funzione:", SchermataQuestionario::BOOLEANO, 0, &booleano, &testo},
    {"Livello (1-5):",   SchermataQuestionario::SCALA,    5, &scala,    &testo},
    {"Nome:",            SchermataQuestionario::INSERIMENTO, 0, &booleano, &testo}
});
q.render();
```

---

## Funzioni di output del namespace `schermate`

Queste funzioni sostituiscono `cout` e mantengono il conteggio delle righe per il rendering corretto:

| Funzione | Descrizione |
|----------|-------------|
| `schermate::print(string)` | Stampa una stringa senza andare a capo |
| `schermate::println(string)` | Stampa una stringa e va a capo |
| `schermate::clear()` | Cancella tutto l'output corrente |
| `schermate::del(unsigned int)` | Cancella le ultime N righe |

> **Nota:** usare `cout` direttamente invece di queste funzioni causa un conteggio errato delle righe e può lasciare righe residue a schermo durante le transizioni tra schermate.

### Funzioni di input

| Funzione | Descrizione |
|----------|-------------|
| `schermate::read()` | Legge una parola da `cin` |
| `schermate::readln()` | Come `read()`, ma inizia in una nuova riga |

---

## Controlli

| Tasto | Azione |
|-------|--------|
| `↑` / `W` | Su |
| `↓` / `S` | Giù |
| `←` / `-` | Sinistra / decrementa scala |
| `→` / `+` | Destra / incrementa scala |
| `Invio` | Conferma |
| `Esc` | Annulla / torna indietro (restituisce `-1`) |
| `Backspace` | Cancella ultimo carattere (inserimento / filtro) |
| `Canc` | Cancella tutto il testo (inserimento / filtro) |
| `Spazio` | Attiva/disattiva booleano |
| `Ctrl+C` | Chiude il programma |

---
