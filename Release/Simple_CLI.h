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
    };
    //      -----{ GENERICO }-----

    /*  La classe Schermata serve per funzioni generiche: da sola rappresenta del contenuto
     *  inquadrato in una corice, ma può anche essere la base di qualsiasi altro tipo di schermata
     */
    class Schermata {
    protected:
        string contenuto;
    public:
        unordered_map<int, char> printables = {
            {WALL_HORIZONTAL, '-'},
            {WALL_VERTICAL, '|'},
            {THICK, '='},
            {BLANK, ' '},
            {FILL, '#'},
            {CIRCLE_BIG, 'O'},
            {CIRCLE_SMALL, 'o'},
            {SELECTOR, '>'},
            {UNSELECTED, '-'}
        };

        explicit Schermata(const vector<string>& contenuto);
        explicit Schermata(const string& contenuto);
        explicit Schermata() = default;
        void aggiorna(const vector<string>& obj);
        void aggiorna(const string& obj);

        ~Schermata() = default;
        void print(bool del = false) const;
        void setContenuto(const string& cont);
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
        void calculate() const;
    public:
        explicit SchermataSelettore(string  titolo, const vector<string>& opzioni);
        ~SchermataSelettore() = default;
        int render();
    };

    class SchermataSelettoreCustom : public Schermata {
    protected:
        int result = -1;
        string titolo;
        vector<string> opzioni;
        vector<char> titoliOpzioni;
    public:
        explicit SchermataSelettoreCustom(string titolo, const vector<char>& titoliOpzioni,
            const vector<string>& opzioni, bool autoRender = false);
        ~SchermataSelettoreCustom() = default;
        int render();
        char getResult() const;
    };

    class SchermataSelettoreLarge : public SchermataSelettore {
    protected:
        int size;
        int shift = 0;
        bool trueLarge = false;
        void calculate();
    public:
        string moreUp = "[...]";
        string moreDown = "[...]";
        explicit SchermataSelettoreLarge(string titolo, const vector<string>& opzioni, int size);
        ~SchermataSelettoreLarge() = default;
        int render();
    };

    class SchermataSelettoreFiltrata : public SchermataSelettoreLarge {
    protected:
        string filtro;
        vector<string> opzioniFiltrate {};
        void calculate();
        void calculateNewSet();
    public:
        explicit SchermataSelettoreFiltrata(string titolo, const vector<string>& opzioni, int size);
        ~SchermataSelettoreFiltrata() = default;
        int render();
    };
}