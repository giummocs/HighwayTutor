#include <cstdlib>  
#include <ctime>    
#include <iomanip>  

#include "Vehicle.h"
#include "Highway.h"
#include "DataGenerator.h"

int main() 
{
    std::srand(static_cast<unsigned int>(std::time(NULL))); // serve per i numeri random

    std::cout << "--- Starting Highway Simulator ---" << std::endl;

    std::string highwayFile = "Highway.txt";
    Highway hw(highwayFile);
    GenerateData simulator(highwayFile);

    double currentSimulationTime = 0.0;

    std::cout << "Generating 10000 vehicles..." << std::endl;

    std::ofstream runsOut("Runs.txt");
    std::ofstream passOut("Passages.txt");
    simulator.startVehicleSimulation(runsOut, passOut);

    runsOut.close();
    passOut.close();
    std::cout << "Simulation completed. Data saved in " << runsFile << std::endl;

    return 0;
}



















