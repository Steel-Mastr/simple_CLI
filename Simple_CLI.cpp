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


/*
 *   ========================================
 *   ========== SCHERMATA GENERALE ==========
 *   ========================================
 */

// Costruttori (creano una schermata con i bordi) - una riga per stringa del vettore
Schermata::Schermata(const vector<string>& contenuto) {
    aggiorna(contenuto);
}
// Costruttori (creano una schermata con i bordi) - una stringa unica con righe separate da '/n'
Schermata::Schermata(const string &contenuto) {
    aggiorna(contenuto);
}

// Aggiornamento (rinnovano una schermata con i bordi) - una riga per stringa del vettore
void Schermata::aggiorna(const vector<string> &obj) {
    string out;
    int max_s = 0;
    for (const string& s : obj)
        max_s = max(max_s, static_cast<int>(s.length()));

    out += ripeti(static_cast<int>(max_s + 2), &printables[WALL_HORIZONTAL]) + '\n';
    for (const string & s : obj) {
        out += printables[WALL_VERTICAL];
        out += s;
        out += ripeti(static_cast<int>(max_s - s.length()), " ");
        out += printables[WALL_VERTICAL];
        out += '\n';
    }
    out += ripeti(static_cast<int>(max_s + 2), &printables[WALL_HORIZONTAL]);
    out += '\n';
    contenuto = out;
}
// Aggiornamento (rinnovano una schermata con i bordi) - una stringa unica con righe separate da '/n'
void Schermata::aggiorna(const string &obj) {
    vector<string> divided;
    string temp;
    for (const char i : obj) {
        if (i == '\n') {
            divided.push_back(temp);
            continue;
        }
        temp += i;
    }
    aggiorna(divided);
}

// Stampa a schermo della schermata - cancella lo schermo prima se si imposta del su true
void Schermata::print(const bool del) const {
    if (del) CLEAN;
    cout << contenuto << endl;
}

// Imposta il contenuto (senza bordi)
void Schermata::setContenuto(const string &cont) {
    contenuto = cont;
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
SchermataSelettore::SchermataSelettore(string  titolo, const vector<string>& opzioni) :
    Schermata(),
    titolo(std::move(titolo)),
    opzioni(opzioni)
    {}

// Aggiorna la schermata
void SchermataSelettore::calculate() {
    string out;
    out += titolo + '\n';
    string sel;
    for (int i = 0; i < opzioni.size(); i++) {
        sel = (i == selezionato ? printables.at(SELECTOR) : printables.at(UNSELECTED));
        out += sel + " " + opzioni[i] + '\n';
    }
    setContenuto(out);
    print(true);
}

// Mostra la schermata in attesa di un invio, al che restituisce l'indice selezionato
int SchermataSelettore::render() {
    selezionato = 0;
    while (true) {
        calculate();
        print(true);
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
    const vector<string>& opzioni, const bool autoRender) :
    titolo(std::move(titolo)), opzioni(opzioni), titoliOpzioni(titoliOpzioni) {
    if (titoliOpzioni.size() != opzioni.size())
        throw length_error("Lunghezze inconsistenti tra titoliOpzioni ed opzioni");
    // CALCOLA LA SCHERMATA
   if (autoRender) render();
}

// Renderizza la schermata, restituisce l'indice del risultato
int SchermataSelettoreCustom::render() {
    string out;
    out += titolo + '\n';
    string sel;
    for (int i = 0; i < titoliOpzioni.size(); i++) {
        sel = titoliOpzioni[i];
        out += sel + ": " + opzioni[i] + '\n';
    }
    setContenuto(out);
    print(true);
    // CERCA IL VALORE INSERITO
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
SchermataSelettoreLarge::SchermataSelettoreLarge(string titolo, const vector<string> &opzioni, const int size = 9) :
    SchermataSelettore(std::move(titolo), opzioni) {
    this->size = size > 2 ? size : 3;
    if (opzioni.size() > size) trueLarge = true;
    else trueLarge = false;
}

// Aggiorna la schermata
void SchermataSelettoreLarge::calculate() {
    string out;
    out += titolo + '\n';
    if (shift < 0) shift = 0;
    string selUpDown;
    selUpDown = (shift > 0) ? printables[ARROW_UP] : ' ';
    out += selUpDown + '\n';
    string sel;
    for (int i = shift; (i < shift + size) && (i < opzioni.size()); i++) {
        sel = ((selezionato == i) ? printables[SELECTOR] : printables[UNSELECTED]);
        out += sel + " " + opzioni[i] + '\n';
    }
    selUpDown = (shift < opzioni.size() - size) ? printables[ARROW_DOWN] : ' ';
    out += selUpDown;
    setContenuto(out);
    print(true);
}

// Renderizza la schermata restituendo l'indice della selezione
int SchermataSelettoreLarge::render() {
    selezionato = 0;
    shift = 0;
    while (true) {
        calculate();
        print(true);
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
SchermataSelettoreFiltrata::SchermataSelettoreFiltrata(string titolo, const vector<string> &opzioni, const int size) :
    SchermataSelettoreLarge(std::move(titolo), opzioni, size) {}

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
    out += titolo + '\n';
    out += filtro + '\n';
    if (shift < 0) shift = 0;
    string selUpDown;
    selUpDown = (shift > 0) ? printables[ARROW_UP] : ' ';
    out += selUpDown + '\n';
    string sel;
    for (int i = 0; i < opzioniFiltrate.size(); i++) {
        sel = (selezionato == i) ? printables[SELECTOR] : printables[UNSELECTED];
        out += sel + " " + opzioniFiltrate[i] + '\n';
    }
    selUpDown = (shift < opzioniFiltrate.size() - size) ? printables[ARROW_DOWN] : ' ';
    out += selUpDown;
    setContenuto(out);
    print(true);
}

// Renderizza la schermata restituendo l'indice della selezione
int SchermataSelettoreFiltrata::render() {
    filtro = "";
    bool cambioFiltro = true;
    while (true) {
        if (cambioFiltro) {
            calculateNewSet();
            selezionato = 0;
            shift = 0;
        }
        cambioFiltro = false;
        calculate();
        print(true);
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