#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

namespace schermate {
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
    //      -----{ GENERICO }-----

    /*  La classe Schermata serve per funzioni generiche: da sola rappresenta del contenuto
     *  inquadrato in una corice, ma può anche essere la base di qualsiasi altro tipo di schermata
     */
    class Schermata {
    protected:
        string contenuto;
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

        explicit Schermata(string& contenuto, bool bordi);
        explicit Schermata() = default;
        void aggiorna(string& obj, bool bordi);

        ~Schermata() = default;
        void print(bool del = false) const;
        void setContenuto(string& cont, bool bordi);
        string getContenuto();
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
        SchermataQuestionario() = default;
        ~SchermataQuestionario() = default;

        void render();
    };
}