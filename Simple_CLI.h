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
        string contenuto;
        explicit Schermata(const vector<string>& contenuto);
        explicit Schermata(const string& contenuto);
        explicit Schermata() = default;
        void aggiorna(const vector<string>& obj);
        void aggiorna(const string& obj);

        ~Schermata() = default;
        void print() const;
    };

    //      -----{ SELETTORI }-----

    /*  La classe SchermataSelettore serve per accettare un input tra una serie di opzioni, ma senza
     *  assegnare alcun carattere particolare alle opzioni
    */
    class SchermataSelettore : public Schermata {
        string titolo;
        vector<string> opzioni;
        void calculate(int index) const;
        int selezionato = 0;
    public:
        explicit SchermataSelettore(string  titolo, const vector<string>& opzioni);
        int render() const;
    };

    class SchermataSelettoreCustom : public Schermata {
        int result = -1;
        string titolo;
        vector<string> opzioni;
        vector<char> titoliOpzioni;
    public:
        explicit SchermataSelettoreCustom(string titolo, const vector<char>& titoliOpzioni,
            const vector<string>& opzioni, bool autoRender = true);
        int render();
        int getResult() const;
    };
}