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
#include "GenerateData.h"

const int NUM_VEHICLES = 10000;

int main() 
{
    std::srand(static_cast<unsigned int>(std::time(NULL))); // serve per i numeri random

    std::cout << "--- Starting Highway Simulator ---" << std::endl;

    std::string highwayFile = "Data/Highway.txt";
    Highway hw(highwayFile);
    GenerateData simulator(highwayFile);
    //creo i vettori

    std::string runsFile = "Data/Runs.txt";

    double currentSimulationTime = 0.0;

    std::cout << "Generating " << NUM_VEHICLES << " vehicles..." << std::endl;

    std::ofstream runsOut("Data/Runs.txt");
    std::ofstream passOut("Data/Passages.txt");
    simulator.startVehicleSimulation(runsOut, passOut);

    runsOut.close();
    passOut.close();
    std::cout << "Simulation completed. Data saved in " << runsFile << std::endl;

    return 0;
}
















