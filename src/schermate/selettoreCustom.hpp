#pragma once
#include <stdexcept>
#include <string>
#include <vector>

#include "generale.hpp"

namespace schermate {
    /**  La classe SchermataSelettoreCustom serve per accettare un input tra una serie di opzioni
         assegnando pulsanti della tastiera alle opzioni
    */
    class SchermataSelettoreCustom : public Schermata {
    protected:
        int result = -1;
        std::string titolo;
        std::vector<std::string> opzioni;
        std::vector<char> titoliOpzioni;

        // Aggiorna la schermata
        void calculate() {
            // Intestazione con il titolo
            contenuto = Content(titolo);

            // Aggiungi tutte le opzioni
            std::string sel;
            for (int i = 0; i < static_cast<int>(titoliOpzioni.size()); i++) {
                sel = titoliOpzioni[i];
                contenuto.data.emplace_back(sel + ": " + opzioni[i]);
            }
            contenuto.maxSize = utils::getMaxSize(contenuto.data);
            // Imposta il contenuto
            setContenuto();
        }
    public:
        // Crea una schermata selettore custom e renderizza automaticamente il contenuto - impostare autoRender su true per renderizzare subito
        SchermataSelettoreCustom(std::string titolo, const std::vector<char>& titoliOpzioni,
            const std::vector<std::string>& opzioni, const bool autoRender = true, const bool bordi = false) :
            titolo(std::move(titolo)), opzioni(opzioni), titoliOpzioni(titoliOpzioni) {
            if (titoliOpzioni.size() != opzioni.size())
                throw std::length_error("Lunghezza vettori inconsistente tra titoliOpzioni ed opzioni");
            // Renderizza la schermata se richiesto
            this->hasBordi = bordi;
            if (autoRender) render();
        }
        ~SchermataSelettoreCustom() = default;

        // Renderizza la schermata, restituisce l'indice del risultato
        int render() {
            // Stampa il contenuto a schermo
            calculate();
            print(true);

            // Cerca il valore inserito
            const int input = terminale::readKey();
            if (input == terminale::CTRLC) std::exit(0);
            if (input == terminale::ESC) return -1;
            for (int i = 0; i < static_cast<int>(titoliOpzioni.size()); i++)
                if (input == titoliOpzioni[i]) {
                    result = i;
                    return i;
                }
            return -1;
        }

        // Restituisce il char corrispondente al risultato ('\0' se non c'è risultato)
        [[nodiscard]] char getResult() const {
            if (result < 0) return '\0';
            return titoliOpzioni[static_cast<std::size_t>(result)];
        }

        // Restituisce il numero corrispondente al risultato (-1 = niente)
        [[nodiscard]] int getResultNumber() const {
            return result;
        }
    };
}