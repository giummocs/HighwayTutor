#include "Highway.h"
#include "Vehicle.h"
#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>

int main() {
    Highway hwy;
    if (!hwy.loadFromFile("Data/Highway.txt")) {
        std::cerr << "Errore caricamento autostrada" << std::endl;
        return 1;
    }

    std::ofstream runs("Data/Runs.txt");
    std::ofstream passages("Data/Passages.txt");
    
    std::mt19937 gen(42);
    std::uniform_int_distribution<> disV(80, 190);
    std::uniform_int_distribution<> disT(300, 900); // 5-15 min in sec [cite: 41]
    
    double currentTime = 0;

    for (int i = 0; i < 10000; ++i) { // [cite: 60]
        // Logica semplificata per generazione targa e percorso
        std::string plate = "AA" + std::to_string(100 + (i % 899)) + "ZZ";
        double entryKm = hwy.points[0].km;
        double exitKm = hwy.points.back().km;
        
        runs << plate << " " << 1 << " " << (hwy.points.size()/2) << " " << currentTime << " ";
        
        double pos = entryKm;
        double timeAtPos = currentTime;

        // Generazione passaggi varchi [cite: 65]
        for (const auto& p : hwy.points) {
            if (p.type == 'V' && p.km > entryKm && p.km < exitKm) {
                // Calcolo semplificato tempo al varco
                double timeAtVarco = timeAtPos + (p.km - pos) / (130.0 / 3600.0);
                passages << p.id << " " << plate << " " << std::fixed << std::setprecision(2) << timeAtVarco << "\n";
            }
        }
        runs << "\n";
        currentTime += 5.0; // Intervallo tra veicoli [cite: 59]
    }
    return 0;
}