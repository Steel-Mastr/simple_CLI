#pragma once
#include <cstdlib>
#include <string>
#include <vector>

#include "generale.hpp"

namespace schermate {
    /**  La classe SchermataQuestionario serve per accettare un input vari e multipli, navigando tra le
         opzioni con le frecce direzionali
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
            int* outInt = nullptr;
            std::string* outString = nullptr;

            formElement() = default;
            formElement(std::string titolo, const questionTypes questionType
                , const int size, int* outInt, std::string* outString)
            : titolo(std::move(titolo)), questionType(questionType), size(size), outInt(outInt), outString(outString)
            {}
        };
    protected:
        std::vector<formElement> opzioni;
        std::string titolo;

        // Aggiorna la schermata
        void calculate() {
            // Aggiungi il titolo
            contenuto = Content(titolo);
            contenuto.data.reserve(contenuto.data.size() + 2 * opzioni.size()
                + hasBordi * 2 * ( paddingVertical + 1));

            // Aggiungi tutte le opzioni
            for (int i = 0; i < static_cast<int>(opzioni.size()); i++) {
                // Aggiungi il titolo dell'opzione
                std::string toAdd;
                if (i == selezionato) [[unlikely]]
                    toAdd += printables[SELECTOR];
                else
                    toAdd += printables[UNSELECTED];
                toAdd += " " + opzioni[i].titolo + " ";
                contenuto.data.emplace_back(toAdd);

                // Aggiungi il corpo
                std::string out;
                switch (opzioni[i].questionType) {
                    case INSERIMENTO:
                        contenuto.data.emplace_back(*opzioni[i].outString);
                        break;
                    case BOOLEANO:
                        out += printables[CONTAIN_LEFT];
                        out += *opzioni[i].outInt == 1 ? printables[CHECKED] : printables[UNCHECKED];
                        out += printables[CONTAIN_RIGHT];
                        contenuto.data.emplace_back(out);
                        break;
                    case SCALA: {
                        out += printables[CONTAIN_LEFT];
                        for (int s = 0; s < opzioni[i].size; s++) {
                            out += printables[s == *opzioni[i].outInt ? CHECKED : UNSELECTED];
                        }
                        out += printables[CONTAIN_RIGHT];
                        contenuto.data.emplace_back(out);
                        break;
                    }
                }

                // Aggiungi lo spazio sotto
                contenuto.data.emplace_back("");
                contenuto.data.emplace_back("");
            }
            contenuto.maxSize = utils::getMaxSize(contenuto.data);
            // Imposta il contenuto
            setContenuto();
            print(true);
        }
        int selezionato = 0;
    public:
        // Crea una schermata questionario senza renderizzarla
        SchermataQuestionario(std::string  titolo, std::vector<formElement> opzioni) :
            Schermata(), opzioni(std::move(opzioni)), titolo(std::move(titolo)) {}
        SchermataQuestionario() = delete;
        ~SchermataQuestionario() = default;

        // Renderizza la schermata aggiornando il questionario
        void render() {
            selezionato = 0;

            while (true) {
                calculate();

                switch (const int in = terminale::readKey()) {
                    case terminale::CTRLC:
                        std::exit(0);
                    case terminale::ESC:
                        return;
                    case terminale::UP:
                        utils::remSelezionato(selezionato);
                        break;
                    case terminale::DOWN:
                        utils::addSelezionato(selezionato, static_cast<int>(opzioni.size()));
                        break;
                    case terminale::ENTER:
                        return;
                    default: {
                        switch (opzioni[selezionato].questionType) {
                            case INSERIMENTO:
                                if (in == terminale::BACKSPACE) {
                                    if (!opzioni[selezionato].outString->empty())
                                        opzioni[selezionato].outString->pop_back();
                                    calculate();
                                }
                                else if (in == terminale::DEL)
                                    *opzioni[selezionato].outString = "";
                                else opzioni[selezionato].outString += static_cast<char>(in);

                                break;
                            case BOOLEANO:
                                if (in == ' ')
                                    *opzioni[selezionato].outInt = 1 - *opzioni[selezionato].outInt;
                                break;
                            case SCALA:
                                if ((in == '+' || in == terminale::RIGHT)
                                    && *opzioni[selezionato].outInt < opzioni[selezionato].size - 1)
                                    opzioni[selezionato].outInt++;
                                if ((in == '-' || in == terminale::LEFT) && *opzioni[selezionato].outInt > 0)
                                    opzioni[selezionato].outInt--;
                                break;
                            default:
                                ;
                        }
                    }
                }
            }
        }
    };
}