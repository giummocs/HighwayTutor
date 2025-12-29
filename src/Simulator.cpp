#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>  
#include <ctime>    
#include <cmath>    
#include <iomanip>  

#include "Vehicle.h"
#include "Highway.h"


const int NUM_VEHICLES = 10000;         
const double MIN_TIME_GAP = 0.5;        
const double MAX_TIME_GAP = 10.0;       
const int MIN_SPEED = 80;
const int MAX_SPEED = 190;
const int MIN_DURATION_MIN = 5;
const int MAX_DURATION_MIN = 15;


double randomDouble(double min, double max) {
    double f = static_cast<double>(std::rand()) / RAND_MAX;
    return min + f * (max - min);
}

int randomInt(int min, int max) {
    return min + std::rand() % (max - min + 1);
}

std::string generatePlate() {
    std::string plate = "";
    plate += static_cast<char>('A' + std::rand() % 26);
    plate += static_cast<char>('A' + std::rand() % 26);
    plate += " ";
    plate += std::to_string(std::rand() % 10);
    plate += std::to_string(std::rand() % 10);
    plate += std::to_string(std::rand() % 10);
    plate += " ";
    plate += static_cast<char>('A' + std::rand() % 26);
    plate += static_cast<char>('A' + std::rand() % 26);
    return plate;
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(NULL))); // serve per i numeri random

    std::cout << "--- Starting Highway Simulator ---" << std::endl;

    Highway highway;
    std::string highwayFile = "Data/Highway.txt";
    
    if (!highway.loadFromFile(highwayFile)) {
        std::cerr << "Error: Impossible to load " << highwayFile << std::endl;
        return 1;
    }
    //creo i vettori
    std::vector<double> svincoli = highway.getSvincoli();
    std::vector<double> varchi = highway.getVarchi();

    if (svincoli.size() < 2) {
        std::cerr << "Error: Not enough svincoli to simulate a path." << std::endl;
        return 1;
    }

    if (varchi.size() < 2) {
        std::cerr << "Error: Not enough varchi to simulate a path." << std::endl;
        return 1;
    }

    std::string runsFile = "Data/Runs.txt";
    std::ofstream outFile(runsFile);
    if (!outFile.is_open()) {
        std::cerr << "Error: Impossible to create " << runsFile << std::endl;
        return 1;
    }

    double currentSimulationTime = 0.0;

    std::cout << "Generating " << NUM_VEHICLES << " vehicles..." << std::endl;
    
    for (int i = 0; i < NUM_VEHICLES; i++) {
        Vehicle vehicle;

        vehicle.plate = generatePlate();
        
        int entryIdx = randomInt(0, static_cast<int>(svincoli.size()) - 2);
        int exitIdx = randomInt(entryIdx + 1, static_cast<int>(svincoli.size()) - 1);

        int svincoloIngresso = svincoli[entryIdx+1];
        int svincoloUscita = svincoli[exitIdx+1];

        double kmEntry = svincoli[entryId];
        double kmExit = svincoli[exitId];
        
        currentSimulationTime += randomDouble(MIN_TIME_GAP, MAX_TIME_GAP);
        vehicle.startTime = currentSimulationTime;

        double totalDistanceToCover = svincoloUscita - svincoloIngresso;
        double coveredDistance = 0.0;

        while (coveredDistance < totalDistanceToCover) {
            SpeedInterval interval;
            
            interval.speed = static_cast<double>(randomInt(MIN_SPEED, MAX_SPEED));
            
            double minutes = static_cast<double>(randomInt(MIN_DURATION_MIN, MAX_DURATION_MIN));
            interval.duration = minutes * 60.0; 
            
            double hours = minutes / 60.0;
            double intervalDistance = interval.speed * hours;

            coveredDistance += intervalDistance;
            vehicle.profile.push_back(interval);
        }

        outFile << vehicle.plate << " "
                << vehicle.startSvincolo << " "
                << vehicle.endSvincolo << " "
                << std::fixed << std::setprecision(2) << vehicle.startTime;
        
        outFile << std::endl;
    }

    outFile.close();
    std::cout << "Simulation completed. Data saved in " << runsFile << std::endl;

    return 0;
}




