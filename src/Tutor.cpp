
#include "DataProcess.h";

int main() {

    std::string input = "";
    std::string command = "";
    std::string parameter = "";

    //Lettura del file Highway.txt
    try {
        Highway highway("Data/Highway.txt");
        gates = highway.getGates();
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1; //Interrompe il main
    }

    //Lettura del file Passages.txt
    try {
        readFromFile("Data/Passages.txt");
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1; //Interrompe il main
    }

    std::cout << "------BENVENUTO IN TUTOR AUTOSTRADALE------";
    std::cout << "Inserisci un comando>>";

    while (std::cin >> input) {
        
        //Separo il comando da eventuali parametri
        size_t splitPoint = input.find(' ');
        if (splitPoint < input.size()) {
            command = input;
            parameter  = "";
        } else {
            command = input.substr(0, splitPoint);
            parameter  = input.substr(splitPoint + 1);
        }


        if (command == "set_time" && parameter != "") {
            try{
                std::cout << set_time(decodeInput(parameter));
            }
            catch(const std::runtime_error& e){
                std::cerr << e.what() << std::endl;
            }
            
            std::cout << "Inserisci un comando>>";
        } 
        else if (command == "stats" && parameter == "") {
            
            std::cout << "Inserisci un comando>>";
        } 
        else if (command == "reset" && parameter == "") {
            std::cout << reset();
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
