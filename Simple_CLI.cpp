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

string ripeti(const int rep, const string& s) {
    string out;
    for (int i = 0; i < rep; i++)
        out += s;
    return out;
}

string toLower(const string& s) {
    string out;
    for (const char c : s)
        out += static_cast<char>(tolower(c));
    return out;
}


/*
 *   ========================================
 *   ========== SCHERMATA GENERALE ==========
 *   ========================================
 */

Schermata::Schermata(const vector<string>& contenuto) {
    aggiorna(contenuto);
}
Schermata::Schermata(const string &contenuto) {
    aggiorna(contenuto);
}

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


void Schermata::print(const bool del) const {
    if (del) CLEAN;
    cout << contenuto << endl;
}

void Schermata::setContenuto(const string &cont) {
    contenuto = cont;
}

/*
 *   ========================================
 *   ========== SCHERMATA SELETTORE =========
 *   ========================================
 */

// IL COSTRUTTORE CREA UNA SCHERMATA SELETTORE STANDARD, MA NON LA RENDERIZZA
SchermataSelettore::SchermataSelettore(string  titolo, const vector<string>& opzioni) :
    Schermata(),
    titolo(std::move(titolo)),
    opzioni(opzioni)
    {}

// AGGIORNA LA SCHERMATA
void SchermataSelettore::calculate() const {
    string out;
    out += titolo + '\n';
    string sel;
    for (int i = 0; i < opzioni.size(); i++) {
        sel = (i == selezionato ? printables.at(SELECTOR) : printables.at(UNSELECTED));
        out += sel + " " + opzioni[i] + '\n';
    }
    cout << out;
}

// MOSTRA LA SCHERMATA IN ATTESA DI UN INVIO, AL CHE RESTITUISCE L'INDICE SELEZIONATO
int SchermataSelettore::render() {
    selezionato = 0;
    CLEAN;
    while (true) {
        calculate();
        switch (_getch()) {
            case 3:
                exit(0);
            case 27:
                return -1;
            case 'w':
            case 72:
                if (selezionato > 0) selezionato--;
                CLEAN;
                break;
            case 's':
            case 80:
                if (selezionato < opzioni.size() - 1) selezionato++;
                CLEAN;
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

// IL COSTRUTTORE RENDERIZZA AUTOMATICAMENTE IL CONTENUTO, DISATTIVABILE CON AUTORENDER
SchermataSelettoreCustom::SchermataSelettoreCustom(string titolo, const vector<char>& titoliOpzioni,
    const vector<string>& opzioni, const bool autoRender) :
    titolo(std::move(titolo)), opzioni(opzioni), titoliOpzioni(titoliOpzioni) {
    if (titoliOpzioni.size() != opzioni.size())
        throw length_error("Lunghezze inconsistenti tra titoliOpzioni ed opzioni");
    // CALCOLA LA SCHERMATA
   if (autoRender) render();
}

// RENDERIZZA LA SCHERMATA
int SchermataSelettoreCustom::render() {
    CLEAN;
    string out;
    out += titolo + '\n';
    string sel;
    for (int i = 0; i < titoliOpzioni.size(); i++) {
        sel = titoliOpzioni[i];
        out += sel + ": " + opzioni[i] + '\n';
    }
    cout << out;
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

// RESTITUISCE L'ULTIMO RISULTATO SELEZIONATO
char SchermataSelettoreCustom::getResult() const { return titoliOpzioni[result]; }


/*
 *   ==============================================
 *   ========== SCHERMATA SELETTORE LARGE =========
 *   ==============================================
*/

// IL COSTRUTTORE CREA UNA SCHERMATA SELETTORE LARGE, MA NON LA RENDERIZZA
SchermataSelettoreLarge::SchermataSelettoreLarge(string titolo, const vector<string> &opzioni, const int size = 9) :
    SchermataSelettore(std::move(titolo), opzioni) {
    this->size = size > 2 ? size : 3;
    if (opzioni.size() > size) trueLarge = true;
    else trueLarge = false;
}

// AGGIORNA LA SCHERMATA
void SchermataSelettoreLarge::calculate() {
    string out;
    out += titolo + '\n';
    if (shift < 0) shift = 0;
    out += (shift > 0 ? moreUp : "") + '\n';
    string sel;
    for (int i = shift; (i < shift + size) && (i < opzioni.size()); i++) {
        sel = ((selezionato == i) ? printables[SELECTOR] : printables[UNSELECTED]);
        out += sel + " " + opzioni[i] + '\n';
    }
    if (shift < opzioni.size() - size)
        out += moreDown;
    CLEAN;
    cout << out << endl;
}

// RENDERIZZA LA SCHERMATA
int SchermataSelettoreLarge::render() {
    selezionato = 0;
    shift = 0;
    while (true) {
        calculate();
        switch (_getch()) {
            case 3:
                exit(0);
            case 27:
                return -1;
            case 'w':
            case 72:
                if (selezionato == 0) break;

                selezionato--;
                if (selezionato < shift + 1)
                    shift--;
                break;
            case 's':
            case 80:
                if (selezionato == opzioni.size() - 1) break;

                selezionato++;
                if (shift + size > opzioni.size()) break;
                if (selezionato > shift + 1)
                    shift++;
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

// IL COSTRUTTORE CREA UNA SCHERMATA SELETTORE FILTRATA SECONDO LE REGOLE DI SCHERMATA SELETTORE
SchermataSelettoreFiltrata::SchermataSelettoreFiltrata(string titolo, const vector<string> &opzioni, const int size) :
    SchermataSelettoreLarge(std::move(titolo), opzioni, size) {}

void SchermataSelettoreFiltrata::calculateNewSet() {
    opzioniFiltrate = {};
    for (const string& s : opzioni)
        if (toLower(s).find(toLower(filtro)) != string::npos)
            opzioniFiltrate.emplace_back(s);
}

void SchermataSelettoreFiltrata::calculate() {
    string out;
    out += titolo + '\n';
    out += filtro + '\n';
    if (shift < 0) shift = 0;
    out += (shift > 0 ? moreUp : "") + '\n';
    string sel;
    for (int i = 0; i < opzioniFiltrate.size(); i++) {
        sel = (selezionato == i) ? printables[SELECTOR] : printables[UNSELECTED];
        out += sel + " " + opzioniFiltrate[i] + '\n';
    }
    if (shift < opzioniFiltrate.size() - size)
        out += moreDown;
    CLEAN;
    cout << out << endl;
}

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
        switch (const char in = static_cast<char>(_getch())) {
            case 3:
                exit(0);
            case 27:
                return -1;
            case 72:
                if (selezionato == 0) break;

                selezionato--;
                if (selezionato < shift + 1)
                    shift--;
                break;
            case 80:
                if (selezionato == opzioniFiltrate.size() - 1) break;

                selezionato++;
                if (shift + size > opzioniFiltrate.size()) break;
                if (selezionato > shift + 1)
                    shift++;
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