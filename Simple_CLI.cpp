#include "Simple_CLI.h"
#include <utility>
#include <conio.h>

#ifdef _WIN32
#define CLEAN system("cls")
#else
#define CLEAN system("clear")
#endif

using namespace schermate;


/*
 *   ==============================
 *   ========== GENERALI ==========
 *   ==============================
 */

// Gestisce il rendering ripetuto di una stessa stringa
string ripeti(const int rep, const string& s) {
    string out;
    for (int i = 0; i < rep; i++)
        out += s;
    return out;
}

// Restituisce la versione toLowerCase della stringa s
string toLower(const string& s) {
    string out;
    for (const char c : s)
        out += static_cast<char>(tolower(c));
    return out;
}

// Nelle schermate di selezione sostituisce selezionato++
void addSelezionato(int& selezionato, const int size) {
    if (selezionato >= size - 1) return;
    selezionato++;
}

// Nelle schermate di selezione sostituisce selezionato--
void remSelezionato(int& selezionato) {
    if (selezionato <= 0) return;
    selezionato--;
}

// Nelle schermate a scorrimento sostituisce shift++
void addShift(int& shift, const int selezionato, const int size, const int listSize) {
    if (shift + size > listSize) return;        //  outOfList
    if (selezionato <= shift + 1) return;       //  notToMove
    shift++;
}

// Nelle schermate a scorrimento sostituisce shift--
void remShift(int& shift, const int selezionato) {
    if (selezionato >= shift + 1) return;       //  notToMove
    shift--;
}

string aggiungiBordi(const string& contenuto, const char WallHorizontal = '-', const char WallVertical = '|',
    const int paddingHorizontal = 0, const int paddingVertical = 0) {
    vector<string> righe;

    // creazione del vettore
    string temp;
    for (const char c : contenuto) {
        if (c == '\n') {
            righe.emplace_back(temp);
            temp = "";
        }
        else temp += c;
    }
    if (!temp.empty()) righe.emplace_back(temp);

    // Cerca la dimensione massima
    int maxSize = 0;
    for (string &s : righe)
        maxSize = max(maxSize, static_cast<int>(s.length()));

    string out;

    //  Riga superiore
    out += ripeti(maxSize + 2 + 2 * paddingHorizontal, {WallHorizontal}) + '\n';
    out += ripeti(paddingVertical, (WallVertical + ripeti(2 + 2 * paddingHorizontal, " ") + WallVertical) + '\n');

    // Contenuto
    for (const string& s : righe) {
        out += WallVertical +
            ripeti(paddingHorizontal, " ") + s + (ripeti(static_cast<int>(maxSize - s.length()), " ") + ripeti(paddingHorizontal, " ")
            + WallVertical + '\n');
    }

    // Riga inferiore
    out += ripeti(paddingVertical, (WallVertical + ripeti(2 + 2 * paddingHorizontal, " ") + WallVertical) + '\n');
    return out + ripeti(maxSize + 2, {WallHorizontal}) + '\n';
}


/*
 *   ========================================
 *   ========== SCHERMATA GENERALE ==========
 *   ========================================
 */

// Costruttori - una stringa unica con righe separate da '/n'
Schermata::Schermata(string& contenuto, const bool bordi) {
    hasBordi = bordi;
    // Richiama la funziona aggiorna
    aggiorna(contenuto, bordi);
}

// Aggiornamento - una stringa unica con righe separate da '/n'
void Schermata::aggiorna(string &obj, const bool bordi) {
    hasBordi = bordi;
    contenuto = bordi ?
        aggiungiBordi(obj, printables[WALL_HORIZONTAL], printables[WALL_VERTICAL],
            paddingHorizontal, paddingVertical) :
        obj;
}

// Stampa a schermo della schermata - cancella lo schermo prima se si imposta del su true
void Schermata::print(const bool del) const {
    if (del) CLEAN;
    cout << contenuto << endl;
}

// Imposta il contenuto (senza bordi)
void Schermata::setContenuto(string& cont, const bool bordi) {
    aggiorna(cont, bordi);
}

// Leggi il contenuto
string Schermata::getContenuto() {
    return contenuto;
}

/*
 *   ========================================
 *   ========== SCHERMATA SELETTORE =========
 *   ========================================
 */

// Crea una schermata selettore standard, ma non la renderizza
SchermataSelettore::SchermataSelettore(string  titolo, const vector<string>& opzioni, const bool bordi) :
    Schermata(),
    titolo(std::move(titolo)),
    opzioni(opzioni)
    {this->hasBordi =bordi;}

// Aggiorna la schermata
void SchermataSelettore::calculate() {
    string out;
    // Intestazione con il titolo
    out += titolo + '\n';

    // Aggiungi tutti gli elementi della lista
    string sel;
    for (int i = 0; i < opzioni.size(); i++) {
        sel = (i == selezionato ? printables[SELECTOR] : printables[UNSELECTED]);
        out += sel + " " + opzioni[i] + '\n';
    }
    setContenuto(out, hasBordi);
    print(true);
}

// Mostra la schermata in attesa di un invio, al che restituisce l'indice selezionato
int SchermataSelettore::render() {
    selezionato = 0;
    while (true) {
        // Stampa la schermata
        calculate();
        print(true);

        // Leggi l'imput
        switch (_getch()) {
            case 3:
                exit(0);
            case 27:
                return -1;
            case 'w':
            case 72:
                remSelezionato(selezionato);
                break;
            case 's':
            case 80:
                addSelezionato(selezionato, static_cast<int>(opzioni.size()));
                break;
            case '\r':
                CLEAN;
                return selezionato;
            default:
                ;
        }
    }
}


/*
 *   ===============================================
 *   ========== SCHERMATA SELETTORE CUSTOM =========
 *   ===============================================
*/

// Crea una schermata selettore custom e renderizza automaticamente il contenuto - impostare autoRender su true per renderizzare subito
SchermataSelettoreCustom::SchermataSelettoreCustom(string titolo, const vector<char>& titoliOpzioni,
    const vector<string>& opzioni, const bool autoRender, const bool bordi) :
    titolo(std::move(titolo)), opzioni(opzioni), titoliOpzioni(titoliOpzioni) {
    if (titoliOpzioni.size() != opzioni.size())
        throw length_error("Lunghezze inconsistenti tra titoliOpzioni ed opzioni");
    // Renderizza la schermata se richiesto
    this->hasBordi = bordi;
    if (autoRender) render();
}

void SchermataSelettoreCustom::calculate() {
    string out;

    // Intestazione con il titolo
    out += titolo + '\n';

    // Aggiungi tutte le opzioni
    string sel;
    for (int i = 0; i < titoliOpzioni.size(); i++) {
        sel = titoliOpzioni[i];
        out += sel + ": " + opzioni[i] + '\n';
    }
    // Imposta il contenuto
    setContenuto(out, hasBordi);
}

// Renderizza la schermata, restituisce l'indice del risultato
int SchermataSelettoreCustom::render() {
    // Stampa il contenuto a schermo
    calculate();
    print(true);

    // Cerca il valore inserito
    const char input = static_cast<char>(_getch());
    if (input == 3) exit(0);
    if (input == 27) return -1;
    for (int i = 0; i < titoliOpzioni.size(); i++)
        if (input == titoliOpzioni[i]) {
            result = i;
            CLEAN;
            return i;
        }
    return -1;
}

// Restituisce il char corrispondente al risultato
char SchermataSelettoreCustom::getResult() const { return titoliOpzioni[result]; }


/*
 *   ==============================================
 *   ========== SCHERMATA SELETTORE LARGE =========
 *   ==============================================
*/

// Crea una schermata selettore large senza renderizzarla
SchermataSelettoreLarge::SchermataSelettoreLarge(string titolo, const vector<string> &opzioni, const int size = 9, bool bordi) :
    SchermataSelettore(std::move(titolo), opzioni) {
    this->hasBordi = bordi;
    this->size = size > 2 ? size : 3;
    if (opzioni.size() > size) trueLarge = true;
    else trueLarge = false;
}

// Aggiorna la schermata
void SchermataSelettoreLarge::calculate() {
    string out;
    // Intestazione con il titolo
    out += titolo + '\n';
    if (shift < 0) shift = 0;
    // Se necessaria, aggiungi ARROW_UP
    string selUpDown;
    selUpDown = (shift > 0) ? printables[ARROW_UP] : ' ';
    out += selUpDown + '\n';
    // Aggiungi tutte le opzioni
    string sel;
    for (int i = shift; (i < shift + size) && (i < opzioni.size()); i++) {
        sel = ((selezionato == i) ? printables[SELECTOR] : printables[UNSELECTED]);
        out += sel + " " + opzioni[i] + '\n';
    }

    // Se necessaria, aggiungi ARROW_DOWN
    selUpDown = (shift < opzioni.size() - size) ? printables[ARROW_DOWN] : ' ';
    out += selUpDown;

    // Imposta il contenuto
    setContenuto(out, hasBordi);
}

// Renderizza la schermata restituendo l'indice della selezione
int SchermataSelettoreLarge::render() {
    selezionato = 0;
    shift = 0;
    while (true) {
        // Stampa la schermata
        calculate();
        print(true);

        // Leggi l'input
        switch (_getch()) {
            case 3:
                exit(0);
            case 27:
                return -1;
            case 'w':
            case 72:
                remSelezionato(selezionato);

                remShift(shift, selezionato);
                break;
            case 's':
            case 80:
                addSelezionato(selezionato, static_cast<int>(opzioni.size()));

                addShift(shift, selezionato, size, static_cast<int>(opzioni.size()));
                break;
            case '\r':
                CLEAN;
                return selezionato;
            default:
                ;
        }
    }
}


/*
 *   =================================================
 *   ========== SCHERMATA SELETTORE FILTRATA =========
 *   =================================================
*/

// Crea una schermata selettore filtrata secondo le regole di schermata selettore
SchermataSelettoreFiltrata::SchermataSelettoreFiltrata(string titolo, const vector<string> &opzioni, const int size, const bool bordi) :
    SchermataSelettoreLarge(std::move(titolo), opzioni, size, bordi) {}

// Applica i filtri
void SchermataSelettoreFiltrata::calculateNewSet() {
    opzioniFiltrate = {};
    for (const string& s : opzioni)
        if (toLower(s).find(toLower(filtro)) != string::npos)
            opzioniFiltrate.emplace_back(s);
}

// Aggiorna la schermata
void SchermataSelettoreFiltrata::calculate() {
    string out;

    // Intestazione con titolo e filtro
    out += titolo + '\n';
    out += filtro + '\n';

    // Se necessaria, aggiungi ARROW_UP
    if (shift < 0) shift = 0;
    string selUpDown;
    selUpDown = (shift > 0) ? printables[ARROW_UP] : ' ';
    out += selUpDown + '\n';

    // Aggiungi tutte le opzioni (filtrate)
    string sel;
    for (int i = 0; i < opzioniFiltrate.size(); i++) {
        sel = (selezionato == i) ? printables[SELECTOR] : printables[UNSELECTED];
        out += sel + " " + opzioniFiltrate[i] + '\n';
    }

    // Se necessaria, aggiungi ARROW_DOWN
    selUpDown = (shift < opzioniFiltrate.size() - size) ? printables[ARROW_DOWN] : ' ';
    out += selUpDown;

    // Imposta il contenuto
    setContenuto(out, hasBordi);
}

// Renderizza la schermata restituendo l'indice della selezione
int SchermataSelettoreFiltrata::render() {
    filtro = "";
    bool cambioFiltro = true;
    while (true) {
        // Calcola il nuovo set solo se il filtro cambia
        if (cambioFiltro) {
            calculateNewSet();
            selezionato = 0;
            shift = 0;
        }
        cambioFiltro = false;

        // Stampa la schermata
        calculate();
        print(true);

        // Leggi l'input
        switch (const char in = static_cast<char>(_getch())) {
            case 3:
                exit(0);
            case 27:
                return -1;
            case 72:
                remSelezionato(selezionato);

                remShift(shift, selezionato);
                break;
            case 80:
                addSelezionato(selezionato, static_cast<int>(opzioniFiltrate.size()));

                addShift(shift, selezionato, size, static_cast<int>(opzioniFiltrate.size()));
                break;
            case '\r':
                CLEAN;
                for (int i = 0; i < opzioni.size(); i++)
                    if (opzioni[i] == opzioniFiltrate[selezionato]) return i;
            case 8:
                filtro.pop_back();
                cambioFiltro = true;
                break;
            case 127:
                filtro = "";
                cambioFiltro = true;
                break;
            default:
                if (in > 32) {
                    filtro += in;
                    cambioFiltro = true;
                }
                break;
        }
    }
}


/*
 *   ===========================================
 *   ========== SCHERMATA QUESTIONARIO =========
 *   ===========================================
*/

SchermataQuestionario::SchermataQuestionario(string  titolo, vector<formElement> opzioni) :
    Schermata(), opzioni(std::move(opzioni)), titolo(std::move(titolo)) {}

void SchermataQuestionario::calculate() {
    string out;

    // Aggiungi il titolo
    out += titolo + '\n';

    // Aggiungi tutte le opzioni
    for (int i = 0; i < opzioni.size(); i++) {
        // Aggiungi il titolo dell'opzione
        out += printables[i == selezionato ? SELECTOR : UNSELECTED];
        out += " " + opzioni[i].titolo + " ";

        // Aggiungi il corpo
        switch (opzioni[i].questionType) {
            case INSERIMENTO:
                out += opzioni[i].outString + "\n";
                break;
            case BOOLEANO:
                out += printables[CONTAIN_LEFT];
                out += opzioni[i].outInt == 1 ? printables[CHECKED] : printables[UNCHECKED];
                out += printables[CONTAIN_RIGHT];
                break;
            case SCALA: {
                out += '\n';
                out += printables[CONTAIN_LEFT];
                for (int s = 0; s < opzioni[i].size; s++) {
                    out += printables[s == opzioni[i].outInt ? CHECKED : UNSELECTED];
                }
                out += printables[CONTAIN_RIGHT];
                break;
            }
        }

        // Aggiungi lo spazio sotto
        out += "\n\n";

        // Imposta il contenuto
        setContenuto(out, hasBordi);
    }
}

void SchermataQuestionario::render() {
    selezionato = 0;

    while (true) {
        calculate();
        print(true);

        switch (const char in = static_cast<char>(_getch())) {
            case 3:
                exit(0);
            case 27:
                return;
            case 72:
                remSelezionato(selezionato);
                break;
            case 80:
                addSelezionato(selezionato, static_cast<int>(opzioni.size()));
                break;
            case '\r':
                return;
            default: {
                switch (opzioni[selezionato].questionType) {
                    case INSERIMENTO:
                        if (in == '\b') {
                            if (!opzioni[selezionato].outString.empty()) opzioni[selezionato].outString.pop_back();
                            calculate();
                            print(true);
                        }
                        else if (in == 127)
                            opzioni[selezionato].outString = "";
                        else opzioni[selezionato].outString += in;

                        break;
                    case BOOLEANO:
                        if (in == ' ')
                            opzioni[selezionato].outInt = 1 - opzioni[selezionato].outInt;
                        break;
                    case SCALA:
                        if ((in == '+' || in == 77) && opzioni[selezionato].outInt < opzioni[selezionato].size - 1)
                            opzioni[selezionato].outInt++;
                        if ((in == '-' || in == 75) && opzioni[selezionato].outInt > 0)
                            opzioni[selezionato].outInt--;
                        break;
                    default:
                        ;
                }
            }
        }
    }
}