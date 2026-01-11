//FILE REALIZZATO DA STEFANI GIANMARIA

#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <limits>
#include <iomanip>

#include "Highway.h"

//La classe DataProcessor ha il compito di leggere i dati di highway.txt e passages.txt, metterli in apposite strutture dati e elaborarli nella maniera corretta
class DataProcessor{
public:
    //Costruttore default
    DataProcessor();

    //Costruttore utile a processare tutti i dati
    //Primo parametro: percorso file del file Highway.txt
    //Secondo parametro: percorso file del file Passages.txt
    DataProcessor(std::string filenameHighway, std::string filenamePassages);

    //Restituisce in un unica stringa tutti i dati richiesti (tutte le violazioni), tra gli istanti currentTime e currentTime+parametro
    //Parametro: input inserito dall'utente
    //Lancia un eccezione se il tempo inserito è minore o uguale a 0
    std::string set_time(const std::string& input);

    //Restituisce in un unica stringa tutti i dati richiesti (statistiche di ogni varco, velocità media totale e totale violazioni)
    std::string stats();
    
    //Resetta il sistema e restituisce l'esito
    std::string reset();
    

private:
    //Costanti
    const int SECONDS_IN_HOURS = 3600;
    const int SECONDS_IN_MINUTES = 60;

    double currentTime; //Conteggio del tempo
    int totalViolations; //Totale delle violazioni relative a currentTime
    Highway hw; //Oggetto autostrada, si occupa della lettura di Highway.txt

    struct PassageByPlate {int id; double time;};
    struct Violation {int gateStartId; int gateEndId; double averageSpeed; double gateStartTime; double gateEndTime;};
    struct TripSegment {double endTime; double distance; double duration;};
    
    std::unordered_map<std::string, std::vector<Violation>> violations; //Contiene tutte le violazioni, organizzate per targa
    std::unordered_map<int, std::vector<double>> statistics; //Contiene le statistiche di ciascun varco, organizzate per varco
    std::vector<TripSegment> segments; //Contiene un segmento del tragitto percorso da un veicolo, cioè la distanza e tempo impiegato tra un varco e l'altro, utile per la velocita media totale

    //Processa tutti i dati contenuti in passages, riempiendo violations e statistics
    //Parametro: nome del file passages.txt
    //Lancia un eccezione se i file sono scritti nella maniera errata
    void processData(std::string filenamePassages);

    //Funzione chiamata in set_time() per tradurre l'input ricevuto dall'utente
    //Parametro: input inserito dall'utente
    //Restituisce il tempo in secondi
    int decodeInput(const std::string& input);

    //Funzione usata per il sort di passages, usata in processData()
    static bool comparePassage(const PassageByPlate& p1, const PassageByPlate& p2);

    //Funzione usata per il sort di segments, usata in processData()
    static bool compareSegment(const TripSegment& t1, const TripSegment& t2);

    //Funzione usata per upper_bound di segments, usata in stats()
    static bool compareTime(double value, const TripSegment& t1);

    //Converte da stringa a intero
    //Primo parametro: numero da convertire
    //Secondo parametro: risultato della conversione passato per riferimento
    //Restituisce true se la conversione è andata a buon fine, false altrimenti
    bool stringToInt(const std::string& numberPart, int& result);

    //Converte da stringa a double
    //Primo parametro: numero da convertire
    //Secondo parametro: risultato della conversione passato per riferimento
    //Restituisce true se la conversione è andata a buon fine, false altrimenti
    bool stringToDouble(const std::string& numberPart, double& result);

    //Costruisce l'output delle statistiche di ogni varco
    //Restituisce una stringa con tutti i dati
    std::string buildGateStats();

    //Costruisce l'output delle statistiche generali (velocità media e numero sanzioni)
    //Restituisce una stringa con tutti i dati
    std::string buildStats();

    //Legge i dati del file e li inserisce in passsages
    //Primo parametro: percorso del file
    //Secondo parametro: variabile da riempire con i dati
    void loadFromFile(const std::string& filename, std::unordered_map<std::string, std::vector<PassageByPlate>>& passages);

};


#endif


















