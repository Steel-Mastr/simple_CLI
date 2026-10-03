#pragma once
#include <array>
#include <cstdlib>
#include <string>
#include <vector>

#include "../_base_/4_content.h"

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
        CONTAIN_RIGHT,

        COUNT
    };
    /**  La classe Schermata serve per funzioni generiche: da sola rappresenta del contenuto
         inquadrato in una corice, ma può anche essere la base di qualsiasi altro tipo di schermata
     */
    class Schermata {
    protected:
        Content contenuto = {std::vector<std::string>{}, 0};
        bool hasBordi = false;
    public:
        int paddingHorizontal = 0;
        int paddingVertical = 0;

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

        // Costruttori - accetta una stringa unica con righe separate da '/n'
        Schermata(const std::string& contenuto, const bool bordi = false) {
            // Richiama la funziona aggiorna
            aggiorna(contenuto, bordi);
        }
        // Costruttori - accetta un oggetto Content
        Schermata(const Content& contenuto, const bool bordi = false) {
            // Richiama la funzione aggiorna
            aggiorna(contenuto, bordi);
        }
        explicit Schermata() = default;

        // Aggiornamento - accetta una stringa unica con righe separate da '/n'
        void aggiorna(const std::string& obj, const bool bordi = false) {
            hasBordi = bordi;
            contenuto = Content(utils::parseStringVector(obj));
            if (!bordi)
                return;

            contenuto.data = aggiungiBordi(contenuto,
                printables[WALL_HORIZONTAL],
                printables[WALL_VERTICAL], paddingHorizontal, paddingVertical);
            contenuto.maxSize += 2 * paddingHorizontal + 2;
        }
        // Aggiornamento - accetta un vettore di stringhe
        void aggiorna(const std::vector<std::string>& obj, const bool bordi= false) {
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
        // Aggiornamento - accetta un oggetto Content
        void aggiorna(const Content& obj, const bool bordi= false) {
            hasBordi = bordi;
            contenuto = obj;
            if (!bordi)
                return;

            contenuto.data = aggiungiBordi(contenuto,
                printables[WALL_HORIZONTAL],
                printables[WALL_VERTICAL], paddingHorizontal, paddingVertical);
            contenuto.maxSize += 2 * paddingHorizontal + 2;
        }

        ~Schermata() = default;
        // Stampa a schermo della schermata - cancella lo schermo prima se si imposta del su true
        void print(const bool del = false) const {
            if (!del) [[unlikely]] {
                for (const std::string& s : contenuto.data) {
                    std::cout << s << '\n';
                }
                righeStampate += contenuto.data.size();
                std::cout << std::flush;
                return;
            }
            clear();
            for (const std::string& s : contenuto.data) {
                std::cout << s << '\n';
            }
            righeStampate = contenuto.data.size();
            std::cout << std::flush;
        }

        // Imposta il contenuto
        void setContenuto(const std::string& cont, const bool bordi = false) {
            aggiorna(cont, bordi);
        }
        // Imposta il contenuto
        void setContenuto(const std::vector<std::string>& cont, const bool bordi = false) {
            aggiorna(cont, bordi);
        }

        // Imposta il contenuto automaticamente
        void setContenuto() {
            if (hasBordi)
                contenuto = {
                aggiungiBordi(
                    contenuto,
                    printables[WALL_HORIZONTAL],
                    printables[WALL_VERTICAL],
                    paddingHorizontal,
                    paddingVertical
                    ),
                contenuto.maxSize + 2 * paddingHorizontal + 2
            };
        }

        // Leggi il contenuto
        [[nodiscard]] const Content& getContenuto() const {
            return contenuto;
        }

        // Leggi il contenuto sotto forma di stringa
        [[nodiscard]] std::string getString() const {
            return utils::parseString(contenuto.data);
        }
    };
}