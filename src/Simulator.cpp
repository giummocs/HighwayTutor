//FILE REALIZZATO DA RIGONI SIMONE

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <memory>

#include "DataGenerator.h"

int main(int argsNumber, char* args[]) 
{    
    const int NUM_VEHICLES = 10000;
    
    std::unique_ptr<DataGenerator> simulator = nullptr; //Puntatore
    std::srand(static_cast<unsigned int>(std::time(NULL))); // Inizializzazione del seme per i numeri casuali

    //Lettura del nome del file highway.txt
    if(argsNumber == 1){
        filename = "../Data/Highway.txt";
    }
    else if (argsNumber != 2) {
        std::cerr << "Usare il comando: " << args[0] << " <nome_file>" << std::endl;
        return 1;
    }
    else{
        std::string filename = "../Data/"+std::string(args[1]);
    }
    
    //Inizializzazione del simulatore
    //In caso di errore nella lettura dei file lancia un eccezione
    try{
        simulator = std::make_unique<DataGenerator>(filename, NUM_VEHICLES);
    }
    catch(const std::runtime_error& e){
        std::cerr << e.what() << std::endl;
        return 1;
    }
    

    std::cout << "\n------INIZIALIZZAZIONE DEL SIMULATORE------" << std::endl;
    std::cout << "Generazione di 10000 veicoli in corso..." << std::endl;

    // Nomi dei file di output
    std::string pathRuns = "../Data/Runs.txt";
    std::string pathPassages = "../Data/Passages.txt";

    std::ofstream runsOutput(pathRuns);
    std::ofstream passOutput(pathPassages);

    // Controllo se i file si sono aperti correttamente
    if (!runsOutput || !passOutput) {
        std::cerr << "ERRORE: Impossibile aprire i file di output!" << std::endl;
        return 1;
    }

    // CHIAMATA ALLA FUNZIONE
    simulator->startHighwaySimulation(runsOutput, passOutput);

    // Chiusura dei file
    runsOutput.close();
    passOutput.close();

    std::cout << "Generazione completata!" << std::endl;
    std::cout << "Dati salvati in: " << pathRuns << " e " << pathPassages << std::endl;

    return 0;
}



















