#include "library.h"

#include <utility>

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

    out += ripeti(static_cast<int>(max_s), &printables[WALL_HORIZONTAL]) + '\n';
    for (const string & s : obj) {
        out += printables[WALL_VERTICAL];
        out += s;
        out += ripeti(static_cast<int>(max_s - s.length()), " ");
        out += printables[WALL_VERTICAL];
        out += '\n';
    }
    out += ripeti(static_cast<int>(max_s), &printables[WALL_HORIZONTAL]);
    out += '\n';
    contenuto = out;
}

void Schermata::aggiorna(const string &obj) {
    vector<string> divided;
    string temp;
    for (int i = 0; i < obj.size(); i++) {
        if (i == '\n') {
            divided.push_back(temp);
            i++;
            continue;
        }
        temp += obj[i];
    }
    aggiorna(divided);
}


void Schermata::print() const {
    CLEAN;
    cout << contenuto << endl;
}

/*
 *   ========================================
 *   ========== SCHERMATA SELETTORE =========
 *   ========================================
 */

SchermataSelettore::SchermataSelettore(string  titolo, const vector<int>& opzioni) :
    Schermata(),
    titolo(std::move(titolo)),
    opzioni(opzioni)
    {}

// AGGIORNA LA SCHERMATA DA RENDERIZZARE
void SchermataSelettore::calculate(const int index) const {
    cout << titolo << endl;
    for (int i = 0; i < opzioni.size(); i++) {
        const char select = i == index ? printables.at(SELECTOR) : printables.at(UNSELECTED);
        cout << select << " " << opzioni[i] << endl;
    }
}

// MOSTRA LA SCHERMATA IN ATTESA DI UN INVIO, AL CHE RESTITUISCE L'INDICE SELEZIONATO
int SchermataSelettore::render() const {
    int index = 0;
    while (true) {
        CLEAN;
        calculate(index);
        print();
        switch (_getch()) {
            case 'w':
                if (index > 0) index--;
                break;
            case 72:
                if (index > 0) index--;
                break;
            case 's':
                if (index < opzioni.size() - 1) index++;
                break;
            case 80:
                if (index < opzioni.size() - 1) index++;
                break;
            case '\n':
                return index;
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

// IL COSTRUTTORE RENDERIZZA AUTOMATICAMENTE IL CONTENUTO
SchermataSelettoreCustom::SchermataSelettoreCustom(string titolo, const vector<char>& titoliOpzioni,
    const vector<string>& opzioni, const bool autoRender) :
    titolo(std::move(titolo)), opzioni(opzioni), titoliOpzioni(titoliOpzioni) {
    if (titoliOpzioni.size() != opzioni.size())
        throw length_error("Lunghezze inconsistenti tra titoliOpzioni ed opzioni");
    // CALCOLA LA SCHERMATA
    if (!autoRender) return;
    CLEAN;
    cout << titolo << endl;
    for (int i = 0; i < titoliOpzioni.size(); i++)
        cout << titoliOpzioni[i] << ": " << opzioni[i] << endl;
    // CERCA IL VALORE INSERITO
    const char input = static_cast<char>(_getch());
    for (int i = 0; i < titoliOpzioni.size(); i++)
        if (input == titoliOpzioni[i]) {
            result = i;
            return;
        }
}

void SchermataSelettoreCustom::render() {
    CLEAN;
    cout << titolo << endl;
    for (int i = 0; i < titoliOpzioni.size(); i++)
        cout << titoliOpzioni[i] << ": " << opzioni[i] << endl;
    // CERCA IL VALORE INSERITO
    const char input = static_cast<char>(_getch());
    for (int i = 0; i < titoliOpzioni.size(); i++)
        if (input == titoliOpzioni[i]) {
            result = i;
            return;
        }
}

int SchermataSelettoreCustom::getResult() const { return result; }