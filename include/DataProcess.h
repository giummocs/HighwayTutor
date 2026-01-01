#ifndef DATAPROCESS_H
#define DATAPROCESS_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <cmath>

#include "Highway.h";
//Una classe che ha il compito di prendere insieme i dati di highway.txt e passages.txt e metterli in apposite strutture dati e elaborarli nella maniera corretta
//al fine di servirli al main, deve essere una sottoclasse di highway cosi da poter leggere e usare anche il file highway.txt
class DataProcess : public Highway{
public:
    DataProcess();
    DataProcess(std::string filenameHighway, std::string filenamePassages);
    std::string set_time(int addTime);
    std::string stats();
    std::string reset();

private:
    struct Passage {int id; std::string plate; double time;};
    const int SECONDS_IN_HOURS = 3600;
    double currentTime;
    //Uso le mappe come indici perche' mi consente di semplificare gli algoritmi di ricerca delle informazioni, riducendone la complessita grazie all'accesso tramite chiave 
    std::unordered_map<std::string, std::vector<size_t>> indexByPlate;
    std::unordered_map<int, std::vector<size_t>> indexById;
    std::vector<Passage> passages;

    int decodeInput(const std::string& s);
    bool compareTime(const PassageIdKey& a, const PassageIdKey& b);
    void readFromFile(const std::string& filename);

};

#endif