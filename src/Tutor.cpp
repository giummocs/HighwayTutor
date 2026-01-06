//FILE REALIZZATO DA STEFANI GIANMARIA

#include "DataProcessor.h"
#include <iostream>
#include <memory>

//Separa il comando da eventuali parametri
void splitInput(std::string line, std::string& command, std::string& parameter){
    std::stringstream ss(line);
    std::string temp;
    std::vector<std::string> words;

    //Legge parola per parola
    while (ss >> temp) {
        words.push_back(temp);
    }

    //Conta quante parole sono presenti
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


int main(int argsNumber, char* args[]) {
    std::string line = "";
    std::string command = "";
    std::string parameter = "";
    bool done = false;
    std::unique_ptr<DataProcessor> tutor = nullptr; //Puntatore

    //Lettura del nome del file highway.txt
    if (argsNumber != 2) {
        std::cerr << "Usare il comando: ./Tutor <nome_file>" << std::endl;
        return 1;
    }
    std::string filename = "../Data/"+args[1];

    //Crea un oggetto DataProcessor, che tramite costruttore effettua lettura dei file Highway.txt e Passsages.txt e riempimento dei dati
    //In caso di errore nella lettura dei file lancia un eccezione
    try {
        tutor = std::make_unique<DataProcessor>(filename,"../Data/Passages.txt");
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
    

    std::cout << "------BENVENUTO IN TUTOR AUTOSTRADALE------" << std::endl;;

    //Lettura da standard input di un comando
    while (!done) {
        std::cout << "\nInserisci un comando>>";

        //Legge la riga, in caso di errore stampa un messaggio e procedi a una nuova lettura (salta a un nuovo ciclo del while)
        if (!std::getline(std::cin, line)) {
            std::cerr << "Errore nella lettura riga" << std::endl;
            continue;
        }

        //Separa il comando dai parametri
        splitInput(line, command, parameter);

        //Chiama la funzione corrispondente
        if (command == "set_time" && parameter != "") {
            try{
                 std::cout << tutor->set_time(parameter) << std::endl;
            }
            catch(const std::runtime_error& e){
                 std::cerr << e.what() << std::endl;
            }
        }
        else if (command == "stats") {
            std::cout << tutor->stats() << std::endl;
        }
        else if (command == "reset") {
            std::cout << tutor->reset() << std::endl;
        }
        else if (command == "q" || command == "Q") {
            std::cout << "Uscita in corso..." << std::endl;
            done = true;
        }    
        else {
            std::cerr << "Errore! Comando non valido!" << std::endl;
        }

        //Reset dei comandi
        line = "";
        command = "";
        parameter = "";

    }
    return 0;

}



























