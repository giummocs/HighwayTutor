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

using namespace std;

const int NUM_VEHICLES = 10000;         
const double MIN_TIME_GAP = 0.5;       
const double MAX_TIME_GAP = 10.0;       
const int MIN_SPEED = 80;
const int MAX_SPEED = 190;
const int MIN_DURATION_MIN = 5;
const int MAX_DURATION_MIN = 15;


double randomDouble(double min, double max) {
    double f = (double)rand() / RAND_MAX;
    return min + f * (max - min);
}

int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

string generatePlate() {
    string plate = "";
    plate += (char)('A' + rand() % 26);
    plate += (char)('A' + rand() % 26);
    plate += " ";
    plate += to_string(rand() % 10);
    plate += to_string(rand() % 10);
    plate += to_string(rand() % 10);
    plate += " ";
    plate += (char)('A' + rand() % 26);
    plate += (char)('A' + rand() % 26);
    return plate;
}

int main() {
    srand((unsigned int)time(NULL));

    cout << "--- Starting Highway Simulator ---" << endl;

    Highway highway;
    string highwayFile = "Data/Highway.txt";
    
    if (!highway.loadFromFile(highwayFile)) {
        cerr << "Error: Impossible to load " << highwayFile << endl;
        return 1;
    }

    const vector<Point>& points = highway.getPoints();
    vector<Point> svincoli;

    for (size_t i = 0; i < points.size(); i++) {
        if (points[i].type == 'S') {
            svincoli.push_back(points[i]);
        }
    }

    if (svincoli.size() < 2) {
        cerr << "Error: Not enough svincoli to simulate a path." << endl;
        return 1;
    }

    string runsFile = "Data/Runs.txt";
    ofstream outFile(runsFile);
    if (!outFile.is_open()) {
        cerr << "Error: Impossible to create " << runsFile << endl;
        return 1;
    }

    double currentSimulationTime = 0.0;

    cout << "Generating " << NUM_VEHICLES << " vehicles..." << endl;

    for (int i = 0; i < NUM_VE_HICLES; i++) {
        Vehicle vehicle;

        vehicle.plate = generatePlate();
        
        int entryIdx = randomInt(0, svincoli.size() - 2);
        int exitIdx = randomInt(entryIdx + 1, svincoli.size() - 1);

        Point svincoloIngresso = svincoli[entryIdx];
        Point svincoloUscita = svincoli[exitIdx];

        vehicle.startSvincolo = svincoloIngresso.id;
        vehicle.endSvincolo = svincoloUscita.id;
        
        currentSimulationTime += randomDouble(MIN_TIME_GAP, MAX_TIME_GAP);
        vehicle.startTime = currentSimulationTime;

        double totalDistanceToCover = svincoloUscita.km - svincoloIngresso.km;
        double coveredDistance = 0.0;

        while (coveredDistance < totalDistanceToCover) {
            SpeedInterval interval;
            
            interval.speed = (double)randomInt(MIN_SPEED, MAX_SPEED);
            
            double minutes = (double)randomInt(MIN_DURATION_MIN, MAX_DURATION_MIN);
            interval.duration = minutes * 60.0; 
            
            double hours = minutes / 60.0;
            double intervalDistance = interval.speed * hours;

            coveredDistance += intervalDistance;
            vehicle.profile.push_back(interval);
        }

        
        outFile << vehicle.plate << " "
                << vehicle.startSvincolo << " "
                << vehicle.endSvincolo << " "
                << fixed << setprecision(2) << vehicle.startTime;

        for (size_t k = 0; k < vehicle.profile.size(); k++) {
            outFile << " " << vehicle.profile[k].speed 
                    << " " << vehicle.profile[k].duration;
        }
        
        outFile << endl;
    }

    outFile.close();
    cout << "Simulation completed. Data saved in " << runsFile << endl;

    return 0;
}
