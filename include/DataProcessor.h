//FILE REALIZZATO DA STEFANI GIANMARIA

#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <limits>

#include "Highway.h"

//La classe DataProcessor ha il compito di leggere i dati di highway.txt e passages.txt, metterli in apposite strutture dati e elaborarli nella maniera corretta
class DataProcessor{
public:
    //Costruttori
    DataProcessor();
    DataProcessor(std::string filenameHighway, std::string filenamePassages);

    //Restituisce in un unica stringa tutti i dati richiesti (tutte le violazioni), tra gli istanti currentTime e currentTime+parametro
    //Lancia un eccezione se il tempo inserito è minore o uguale a 0
    std::string set_time(const std::string& input);

    //Restituisce in un unica stringa tutti i dati richiesti (statistiche di ogni varco, velocità media totale e totale violazioni)
    std::string stats();
    
    //Resetta il conteggio del tempo (currentTime) e restituisce l'esito
    std::string reset();
    

private:
    //Costanti
    const int SECONDS_IN_HOURS = 3600;
    const int SECONDS_IN_MINUTES = 60;

    double currentTime; //Conteggio del tempo
    double totalAverageVelocity; //Velocità media di tutti i veicoli, utile a stats
    Highway hw; //Oggetto autostrada, si occupa della lettura di Highway.txt

    struct PassageByPlate {int id; double time;};
    struct Violation {int gateStartId; int gateEndId; double averageVelocity; double gateStartTime; double gateEndTime;};
    struct Statistic {int vehiclesNumber = 0; double minTime = std::numeric_limits<double>::infinity(); double maxTime = 0.0;};
    
    std::unordered_map<std::string, std::vector<PassageByPlate>> passages; //Contiene tutti i dati letti da passages.txt, organizzati per targa tramite la chiave
    std::unordered_map<std::string, std::vector<Violation>> violations; //Contiene tutte le violazioni, organizzate per targa
    std::unordered_map<int, Statistic> statistics; //Contiene le statistiche di ciascun varco, organizzate per varco

    //Processa tutti i dati contenuti in passages, riempiendo violations e statistics
    //Lancia un eccezione se i file sono scritti nella maniera errata
    void processData();

    //Funzione chiamata in set_time() per tradurre l'input ricevuto dall'utente
    //Restituisce il tempo in secondi
    int decodeInput(const std::string& input);

    //Funzione usata per il sort di passages, usata in processData()
    static bool compareId(const PassageByPlate& p1, const PassageByPlate& p2);

    //Aggiunge una nuova statistica se l'id non è presente, altrimenti aggiorna i dati già esistenti
    void updateStat(int id, double time);

    //Legge i dati del file e li inserisce in passsages
    void loadFromFile(const std::string& filename);

};


#endif










