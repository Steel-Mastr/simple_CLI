#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

#ifdef _WIN32
    #include <conio.h>
    #define GETCH() _getch()
#else
    #include <termios.h>
    #include <unistd.h>

    termios _originalTermios;

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &_originalTermios);
    termios raw = _originalTermios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    atexit([]() {
        tcsetattr(STDIN_FILENO, TCSANOW, &_originalTermios);
    });
}

char _getch_unix() {
    return static_cast<char>(getchar());
}

#define GETCH() _getch_unix()
#endif

enum Key { UP = 256, DOWN, LEFT, RIGHT, ENTER, ESC, BACKSPACE, DEL, CTRLC };

#ifndef _WIN32
#define SETUP  enableRawMode()
#else
#define SETUP /* Non necessario */
#endif

namespace schermate {
    using namespace std;
    inline unsigned int righeStampate = 0;
    enum PrintableTypes {
        WALL_HORIZONTAL,
        WALL_VERTICAL,
        THICK,
        BLANK,
        FILL,
        CIRCLE_BIG,
        CIRCLE_SMALL,
        SELECTOR,
        UNSELECTED,
        ARROW_UP,
        ARROW_DOWN,
        ARROW_RIGHT,
        ARROW_LEFT,
        CHECKED,
        UNCHECKED,
        CONTAIN_LEFT,
        CONTAIN_RIGHT
    };

    unsigned int getMaxSize(const vector<string>&);
    vector<string> parseStringVector(const string&);
    void print(const string& contenuto);
    void println(const string& contenuto);
    void del(unsigned int count);
    void clear();

    struct Content {
        vector<string> data = {};
        unsigned int maxSize = 0;

        explicit Content(const vector<string>& contenuto) : data(contenuto) {
            maxSize = getMaxSize(contenuto);
        }
        explicit Content(const string& s) : data(parseStringVector(s)) {
            maxSize = getMaxSize(data);
        }
        Content(const vector<string>& data, const unsigned int maxSize) : data(data), maxSize(maxSize) {}

        Content& operator += (const vector<string>& toAdd) {
            data.insert(data.end(), toAdd.begin(), toAdd.end());
            maxSize = max(maxSize, getMaxSize(toAdd));
            return *this;
        }
        Content& operator += (const string& toAdd) {
            data.emplace_back(toAdd);
            maxSize = max(maxSize, static_cast<unsigned int>(toAdd.length()));
            return *this;
        }
    };
    //      -----{ GENERICO }-----

    /*  La classe Schermata serve per funzioni generiche: da sola rappresenta del contenuto
     *  inquadrato in una corice, ma può anche essere la base di qualsiasi altro tipo di schermata
     */
    class Schermata {
    protected:
        Content contenuto = {vector<string>{}, 0};
        bool hasBordi = false;
    public:
        int paddingHorizontal = 0;
        int paddingVertical = 0;

        unordered_map<int, char> printables = {
            {WALL_HORIZONTAL, '-'},
            {WALL_VERTICAL, '|'},
            {THICK, '='},
            {BLANK, ' '},
            {FILL, '#'},
            {CIRCLE_BIG, 'O'},
            {CIRCLE_SMALL, 'o'},
            {SELECTOR, '>'},
            {UNSELECTED, '-'},
            {ARROW_UP, '^'},
            {ARROW_DOWN, 'v'},
            {ARROW_LEFT, '<'},
            {ARROW_RIGHT, '>'},
            {CHECKED, 'X'},
            {UNCHECKED, ' '},
            {CONTAIN_LEFT, '['},
            {CONTAIN_RIGHT, ']'}
        };

        explicit Schermata(const string& contenuto, bool bordi);
        explicit Schermata() = default;
        void aggiorna(const string &obj, bool bordi);

        void aggiorna(const vector<string> &obj, bool bordi);

        ~Schermata() = default;
        void print(bool del = false) const;

        void setContenuto(const string &cont, bool bordi);
        void setContenuto(const vector<string> &cont, bool bordi);

        void setContenuto();

        const Content& getContenuto() const;

        [[nodiscard]] string getString() const;
    };

    //      -----{ SELETTORI }-----

    /*  La classe SchermataSelettore serve per accettare un input tra una serie di opzioni, ma senza
     *  assegnare alcun carattere particolare alle opzioni
    */
    class SchermataSelettore : public Schermata {
    protected:
        string titolo;
        int selezionato = 0;
        vector<string> opzioni;
        void calculate();
    public:
        explicit SchermataSelettore(string  titolo, const vector<string>& opzioni, bool bordi = false);
        ~SchermataSelettore() = default;
        int render();
    };

    /*  La classe SchermataSelettoreCustom serve per accettare un input tra una serie di opzioni
     *  assegnando pulsanti della tastiera alle opzioni
    */
    class SchermataSelettoreCustom : public Schermata {
    protected:
        int result = -1;
        string titolo;
        vector<string> opzioni;
        vector<char> titoliOpzioni;
        void calculate();
    public:
        explicit SchermataSelettoreCustom(string titolo, const vector<char>& titoliOpzioni,
            const vector<string>& opzioni, bool autoRender = false, bool bordi = false);
        ~SchermataSelettoreCustom() = default;
        int render();
        char getResult() const;
    };

    /*  La classe SchermataSelettore serve per accettare un input tra una serie di opzioni, ma senza
     *  assegnare alcun carattere particolare alle opzioni. Si aggiunge la possibilità di scorrimento
     *  della schermata
    */
    class SchermataSelettoreLarge : public SchermataSelettore {
    protected:
        int size;
        int shift = 0;
        bool trueLarge = false;
    public:
        void calculate();
        explicit SchermataSelettoreLarge(string titolo, const vector<string>& opzioni, int size, bool bordi = false);
        ~SchermataSelettoreLarge() = default;
        int render();
    };

    /*  La classe SchermataSelettore serve per accettare un input tra una serie di opzioni, ma senza
     *  assegnare alcun carattere particolare alle opzioni. Si aggiunge la possibilità di scorrimento
     *  della schermata e di filtrare i risultati
    */
    class SchermataSelettoreFiltrata : public SchermataSelettoreLarge {
    protected:
        string filtro;
        vector<string> opzioniFiltrate {};
        void calculate();
        void calculateNewSet();
    public:
        explicit SchermataSelettoreFiltrata(string titolo, const vector<string>& opzioni, int size, bool bordi = false);
        ~SchermataSelettoreFiltrata() = default;
        int render();
    };

    /*  La classe SchermataQuestionario serve per accettare un input vari e multipli, navigando tra le
     *  opzioni con le frecce direzionali
    */
    class SchermataQuestionario : public Schermata {
    public:
        enum questionTypes {
            INSERIMENTO,
            BOOLEANO,
            SCALA
        };

        struct formElement {
            string titolo;
            questionTypes questionType = BOOLEANO;
            int size = 0;
            int& outInt;
            string& outString;
        };
    protected:
        vector<formElement> opzioni;
        string titolo;
        void calculate();
        int selezionato = 0;
    public:
        SchermataQuestionario(string  titolo, vector<formElement> opzioni);
        SchermataQuestionario() = delete;
        ~SchermataQuestionario() = default;

        void render();
    };
}