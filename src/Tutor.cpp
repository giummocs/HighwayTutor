//FILE REALIZZATO DA STEFANI GIANMARIA

#include "DataProcessor.h"
#include <iostream>
#include <memory>

//Separa il comando da eventuali parametri
void splitInput(std::string line, std::string& command, std::string& parameter){
    std::stringstream ss(line);
    std::string temp;
    std::vector<std::string> words;
    while (ss >> temp) {
        words.push_back(temp);
    }
    if (words.size() == 1 ) {
        command = words[0];
    }
    else if (words.size() == 2) {
        command = words[0];
        parameter = words[1];
    }
    else {
        command = "";
    }
}


int main() {

    std::string line = "";
    std::string command = "";
    std::string parameter = "";
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

        splitInput(line, command, parameter);

        //Chiama la funzione corrispondente
        if (command == "set_time") {
        
            try{
                 std::cout << tutor->set_time(parameter);
            }
            catch(const std::runtime_error& e){
                 std::cerr << e.what() << std::endl;
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
            std::cerr << "Errore! Comando non valido!";
        }

    }
    return 0;

}














