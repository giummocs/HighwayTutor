#define GENERATEDATA_H

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

class GenerateData : public Highway{
public:
    

private:
    const int HOURS_IN_SECOND = 3600 ;
    const int NUM_VEHICLES = 10000;         
    const double MIN_TIME_GAP = 0.5;        
    const double MAX_TIME_GAP = 10.0;       
    const int MIN_SPEED = 80;
    const int MAX_SPEED = 190;
    const int MIN_DURATION_MIN = 5;
    const int MAX_DURATION_MIN = 15;


};

