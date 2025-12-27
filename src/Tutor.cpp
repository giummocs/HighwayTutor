#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

struct Passage {
    std::string plate;
    double time;
};

int main() {
    double currentTime = 0;
    std::string command;
    std::map<int, std::vector<Passage>> data; // VarcoID -> Passaggi

    // Caricamento semplificato dei dati per dimostrazione
    std::ifstream f("Data/Passages.txt");
    int vId; std::string plate; double t;
    while(f >> vId >> plate >> t) {
        data[vId].push_back({plate, t});
    }

    while (std::cin >> command) {
        if (command == "set_time") {
            double value;
            std::string unit;
            std::cin >> value;
            // Gestione m per minuti [cite: 80]
            currentTime += value; 
            std::cout << "Nuovo istante: " << currentTime << ". Analisi violazioni..." << std::endl;
            // Qui andrebbe la logica di calcolo velocità media tra varchi consecutivi [cite: 87]
        } else if (command == "stats") {
            std::cout << "Statistiche sistema tutor..." << std::endl; // [cite: 90]
        } else if (command == "reset") {
            currentTime = 0; // [cite: 89]
        }
    }
    return 0;
}