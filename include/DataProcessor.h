#ifndef DATAPROCESS_H
#define DATAPROCESS_H

#include <limits>

#include "Highway.h"
//Una classe che ha il compito di prendere insieme i dati di highway.txt e passages.txt e metterli in apposite strutture dati e elaborarli nella maniera corretta
//al fine di servirli al main, deve essere una sottoclasse di highway cosi da poter leggere e usare anche il file highway.txt
class DataProcessor{
public:
    DataProcessor();
    DataProcessor(std::string filenameHighway, std::string filenamePassages);
    std::string set_time(const std::string& s);
    std::string stats();
    std::string reset();

private:
    const int SECONDS_IN_HOURS = 3600;
    const int SECONDS_IN_MINUTES = 60;
    double currentTime;
    double totalAverageVelocity;
    Highway hw;

    struct PassageByPlate {int id; double time;};
    struct Violation {int gateStartId; int gateEndId; double averageVelocity; double gateStartTime; double gateEndTime;};
    struct Statistic {int vehiclesNumber = 0; double minTime = std::numeric_limits<double>::infinity(); double maxTime = 0.0;};
    
    //Uso le mappe come indici perche' mi consente di semplificare gli algoritmi di ricerca delle informazioni, riducendone la complessita grazie all'accesso tramite chiave 
    std::unordered_map<std::string, std::vector<PassageByPlate>> passages;
    std::unordered_map<std::string, std::vector<Violation>> violations;
    std::unordered_map<int, Statistic> statistics;
    
    void processData();
    int decodeInput(const std::string& s);
    static bool compareId(const PassageByPlate& p1, const PassageByPlate& p2);
    void updateStat(int id, double time);
    void loadFromFile(const std::string& filename);

};


#endif

