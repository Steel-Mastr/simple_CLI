#include "Simple_CLI.h"
#include <utility>

#define CURSOR_UP(n)    "\033[" + to_string(n) + "A"
#define CURSOR_DOWN(n)  "\033[" + to_string(n) + "B"
#define CLEAR_LINE      "\033[2K"
#define DELETE_LINE     "\033[M"
#define DELETE_LOWER    "\033[J"

using namespace schermate;


/*
 *   ==============================
 *   ========== GENERALI ==========
 *   ==============================
 */

int readKey() {
    const int c = GETCH();
#ifdef _WIN32
    if (c == 224 || c == 0) {
        switch (GETCH()) {
            case 72: return UP;
            case 80: return DOWN;
            case 75: return LEFT;
            case 77: return RIGHT;
            default:
                return c;
        }
    }
#else
    if (c == 27) {
        const int c2 = GETCH();
        if (c2 == '[') {
            switch (GETCH()) {
                case 'A': return UP;
                case 'B': return DOWN;
                case 'C': return RIGHT;
                case 'D': return LEFT;
            }
        }
        return ESC;
    }
#endif
    if (c == '\r' || c == '\n') return ENTER;
    if (c == 3)   return CTRLC;
    if (c == 27)  return ESC;
    if (c == 8 || c == 127) return BACKSPACE;
    return c;
}

// Gestisce il rendering ripetuto di una stessa stringa
string ripeti(const int rep, const string& s) {
    string out;
    out.reserve(s.size() * rep);
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
    if (selezionato >= shift + 1) return;       //  notToMove
    shift--;
}

// Si sostituisce a cout
void schermate::print(const string& contenuto) {
    cout << contenuto << flush;
    for (const char& c : contenuto)
        if (c == '\n') righeStampate++;
}
// Si sostituisce a cout
void schermate::println(const string& contenuto) {
    cout << '\n' << contenuto << flush;
    righeStampate++;
    for (const char& c : contenuto)
        if (c == '\n') righeStampate++;
}
// Cancella n righe
void schermate::del(unsigned int count) {
    if (righeStampate > 0) [[likely]]
        cout << CURSOR_UP(min(righeStampate, count)) << '\r' << DELETE_LOWER << flush;
    righeStampate = max(0, static_cast<int>(righeStampate) - static_cast<int>(count));
}
// Cancella tutto
void schermate::clear() {
    if (righeStampate > 0) [[likely]]
        cout << CURSOR_UP(righeStampate) << '\r' << DELETE_LOWER << flush;
    righeStampate = 0;
}



// Trasforma un vettore di stringhe in una singola stringa con /n
string parseString(const vector<string>& contenuto) {
    string out;
    unsigned int total = 0;
    for (const string& s : contenuto)
        total += s.length() + 1;
    out.reserve(total);
    for (const string& s : contenuto)
        out += s + '\n';
    return out;
}

// Trasforma una stringa in un vettore
vector<string> schermate::parseStringVector(const string& stringa) {
    vector<string> out;
    string current;
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
unsigned int schermate::getMaxSize(const vector<string>& contenuto) {
    unsigned int maxSize = 0;
    for (const string& s : contenuto)
        maxSize = max(maxSize, static_cast<unsigned int>(s.length()));
    return maxSize;
}

vector<string> aggiungiBordi(
    const Content& contenuto,
    const char WallHorizontal = '-', const char WallVertical = '|',
    const int paddingHorizontal = 0, const int paddingVertical = 0) {
    vector<string> out;
    out.reserve(contenuto.data.size() + 2 * paddingVertical + 2);
    //  Riga superiore
    out.emplace_back(ripeti(contenuto.maxSize + 2 + 2 * paddingHorizontal, {WallHorizontal}));
    // Righe di padding verticale
    out.emplace_back(ripeti(paddingVertical, (WallVertical + ripeti(2 + 2 * paddingHorizontal, " ") + WallVertical)));

    // Contenuto
    for (const string& s : contenuto.data) {
        out.emplace_back(
            // Muro e padding
            WallVertical
            + ripeti(paddingHorizontal, " ")
            // Contenuto e filler alla fine
            + s
            + (ripeti(static_cast<int>(contenuto.maxSize - s.length()), " ")
            // Padding e muro
            + ripeti(paddingHorizontal, " ")
            + WallVertical)
        );
    }

    // Padding verticale e riga inferiore
    out.emplace_back(
        ripeti(paddingVertical,
            WallVertical + ripeti(2 + 2 * paddingHorizontal, " ") + WallVertical));
    out.emplace_back(ripeti(contenuto.maxSize + 2, {WallHorizontal}));
    return out;
}


/*
 *   ========================================
 *   ========== SCHERMATA GENERALE ==========
 *   ========================================
 */

// Costruttori - una stringa unica con righe separate da '/n'
Schermata::Schermata(const string& contenuto, const bool bordi) {
    // Richiama la funziona aggiorna
    aggiorna(contenuto, bordi);
}

// Aggiornamento - una stringa unica con righe separate da '/n'
void Schermata::aggiorna(const string& obj, const bool bordi) {
    hasBordi = bordi;
    contenuto = Content(parseStringVector(obj));
    if (!bordi)
        return;

    contenuto.data = aggiungiBordi(contenuto,
        printables[WALL_HORIZONTAL],
        printables[WALL_VERTICAL], paddingHorizontal, paddingVertical);
    contenuto.maxSize += 2 * paddingHorizontal + 2;
}

// Aggiornamento - vettore di stringhe
void Schermata::aggiorna(const vector<string>& obj, const bool bordi) {
    contenuto = Content(obj);
    hasBordi = bordi;
    if (!bordi)
        return;
    contenuto.data = aggiungiBordi(
        contenuto,
        printables[WALL_HORIZONTAL],
        printables[WALL_VERTICAL],
        paddingHorizontal,
        paddingVertical
        );
    contenuto.maxSize += 2 * paddingHorizontal + 2;
}
// Stampa a schermo della schermata - cancella lo schermo prima se si imposta del su true
void Schermata::print(const bool del) const {

    if (!del) [[unlikely]] {
        for (const string& s : contenuto.data) {
            cout << s << '\n';
        }
        righeStampate += contenuto.data.size();
        cout << flush;
        return;
    }
    clear();
    for (const string& s : contenuto.data) {
        cout << s << '\n';
    }
    righeStampate = contenuto.data.size();
    cout << flush;
}

// Imposta il contenuto
void Schermata::setContenuto(const string& cont, const bool bordi = false) {
    aggiorna(cont, bordi);
}
// Imposta il contenuto
void Schermata::setContenuto(const vector<string>& cont, const bool bordi = false) {
    aggiorna(cont, bordi);
}
// Imposta il contenuto automaticamente
void Schermata::setContenuto() {
    if (hasBordi)
        contenuto = {
        aggiungiBordi(contenuto),
        contenuto.maxSize + 2 * paddingHorizontal + 2
    };
}

// Leggi il contenuto
const Content& Schermata::getContenuto() const {
    return contenuto;
}
// Leggi il contenuto sotto forma di stringa
[[nodiscard]] string Schermata::getString() const {
    return parseString(contenuto.data);
}

/*
 *   ========================================
 *   ========== SCHERMATA SELETTORE =========
 *   ========================================
 */

// Crea una schermata selettore standard, ma non la renderizza
SchermataSelettore::SchermataSelettore(string titolo, const vector<string>& opzioni, const bool bordi) :
    Schermata(),
    titolo(std::move(titolo)),
    opzioni(opzioni)
    {this->hasBordi = bordi;}

// Aggiorna la schermata
void SchermataSelettore::calculate() {
    const vector<string> titoloParsed = parseStringVector(titolo);
    // Intestazione con il titolo
    contenuto = Content(titoloParsed);
    contenuto.data.reserve(2 * paddingVertical + hasBordi * 2 + titoloParsed.size() + opzioni.size());

    // Aggiungi tutti gli elementi della lista
    string sel;
    for (int i = 0; i < opzioni.size(); i++) {
        if (i == selezionato) [[unlikely]]
            sel = printables[SELECTOR];
        else
            sel = printables[UNSELECTED];
        contenuto.data.emplace_back(sel + " " + opzioni[i]);
    }
    contenuto.maxSize = getMaxSize(contenuto.data);
    setContenuto();
    print(true);
}

// Mostra la schermata in attesa di un invio, al che restituisce l'indice selezionato
int SchermataSelettore::render() {
    selezionato = 0;
    while (true) {
        // Stampa la schermata
        calculate();

        // Leggi l'ingresso
        switch (readKey()) {
            case CTRLC:
                exit(0);
            case ESC:
                return -1;
            case 'w':
            case UP:
                remSelezionato(selezionato);
                break;
            case 's':
            case DOWN:
                addSelezionato(selezionato, static_cast<int>(opzioni.size()));
                break;
            case ENTER:
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
    // Intestazione con il titolo
    contenuto = Content(titolo);

    // Aggiungi tutte le opzioni
    string sel;
    for (int i = 0; i < titoliOpzioni.size(); i++) {
        sel = titoliOpzioni[i];
        contenuto.data.emplace_back(sel + ": " + opzioni[i]);
    }
    contenuto.maxSize = getMaxSize(contenuto.data);
    // Imposta il contenuto
    setContenuto();
}

// Renderizza la schermata, restituisce l'indice del risultato
int SchermataSelettoreCustom::render() {
    // Stampa il contenuto a schermo
    calculate();
    print(true);

    // Cerca il valore inserito
    const int input = readKey();
    if (input == CTRLC) exit(0);
    if (input == ESC) return -1;
    for (int i = 0; i < titoliOpzioni.size(); i++)
        if (input == titoliOpzioni[i]) {
            result = i;
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
    // Intestazione con il titolo
    contenuto = Content(titolo);
    contenuto.data.reserve(contenuto.data.size() + size
        + hasBordi * 2 * (paddingVertical + 1));
    shift = max(shift, 0);
    if (shift > 0) [[likely]] {
        string toAdd;
        toAdd += printables[ARROW_UP];
        contenuto.data.emplace_back(toAdd);
    }
    // Aggiungi tutte le opzioni
    string sel;
    for (int i = shift; (i < shift + size) && (i < opzioni.size()); i++) {
        if (selezionato == i) [[unlikely]]
            sel = printables[SELECTOR];
        else
            sel = printables[UNSELECTED];
        contenuto.data.emplace_back(sel + " " + opzioni[i]);
    }

    // Se necessaria, aggiungi ARROW_DOWN
    if (static_cast<int>(shift) < opzioni.size() - size) {
        string toAdd;
        toAdd += printables[ARROW_DOWN];
        contenuto.data.emplace_back(toAdd);
    }
    contenuto.maxSize = getMaxSize(contenuto.data);
    // Imposta il contenuto
    setContenuto();
    print(true);
}

// Renderizza la schermata restituendo l'indice della selezione
int SchermataSelettoreLarge::render() {
    selezionato = 0;
    shift = 0;
    while (true) {
        // Stampa la schermata
        calculate();

        // Leggi l'input
        switch (readKey()) {
            case CTRLC:
                exit(0);
            case ESC:
                return -1;
            case 'w':
            case UP:
                remSelezionato(selezionato);

                remShift(shift, selezionato);
                break;
            case 's':
            case DOWN:
                addSelezionato(selezionato, static_cast<int>(opzioni.size()));

                addShift(shift, selezionato, size, static_cast<int>(opzioni.size()));
                break;
            case ENTER:
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
    opzioniFiltrate.clear();
    const string filtroLower = toLower(filtro);
    for (const string& s : opzioni)
        if (toLower(s).find(filtroLower) != string::npos)
            opzioniFiltrate.emplace_back(s);
}

// Aggiorna la schermata
void SchermataSelettoreFiltrata::calculate() {
    // Intestazione con titolo e filtro
    contenuto = Content(titolo + '\n' + filtro);
    contenuto.data.reserve(contenuto.data.size() + size);

    // Se necessaria, aggiungi ARROW_UP
    if (shift < 0) shift = 0;
    if (shift > 0) [[likely]] {
        string selUpDown;
        selUpDown = printables[ARROW_UP];
        contenuto.data.emplace_back(selUpDown);
    }

    // Aggiungi tutte le opzioni (filtrate)
    string sel;
    for (int i = shift; (i < shift + size) && (i < opzioniFiltrate.size()); i++) {
        if (selezionato == i) [[unlikely]]
            sel = printables[SELECTOR];
        else
            sel = printables[UNSELECTED];
        contenuto.data.emplace_back(sel + " " + opzioniFiltrate[i]);
    }

    // Se necessaria, aggiungi ARROW_DOWN
    if (static_cast<int>(shift) < opzioniFiltrate.size() - size) {
        string selUpDown;
        selUpDown = printables[ARROW_DOWN];
        contenuto.data.emplace_back(selUpDown);
    }
    contenuto.maxSize = getMaxSize(contenuto.data);

    // Imposta il contenuto
    setContenuto();
    print(true);
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

        // Leggi l'input
        switch (const int in = readKey()) {
            case CTRLC:
                exit(0);
            case ESC:
                return -1;
            case UP:
                remSelezionato(selezionato);
                remShift(shift, selezionato);
                break;
            case DOWN:
                addSelezionato(selezionato, static_cast<int>(opzioniFiltrate.size()));

                addShift(shift, selezionato, size, static_cast<int>(opzioniFiltrate.size()));
                break;
            case ENTER:
                if (opzioniFiltrate.empty()) return -1;
                for (int i = 0; i < opzioni.size(); i++)
                    if (opzioni[i] == opzioniFiltrate[selezionato]) return i;
                return -1;
            case BACKSPACE:
                if (!filtro.empty())
                    filtro.pop_back();
                cambioFiltro = true;
                break;
            case DEL:
                filtro = "";
                cambioFiltro = true;
                break;
            default:
                if (in > 32) {
                    filtro += static_cast<char>(in);
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
    // Aggiungi il titolo
    contenuto = Content(titolo);
    contenuto.data.reserve(contenuto.data.size() + 2 * opzioni.size()
        + hasBordi * 2 * ( paddingVertical + 1));

    // Aggiungi tutte le opzioni
    for (int i = 0; i < opzioni.size(); i++) {
        // Aggiungi il titolo dell'opzione
        string toAdd;
        if (i == selezionato) [[unlikely]]
            toAdd += printables[SELECTOR];
        else
            toAdd += printables[UNSELECTED];
        toAdd += " " + opzioni[i].titolo + " ";
        contenuto.data.emplace_back(toAdd);

        // Aggiungi il corpo
        string out;
        switch (opzioni[i].questionType) {
            case INSERIMENTO:
                contenuto.data.emplace_back(opzioni[i].outString);
                break;
            case BOOLEANO:
                out += printables[CONTAIN_LEFT];
                out += opzioni[i].outInt == 1 ? printables[CHECKED] : printables[UNCHECKED];
                out += printables[CONTAIN_RIGHT];
                contenuto.data.emplace_back(out);
                break;
            case SCALA: {
                out += printables[CONTAIN_LEFT];
                for (int s = 0; s < opzioni[i].size; s++) {
                    out += printables[s == opzioni[i].outInt ? CHECKED : UNSELECTED];
                }
                out += printables[CONTAIN_RIGHT];
                contenuto.data.emplace_back(out);
                break;
            }
        }

        // Aggiungi lo spazio sotto
        contenuto.data.emplace_back("");
        contenuto.data.emplace_back("");
    }
    contenuto.maxSize = getMaxSize(contenuto.data);
    // Imposta il contenuto
    setContenuto();
    print(true);
}

void SchermataQuestionario::render() {
    selezionato = 0;

    while (true) {
        calculate();

        switch (const int in = readKey()) {
            case CTRLC:
                exit(0);
            case ESC:
                return;
            case UP:
                remSelezionato(selezionato);
                break;
            case DOWN:
                addSelezionato(selezionato, static_cast<int>(opzioni.size()));
                break;
            case ENTER:
                return;
            default: {
                switch (opzioni[selezionato].questionType) {
                    case INSERIMENTO:
                        if (in == BACKSPACE) {
                            if (!opzioni[selezionato].outString.empty()) opzioni[selezionato].outString.pop_back();
                            calculate();
                        }
                        else if (in == DEL)
                            opzioni[selezionato].outString = "";
                        else opzioni[selezionato].outString += static_cast<char>(in);

                        break;
                    case BOOLEANO:
                        if (in == ' ')
                            opzioni[selezionato].outInt = 1 - opzioni[selezionato].outInt;
                        break;
                    case SCALA:
                        if ((in == '+' || in == RIGHT) && opzioni[selezionato].outInt < opzioni[selezionato].size - 1)
                            opzioni[selezionato].outInt++;
                        if ((in == '-' || in == LEFT) && opzioni[selezionato].outInt > 0)
                            opzioni[selezionato].outInt--;
                        break;
                    default:
                        ;
                }
            }
        }
    }
}