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

const int NUMERO_CIFRE = 26;
const int NUMERO_LETTERE = 26;
const int HOURS_IN_SECOND = 3600;
const int NUM_VEHICLES = 10000;         
const double MIN_TIME_GAP = 0.5;        
const double MAX_TIME_GAP = 10.0;       
const int MIN_SPEED = 80;
const int MAX_SPEED = 190;
const int MIN_DURATION_MIN = 5;
const int MAX_DURATION_MIN = 15;

int findFirstGate(const std::vector<double>& gates, double kmEntry)
{
    int firstGateIdx = 0;
    while (gateIdx < gates.size() && gates[gateIdx] < kmEntry) 
    {
        firstGateIdx++;
    }
}
double randomDouble(double min, double max) 
{
    double f = static_cast<double>(std::rand()) / RAND_MAX;
    return min + f * (max - min);
}

int randomInt(int min, int max) 
{
    return min + std::rand() % (max - min + 1);
}
void generatePassages(std::ofstream& outFile, const Vehicle& vehicle, const std::vector<double>& gates, double kmEntry, double kmExit)
{
    int gateIdx = findFirstGate(gates, kmEntry);
    
    double currentKm = kmEntry;
    double currentTime = vehicle.startTime;

    for (int i = 0; i < vehicle.profile.size(); i++) {
        double speedKmS =  vehicle.profile[i].speed / HOURS_IN_SECOND ; 
        double duration = vehicle.profile[i].duration;   
        
        double nextKm = currentKm + (speedKmS * duration);

        while( gateIdx < gates.size() && gates[gateIdx] <= nextKm && gates[gateIdx] < kmExit )
        {    
            double gateKm = gates[gateIdx];
            int gateInstant = currentTime + ((gateKm - currentKm) / speedKmS);
            
            outFile << gateIdx + 1 << " "
                    << vehicle.plate << " " 
                    <<gateInstant;
            
            outFile << std::endl;
            
            gateIdx++;
        }
        
        currentKm = nextKm;
        currentTime += duration;
    }
}
void generateRunsLine(std::ofstream& outFile, const Vehicle& vehicle)
{
    // Scriviamo i dati base
    outFile << vehicle.plate << " "
            << vehicle.startSvincolo << " "
            << vehicle.endSvincolo << " "
            << std::fixed << std::setprecision(2) << vehicle.startTime;

    // Scriviamo tutto il profilo di velocità (v1, t1, v2, t2...)
    for (std::size_t k = 0; k < vehicle.profile.size(); k++) {
        outFile << " v" << k + 1 << " " << vehicle.profile[k].speed 
                << " t" << k + 1 << vehicle.profile[k].duration;
    }
    outFile << std::endl;
}
std::string generatePlate() 
{
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

int main() 
{
    std::srand(static_cast<unsigned int>(std::time(NULL))); // serve per i numeri random

    std::cout << "--- Starting Highway Simulator ---" << std::endl;

    std::string highwayFile = "Data/Highway.txt";
    Highway hw(highwayFile);
    
    //creo i vettori
    std::vector<double> junctions = highway.getSvincoli();
    std::vector<double> gates = highway.getVarchi();

    std::string runsFile = "Data/Runs.txt";

    double currentSimulationTime = 0.0;

    std::cout << "Generating " << NUM_VEHICLES << " vehicles..." << std::endl;

    std::ofstream runsOut("Data/Runs.txt");
    std::ofstream passOut("Data/Passages.txt");
    
    for (int i = 0; i < NUM_VEHICLES; i++) 
    {
        Vehicle vehicle;

        vehicle.plate = generatePlate();
        
        int entryIdx = randomInt(0, static_cast<int>(junctions.size()) - 2);
        int exitIdx = randomInt(entryIdx + 1, static_cast<int>(junctions.size()) - 1);

        double kmEntry = junctions[entryIdx];
        double kmExit = junctions[exitIdx];
        
        currentSimulationTime += randomDouble(MIN_TIME_GAP, MAX_TIME_GAP);
        vehicle.startTime = currentSimulationTime;

        double totalDistanceToCover = kmExit - kmEntry;
        double coveredDistance = 0.0;

        while (coveredDistance < totalDistanceToCover) 
        {
            SpeedInterval interval;
            
            interval.speed = static_cast<double>(randomInt(MIN_SPEED, MAX_SPEED));
            
            double minutes = static_cast<double>(randomInt(MIN_DURATION_MIN, MAX_DURATION_MIN));
            interval.duration = minutes * 60.0; 
            
            double hours = minutes / 60.0;
            double intervalDistance = interval.speed * hours;

            coveredDistance += intervalDistance;
            vehicle.profile.push_back(interval);
        }

        generateRunsLine(runsOut, vehicle);
        generatePassages(passOut, vehicle, gates, kmEntry, kmExit);
        
    }

    outFile.close();
    std::cout << "Simulation completed. Data saved in " << runsFile << std::endl;

    return 0;
}











