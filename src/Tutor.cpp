
#include "DataProcessor.h";
#include <memory>

void splitInput(std::string input, std::string& command, std::string& parameter){
    //Separo il comando da eventuali parametri
    size_t splitPoint = input.find(' ');
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

    std::unique_ptr<DataProcessor> dp = nullptr;

    //Lettura dei file Highway.txt e Passsages.txt e riempimento dei dati
    try {
        dp = std::make_unique<DataProcessor>("Data/Highway.txt","Data/Passages.txt");
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1; //Interrompe il main
    }

    std::cout << "------BENVENUTO IN TUTOR AUTOSTRADALE------";
    std::cout << "Inserisci un comando>>";

    while (std::cin >> input) {
        
        splitInput(input, command, parameter);

        if (command == "set_time" && parameter != "") {
            try{
                std::cout << dp->set_time(parameter);
            }
            catch(const std::runtime_error& e){
                std::cerr << e.what() << std::endl;
            }
            
            std::cout << "Inserisci un comando>>";
        } 
        else if (command == "stats" && parameter == "") {
            std::cout << dp->stats();
            std::cout << "Inserisci un comando>>";
        } 
        else if (command == "reset" && parameter == "") {
            std::cout << dp->reset();
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

