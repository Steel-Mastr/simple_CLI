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
dotnet add package Simple_CLI --version 1.6.3
```

Installazione con PMC:
```
NuGet\Install-Package Simple_CLI -Version 1.6.3
```

### Manuale

Copia `Simple_CLI.h` e `Simple_CLI.cpp` nel tuo progetto e compilali insieme ai tuoi sorgenti:
```
g++ -std=c++20 main.cpp Simple_CLI.cpp -o myapp
```

## Setup

Su **Unix/Linux/macOS**, aggiungi `SETUP;` all'inizio del `main()` per abilitare la modalità raw del terminale:

```cpp
#include "Simple_CLI.h"

int main() {
    SETUP;
    // ...
}
```

Su **Windows** `SETUP` non è necessario ma può essere chiamato senza problemi.

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

La variabile pubblica `printables` (`unordered_map<int, char>`) mappa i tipi grafici ai caratteri usati. Può essere modificata per personalizzare l'aspetto delle schermate.

### Selettori

| Funzione | Descrizione |
|----------|-------------|
| `costruttore(...)` | Crea il selettore senza renderizzarlo |
| `render()` | Mostra la schermata e restituisce l'indice dell'opzione selezionata, oppure `-1` se si preme Escape |

`SchermataSelettoreCustom` espone inoltre:

| Funzione | Descrizione |
|----------|-------------|
| `getResult()` | Restituisce il `char` associato all'opzione selezionata |

### Questionario

Il questionario usa oggetti `formElement` per rappresentare le opzioni:

| Campo | Tipo | Descrizione |
|-------|------|-------------|
| `titolo` | `string` | Intestazione dell'opzione |
| `questionType` | `questionTypes` | Tipo: `BOOLEANO`, `SCALA`, `INSERIMENTO` |
| `size` | `int` | Dimensione della scala (solo per `SCALA`) |
| `outInt` | `int&` | Output: `0`/`1` per booleani, indice per scale |
| `outString` | `string&` | Output: testo inserito per `INSERIMENTO` |

Esempio:
```cpp
int booleano = 0;
int scala = 0;
string testo;

SchermataQuestionario q("Titolo:", {
    {"Attiva funzione:", SchermataQuestionario::BOOLEANO, 0, booleano, testo},
    {"Livello (1-5):",   SchermataQuestionario::SCALA,    5, scala,    testo},
    {"Nome:",            SchermataQuestionario::INSERIMENTO, 0, booleano, testo}
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