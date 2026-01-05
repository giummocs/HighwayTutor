//FILE REALIZZATO DA STEFANI GIANMARIA

#include "DataProcessor.h"
#include <iostream>
#include <memory>

//Separa il comando da eventuali parametri
void splitInput(std::string input, std::string& command, std::string& parameter){
    //Find ritorna un l'indice se trova ' ', altrimenti ritorna un valore molto alto
    size_t splitPoint = input.find(' ');

    //Se l'indice è inferiore alla lunghezza della stringa allora find ha trovato ' ' e quindi esistono dei parametri
    if (splitPoint < input.size()) {
        command = input.substr(0, splitPoint);
        parameter  = input.substr(splitPoint + 1);
    } else {
        command = input;
        parameter  = "";
    }
}


int main() {

    std::string line;
    std::string command;
    bool done = false;

    //Puntatore
    std::unique_ptr<DataProcessor> tutor = nullptr;

    //Crea un oggetto DataProcessor, che tramite costruttore effettua lettura dei file Highway.txt e Passsages.txt e riempimento dei dati
    try {
        tutor = std::make_unique<DataProcessor>("Data/Highway.txt","Data/Passages.txt");
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1; //Interrompe il main
    }

    std::cout << "------BENVENUTO IN TUTOR AUTOSTRADALE------";

    //Attende un input
    while (!done) {
        std::cout << "\nInserisci un comando>>";

        if (!std::getline(std::cin, line)) break;

        std::stringstream ss(line);
        ss >> command;

        //Chiama la funzione corrispondente
        if (command == "set_time") {
            int parameter;
            
            if (ss >> parameter) {
                try{
                    std::cout << tutor->set_time(parameter);
                }
                catch(const std::runtime_error& e){
                    std::cerr << e.what() << std::endl;
                }
            }
            else {
                std::cout << "Errore: set_time richiede un numero intero." << std::endl;
            }
        } 
        else if (command == "stats") {
            std::cout << tutor->stats();
        }
        else if (command == "reset") {
            std::cout << tutor->reset();
        }
        else if (command == "q" || command == "Q") {
            std::cout << "Uscita in corso..." << std::endl;
            done = true;
        }    
        else {
            std::cerr << "\nErrore! Commando non valido!";
        }

    }
    return 0;

}










