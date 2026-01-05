#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <memory>

#include "DataGenerator.h"

int main() 
{
    // Inizializzazione del seme per i numeri casuali
    std::srand(static_cast<unsigned int>(std::time(NULL))); 

    std::cout << "--- Starting Highway Simulator ---" << std::endl;

    // Nome del file di input dell'autostrada
    std::string highwayFile = "Data/Highway.txt";

    // Inizializzazione del simulatore
    std::unique_ptr<DataGenerator> simulator = nullptr;
    try{
        simulator = std::make_unique<DataGenerator>(highwayFile);
    }
    catch(const std::runtime_error& e){
        std::cerr << e.what() << std::endl;
        return 1;
    }
    

    std::cout << "Generating 10000 vehicles..." << std::endl;

    // Nomi dei file di output
    std::string outRuns = "Runs.txt";
    std::string outPassages = "Passages.txt";

    std::ofstream runsOut(outRuns);
    std::ofstream passOut(outPassages);

    // Controllo se i file si sono aperti correttamente
    if (!runsOut || !passOut) {
        std::cerr << "ERRORE: Impossibile aprire i file di output!" << std::endl;
        return 1;
    }

    // CHIAMATA ALLA FUNZIONE
    simulator.startHighwaySimulation(runsOut, passOut);

    // Chiusura dei file
    runsOut.close();
    passOut.close();

    std::cout << "Simulation completed." << std::endl;
    std::cout << "Data saved in: " << outRuns << " and " << outPassages << std::endl;

    return 0;
}


