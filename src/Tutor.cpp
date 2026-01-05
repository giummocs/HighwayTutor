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

    std::string input = "";
    std::string command = "";
    std::string parameter = "";

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
    std::cout << "Inserisci un comando>>";

    //Attende un input
    while (std::cin >> input) {

        //Separa il comando dai parametri
        splitInput(input, command, parameter);

        //Chiama la funzione corrispondente
        if (command == "set_time" && parameter != "") {
            try{
                std::cout << tutor->set_time(parameter);
            }
            catch(const std::runtime_error& e){
                std::cerr << e.what() << std::endl;
            }
            
            std::cout << "Inserisci un comando>>";
        } 
        else if (command == "stats" && parameter == "") {
            std::cout << tutor->stats();
            std::cout << "Inserisci un comando>>";
        } 
        else if (command == "reset" && parameter == "") {
            std::cout << tutor->reset();
            std::cout << "Inserisci un comando>>";
        }
        else if (command == "Q" || command == "q"){
            return 0;
        }
        else {
            std::cerr << "Errore! Commando invalido!";
            std::cout << "Inserisci un comando>>";
        }

    }
    return 0;

}





