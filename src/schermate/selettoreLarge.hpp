#pragma once
#include "selettore.hpp"

namespace schermate {
    /**  La classe SchermataSelettore serve per accettare un input tra una serie di opzioni, ma senza
         assegnare alcun carattere particolare alle opzioni. Si aggiunge la possibilità di scorrimento
         della schermata
    */
    class SchermataSelettoreLarge : public SchermataSelettore {
    protected:
        int size;
        int shift = 0;
        bool trueLarge = false;

        // Aggiorna la schermata
        void calculate() {
            // Intestazione con il titolo
            contenuto = Content(titolo);
            contenuto.data.reserve(contenuto.data.size() + size
                + hasBordi * 2 * (paddingVertical + 1));
            shift = std::max(shift, 0);
            if (shift > 0) [[likely]] {
                std::string toAdd;
                toAdd += printables[ARROW_UP];
                contenuto.data.emplace_back(toAdd);
            }
            // Aggiungi tutte le opzioni
            std::string sel;
            for (int i = shift; (i < shift + size) && (i < static_cast<int>(opzioni.size())); i++) {
                if (selezionato == i) [[unlikely]]
                    sel = printables[SELECTOR];
                else
                    sel = printables[UNSELECTED];
                contenuto.data.emplace_back(sel + " " + opzioni[i]);
            }

            // Se necessaria, aggiungi ARROW_DOWN
            if (shift < static_cast<int>(opzioni.size()) - size) {
                std::string toAdd;
                toAdd += printables[ARROW_DOWN];
                contenuto.data.emplace_back(toAdd);
            }
            contenuto.maxSize = utils::getMaxSize(contenuto.data);
            // Imposta il contenuto
            setContenuto();
            print(true);
        }
    public:
        // Crea una schermata selettore large senza renderizzarla
        SchermataSelettoreLarge(std::string titolo, const std::vector<std::string> &opzioni
            , const int size, const bool bordi) :
            SchermataSelettore(std::move(titolo), opzioni)
        {
            this->hasBordi = bordi;
            this->size = size > 2 ? size : 3;
            // size_t: senza il cast il confronto con int è sotto overflow quando opzioni è vuota
            trueLarge = opzioni.size() > static_cast<std::size_t>(this->size);
        }
        ~SchermataSelettoreLarge() = default;

        // Renderizza la schermata restituendo l'indice della selezione
        int render() {
            selezionato = 0;
            shift = 0;
            while (true) {
                // Stampa la schermata
                calculate();

                // Leggi l'input
                switch (terminale::readKey()) {
                    case terminale::CTRLC:
                        std::exit(0);
                    case terminale::ESC:
                        return -1;
                    case 'w':
                    case terminale::UP:
                        utils::remSelezionato(selezionato);
                        utils::remShift(shift, selezionato);
                        break;
                    case 's':
                    case terminale::DOWN:
                        utils::addSelezionato(selezionato,
                            static_cast<int>(opzioni.size()));
                        utils::addShift(shift, selezionato, size,
                            static_cast<int>(opzioni.size()));
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