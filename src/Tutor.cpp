#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

double currentTime = 0;
std::unordered_map<std::string, std::vector<PassagePlateKey>> passagesPlateKey;  //POSSIBILE PROBLEMA DI MEMORIA??? TROPPI DATI 
std::unordered_map<int, std::vector<PassageIdKey>> passagesIdKey; 

struct PassagePlateKey {
    int id;
    double time;
};

struct PassageIdKey {
    std::string plate;
    double time;
};

bool readFromFile(const std::string& filename){
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        
        std::stringstream ss(line);
        std::string singleWord;
        std::vector<std::string> words;

        while (ss >> singleWord) {
            words.push_back(singleWord);
        }

        if (words.size() == 3) {
            try {
                int id = std::stoi(words[0]);
                std::string plate = words[1];
                double time = std::stod(words[3]);
                passagesPlateKey[plate].push_back({id, time});
                passagesIdKey[id].push_back({plate, time});
            } catch (...) {
                return false;
            }
        }
    }
}

std::string set_time(int newTime){


    if(newTime <= 0){
        return "Errore! Inserire un argomento in un formato valido!";
    }

    currentTime += newTime;
}

//Traduce il tempo ricevuto come parametro da stringa a intero
int decodeInput(const std::string& s){
    bool hasM = false;
    std::string numberPart;
    int result;

    //Controllo ultimo carattere e tolgo m
    if (s.back() == 'm') {
        hasM = true;
        numberPart = s.substr(0, s.size() - 1);
    } else {
        numberPart = s;
    }

    //La parte numerica deve esistere
    if (numberPart.empty()) return -1;

    //Conversione
    try {
        result = std::stoi(numberPart);
    } catch (...) {
        return -1;
    }
    

    //Se aveva m moltiplico per 60 per convertire in secondi
    if (hasM) {
        result *= 60;
    }

    return result;
}

int main() {
    
    std::string command;
    

    while (std::cin >> command) {
        if (command == "set_time") {
            double value;
            std::string unit;
            std::cin >> value;
            // Gestione m per minuti [cite: 80]
            currentTime += value; 
            std::cout << "Nuovo istante: " << currentTime << ". Analisi violazioni..." << std::endl;
            // Qui andrebbe la logica di calcolo velocità media tra varchi consecutivi [cite: 87]
        } 
        else if (command == "stats") {
            std::cout << "Statistiche sistema tutor..." << std::endl; // [cite: 90]
        } 
        else if (command == "reset") {
            currentTime = 0; // [cite: 89]
        }
        else if (command == "Q" || command == "q"){
            break;
        }
        else {
            std::cout << " Commando invalido!";
        }
    }
    return 0;

}
