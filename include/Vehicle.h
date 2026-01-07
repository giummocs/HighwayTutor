//FILE REALIZZATO DA RIGONI SIMONE
#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <vector>

struct SpeedInterval {
    double speed; 
    double duration; 
};

struct Vehicle {
    std::string plate;
    int startJunction;
    int endJunction;
    double startTime;
    std::vector<SpeedInterval> profile;
};


#endif


