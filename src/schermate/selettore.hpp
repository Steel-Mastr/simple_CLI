#pragma once
#include "generale.hpp"

namespace schermate {
    /**  La classe SchermataSelettore serve per accettare un input tra una serie di opzioni, ma senza
         assegnare alcun carattere particolare alle opzioni
    */
    class SchermataSelettore : public Schermata {
    protected:
        std::string titolo;
        int selezionato = 0;
        std::vector<std::string> opzioni;

        // Aggiorna la schermata
        void calculate() {
            const std::vector<std::string> titoloParsed = utils::parseStringVector(titolo);
            // Intestazione con il titolo
            contenuto = Content(titoloParsed);
            contenuto.data.reserve(2 * paddingVertical + hasBordi * 2 + titoloParsed.size() + opzioni.size());

            // Aggiungi tutti gli elementi della lista
            std::string sel;
            for (int i = 0; i < static_cast<int>(opzioni.size()); i++) {
                if (i == selezionato) [[unlikely]]
                    sel = printables[SELECTOR];
                else
                    sel = printables[UNSELECTED];
                contenuto.data.emplace_back(sel + " " + opzioni[i]);
            }
            contenuto.maxSize = utils::getMaxSize(contenuto.data);
            setContenuto();
            print(true);
        }
    public:
        // Crea una schermata selettore standard, ma non la renderizza
        SchermataSelettore(std::string titolo, const std::vector<std::string>& opzioni,
            const bool bordi = false)
            : Schermata()
            , titolo(std::move(titolo))
            , opzioni(opzioni)
        {
            this->hasBordi = bordi;
        }

        ~SchermataSelettore() = default;

        // Mostra la schermata in attesa di un invio, al che restituisce l'indice selezionato
        int render() {
            selezionato = 0;
            while (true) {
                // Stampa la schermata
                calculate();

                // Leggi l'ingresso
                switch (terminale::readKey()) {
                    case terminale::CTRLC:
                        std::exit(0);
                    case terminale::ESC:
                        return -1;
                    case 'w':
                    case terminale::UP:
                        utils::remSelezionato(selezionato);
                        break;
                    case 's':
                    case terminale::DOWN:
                        utils::addSelezionato(selezionato, static_cast<int>(opzioni.size()));
                        break;
                    case terminale::ENTER:
                        return selezionato;
                    default:
                        ;
                }
            }
        }
    };
}
