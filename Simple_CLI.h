#pragma once

#include <array>
#include <iostream>
#include <vector>

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
        CONTAIN_RIGHT,

        COUNT
    };

    unsigned int getMaxSize(const std::vector<std::string>&);
    std::vector<std::string> parseStringVector(const std::string&);
    void print(const std::string& contenuto);
    void println(const std::string& contenuto);
    std::string read();
    std::string readln();
    void del(unsigned int count);
    void clear();

    struct Content {
        std::vector<std::string> data = {};
        unsigned int maxSize = 0;

        explicit Content(const std::vector<std::string>& contenuto) : data(contenuto) {
            maxSize = getMaxSize(contenuto);
        }
        explicit Content(const std::string& s) : data(parseStringVector(s)) {
            maxSize = getMaxSize(data);
        }
        Content(const std::vector<std::string>& data, const unsigned int maxSize) : data(data), maxSize(maxSize) {}

        Content& operator += (const std::vector<std::string>& toAdd) {
            data.insert(data.end(), toAdd.begin(), toAdd.end());
            maxSize = std::max(maxSize, getMaxSize(toAdd));
            return *this;
        }
        Content& operator += (const std::string& toAdd) {
            data.emplace_back(toAdd);
            maxSize = std::max(maxSize, static_cast<unsigned int>(toAdd.length()));
            return *this;
        }
    };
    //      -----{ GENERICO }-----

    /*  La classe Schermata serve per funzioni generiche: da sola rappresenta del contenuto
     *  inquadrato in una corice, ma può anche essere la base di qualsiasi altro tipo di schermata
     */
    class Schermata {
    protected:
        Content contenuto = {std::vector<std::string>{}, 0};
        bool hasBordi = false;
    public:
        int paddingHorizontal = 0;
        int paddingVertical = 0;

        /*unordered_map<int, char> printables = {
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
        };*/

        std::array<char, COUNT> printables = {
            '-', //WALL_HORIZONTAL
            '|', //WALL_VERTICAL,
            '=', //THICK,
            ' ', //BLANK,
            '#', //FILL,
            'O', //CIRCLE_BIG,
            'o', //CIRCLE_SMALL,
            '>', //SELECTOR,
            '-', //UNSELECTED,
            '^', //ARROW_UP,
            'v', //ARROW_DOWN,
            '<', //ARROW_RIGHT,
            '>', //ARROW_LEFT,
            'X', //CHECKED,
            ' ', //UNCHECKED,
            '[', //CONTAIN_LEFT,
            ']'  //CONTAIN_RIGHT
        };

        explicit Schermata(const std::string& contenuto, bool bordi);
        explicit Schermata() = default;
        void aggiorna(const std::string &obj, bool bordi);

        void aggiorna(const std::vector<std::string> &obj, bool bordi);

        ~Schermata() = default;
        void print(bool del = false) const;

        void setContenuto(const std::string &cont, bool bordi);
        void setContenuto(const std::vector<std::string> &cont, bool bordi);

        void setContenuto();

        [[nodiscard]] const Content& getContenuto() const;

        [[nodiscard]] std::string getString() const;
    };

    //      -----{ SELETTORI }-----

    /*  La classe SchermataSelettore serve per accettare un input tra una serie di opzioni, ma senza
     *  assegnare alcun carattere particolare alle opzioni
    */
    class SchermataSelettore : public Schermata {
    protected:
        std::string titolo;
        int selezionato = 0;
        std::vector<std::string> opzioni;
        void calculate();
    public:
        explicit SchermataSelettore(std::string  titolo, const std::vector<std::string>& opzioni, bool bordi = false);
        ~SchermataSelettore() = default;
        int render();
    };

    /*  La classe SchermataSelettoreCustom serve per accettare un input tra una serie di opzioni
     *  assegnando pulsanti della tastiera alle opzioni
    */
    class SchermataSelettoreCustom : public Schermata {
    protected:
        int result = -1;
        std::string titolo;
        std::vector<std::string> opzioni;
        std::vector<char> titoliOpzioni;
        void calculate();
    public:
        explicit SchermataSelettoreCustom(std::string titolo, const std::vector<char>& titoliOpzioni,
            const std::vector<std::string>& opzioni, bool autoRender = false, bool bordi = false);
        ~SchermataSelettoreCustom() = default;
        int render();
        [[nodiscard]] char getResult() const;
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
        explicit SchermataSelettoreLarge(std::string titolo, const std::vector<std::string>& opzioni, int size = 9, bool bordi = false);
        ~SchermataSelettoreLarge() = default;
        int render();
    };

    /*  La classe SchermataSelettore serve per accettare un input tra una serie di opzioni, ma senza
     *  assegnare alcun carattere particolare alle opzioni. Si aggiunge la possibilità di scorrimento
     *  della schermata e di filtrare i risultati
    */
    class SchermataSelettoreFiltrata : public SchermataSelettoreLarge {
    protected:
        std::string filtro;
        std::vector<std::string> opzioniFiltrate {};
        void calculate();
        void calculateNewSet();
    public:
        explicit SchermataSelettoreFiltrata(std::string titolo, const std::vector<std::string>& opzioni, int size, bool bordi = false);
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
            std::string titolo;
            questionTypes questionType = BOOLEANO;
            int size = 0;
            int& outInt;
            std::string& outString;
        };
    protected:
        std::vector<formElement> opzioni;
        std::string titolo;
        void calculate();
        int selezionato = 0;
    public:
        SchermataQuestionario(std::string  titolo, std::vector<formElement> opzioni);
        SchermataQuestionario() = delete;
        ~SchermataQuestionario() = default;

        void render();
    };
}