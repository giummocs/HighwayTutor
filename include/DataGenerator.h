#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

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

class DataGenerator
{
    public:
        void startHighwaySimulation(std::ofstream& runsOut, std::ofstream& passOut);
        DataGenerator(const std::string& highwayFile);
    private:
        std::vector<int> firstGateForJunction;
        Highway hw;
        std::unordered_map<std::string, bool> plates;
        double currentSimulationTime;

        const int HOURS_IN_SECOND = 3600 ;
        const int NUM_VEHICLES = 10000;         
        const double MIN_TIME_GAP = 0.5;        
        const double MAX_TIME_GAP = 10.0;       
        const int MIN_SPEED = 80;
        const int MAX_SPEED = 190;
        const int MIN_DURATION_MIN = 5;
        const int MAX_DURATION_MIN = 15;
        const int NUM_OF_LETTERS= 26;
        const int NUM_OF_DIGITS= 10;     
    
        std::string generatePlate();
        int randomInt(int min, int max);
        double randomDouble(double min, double max);    
        void generateRunsLine(std::ofstream& outFile, const Vehicle& vehicle);
        void generatePassages(std::ofstream& outFile, const Vehicle& vehicle,double kmEntry, double kmExit);
        int findFirstGateForJunctions(int junctionsId);
        void generateProfile(Vehicle& v, double totalDistance);
};

#endif
