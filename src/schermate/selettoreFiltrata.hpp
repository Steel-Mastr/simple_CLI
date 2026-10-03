#pragma once
#include "selettoreLarge.hpp"

namespace schermate {
    /**  La classe SchermataSelettore serve per accettare un input tra una serie di opzioni, ma senza
         assegnare alcun carattere particolare alle opzioni. Si aggiunge la possibilità di scorrimento
         della schermata e di filtrare i risultati
    */
    class SchermataSelettoreFiltrata : public SchermataSelettoreLarge {
    protected:
        std::string filtro;
        std::vector<std::string> opzioniFiltrate {};

        // Aggiorna la schermata
        void calculate() {
            // Intestazione con titolo e filtro
            contenuto = Content(titolo + '\n' + filtro);
            contenuto.data.reserve(contenuto.data.size() + size);

            // Se necessaria, aggiungi ARROW_UP
            if (shift < 0) shift = 0;
            if (shift > 0) [[likely]] {
                std::string selUpDown;
                selUpDown = printables[ARROW_UP];
                contenuto.data.emplace_back(selUpDown);
            }

            // Aggiungi tutte le opzioni (filtrate)
            std::string sel;
            for (int i = shift; (i < shift + size) && (i < static_cast<int>(opzioniFiltrate.size())); i++) {
                if (selezionato == i) [[unlikely]]
                    sel = printables[SELECTOR];
                else
                    sel = printables[UNSELECTED];
                contenuto.data.emplace_back(sel + " " + opzioniFiltrate[i]);
            }

            // Se necessaria, aggiungi ARROW_DOWN
            if (static_cast<int>(shift) < static_cast<int>(opzioniFiltrate.size()) - size) {
                std::string selUpDown;
                selUpDown = printables[ARROW_DOWN];
                contenuto.data.emplace_back(selUpDown);
            }
            contenuto.maxSize = utils::getMaxSize(contenuto.data);

            // Imposta il contenuto
            setContenuto();
            print(true);
        }

        // Applica i filtri
        void calculateNewSet() {
            opzioniFiltrate.clear();
            const std::string filtroLower = utils::toLower(filtro);
            for (const std::string& s : opzioni)
                if (utils::toLower(s).find(filtroLower) != std::string::npos)
                    opzioniFiltrate.emplace_back(s);
        }
    public:
        // Crea una schermata selettore filtrata secondo le regole di schermata selettore
        SchermataSelettoreFiltrata(std::string titolo, const std::vector<std::string> &opzioni
            , const int size, const bool bordi)
            : SchermataSelettoreLarge(std::move(titolo), opzioni, size, bordi) {}

        ~SchermataSelettoreFiltrata() = default;

        // Renderizza la schermata restituendo l'indice della selezione
        int render() {
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
                switch (const int in = terminale::readKey()) {
                    case terminale::CTRLC:
                        std::exit(0);
                    case terminale::ESC:
                        return -1;
                    case terminale::UP:
                        utils::remSelezionato(selezionato);
                        utils::remShift(shift, selezionato);
                        break;
                    case terminale::DOWN:
                        utils::addSelezionato(selezionato,
                            static_cast<int>(opzioniFiltrate.size()));
                        utils::addShift(shift, selezionato, size,
                            static_cast<int>(opzioniFiltrate.size()));
                        break;
                    case terminale::ENTER:
                        if (opzioniFiltrate.empty()) return -1;
                        if (selezionato >= static_cast<int>(opzioniFiltrate.size())) return -1;
                        for (int i = 0; i < static_cast<int>(opzioni.size()); i++)
                            if (opzioni[i] == opzioniFiltrate[selezionato]) return i;
                        return -1;
                    case terminale::BACKSPACE:
                        if (!filtro.empty())
                            filtro.pop_back();
                        cambioFiltro = true;
                        break;
                    case terminale::DEL:
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
    };
}