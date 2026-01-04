#ifndef DATAPROCESS_H
#define DATAPROCESS_H

#include <limits>

#include "Highway.h"

//La classe DataProcessor ha il compito di leggere i dati di highway.txt e passages.txt, metterli in apposite strutture dati e elaborarli nella maniera corretta
class DataProcessor{
public:
    //Costruttori
    DataProcessor();
    DataProcessor(std::string filenameHighway, std::string filenamePassages);

    //Funzioni pubbliche
    std::string set_time(const std::string& s);
    //Restituisce in un unica stringa tutti i dati richiesti (tutte le violazioni), tra gli istanti currentTime e currentTime+parametro
    //Lancia un eccezione se il tempo inserito è minore o uguale a 0

    std::string stats();
    //Restituisce in un unica stringa tutti i dati richiesti (statistiche di ogni varco, velocità media totale e totale violazioni)

    std::string reset();
    //Resetta il conteggio del tempo (currentTime) e restituisce l'esito

private:
    //Costanti
    const int SECONDS_IN_HOURS = 3600;
    const int SECONDS_IN_MINUTES = 60;

    //Variabili private
    double currentTime; //Conteggio del tempo
    double totalAverageVelocity; //Velocità media di tutti i veicoli, utile a stats
    Highway hw; //Oggetto autostrada, si occupa della lettura di Highway.txt

    struct PassageByPlate {int id; double time;};
    struct Violation {int gateStartId; int gateEndId; double averageVelocity; double gateStartTime; double gateEndTime;};
    struct Statistic {int vehiclesNumber = 0; double minTime = std::numeric_limits<double>::infinity(); double maxTime = 0.0;};
    
    //Uso le mappe come indici perche' mi consente di semplificare gli algoritmi di ricerca delle informazioni, riducendone la complessita grazie all'accesso tramite chiave 
    std::unordered_map<std::string, std::vector<PassageByPlate>> passages; //Contiene tutti i dati letti da passages.txt, organizzati per targa tramite la chiave
    std::unordered_map<std::string, std::vector<Violation>> violations; //Contiene tutte le violazioni, organizzate per targa
    std::unordered_map<int, Statistic> statistics; //Contiene le statistiche di ciascun varco, organizzate per varco

    //Helper functions
    //Processa tutti i dati contenuti in passages, riempiendo violations e statistics
    void processData();
    //Lancia un eccezione se i file sono scritti nella maniera errata

    int decodeInput(const std::string& s);
    static bool compareId(const PassageByPlate& p1, const PassageByPlate& p2);
    void updateStat(int id, double time);
    void loadFromFile(const std::string& filename);

};


#endif






