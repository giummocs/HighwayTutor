//FILE REALIZZATO DA RIGONI SIMONE
#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <cstdlib>  
#include <ctime>       
#include <iomanip>  

#include "Highway.h"
//La classe DataGenerator ha il compito di scrivere su due file di testo, in Runs.txt scrive il profilo del veicolo e le velocità nei vari intervalli
//in Passages.txt scrive il numero del varco, la targa del veicolo passato per quel varco e l'istante di passaggio calcolato con le velocità e gli 
//intervalli di tempo generati in modo randomico
class DataGenerator
{
    public:
        //Funzione che mette insieme tutte le funzioni di DataGenerator 
        void startHighwaySimulation(std::ofstream& runsOut, std::ofstream& passOut);

        //Costruttore che ha come parametri il file dell'autostrada e il numero di veicoli(che possono variare anche se standard sono 10000)
        //Inizializza la simulazione e crea una mappatura tra svincoli (S) e varchi (V), per ogni svincolo, individua il varco successivo.
        DataGenerator(const std::string& highwayFile, int num_vehicles);

    private:
        //Vettore che mi serve nella funzione findFirstGateForJunctions
        std::vector<int> firstGateForJunction;
        //Oggetto di tipo Highway
        Highway hw;
        //Mappa chiave valore con chiave una targa e valore true o false per evitare coppie di targhe 
        std::unordered_map<std::string, bool> plates;
        //Variabile che tiene traccia del tempo
        double currentSimulationTime;

        //Costanti
        const int NUM_VEHICLES;
        const int HOURS_IN_SECONDS = 3600 ;         
        const double MIN_TIME_GAP = 0.5;        
        const double MAX_TIME_GAP = 10.0;       
        const int MIN_SPEED = 80;
        const int MAX_SPEED = 190;
        const int MIN_DURATION_MIN = 5;
        const int MAX_DURATION_MIN = 15;
        const int NUM_OF_LETTERS = 26;
        const int NUM_OF_DIGITS = 10;

        //Struct dove le variabili sono la velocità e l'intervallo di tempo generate randomicamente, 
        //utile per la struct Vehicle dove c'è un vettore di SpeedInterval 
        struct SpeedInterval {double speed; double duration;};

        //Struct dove le variabili, generate randomicamente, sono il profilo del veicolo
        struct Vehicle {std::string plate; int startJunction; int endJunction; double startTime; std::vector<SpeedInterval> profile;};
        
        //Generatore di targhe randomico
        std::string generatePlate();

        //Generatore di int randomico
        int randomInt(int min, int max);

        //Generatore di double randomico
        double randomDouble(double min, double max);  

        //Generatore di una riga di runs che poi gira in un for che scorre tutti i veicoli
        void generateRunsLine(std::ofstream& outFile, const Vehicle& vehicle);

        //Generatore di righe del file passages per veicolo
        void generatePassages(std::ofstream& outFile, const Vehicle& vehicle,double kmEntry, double kmExit);

        //Funzione che mi serve per sapere il primo varco dopo uno svincolo sapendo l'ID 
        int findFirstGateForJunctions(int junctionsId);

        //Creazione del profilo di un veicolo 
        void generateProfile(Vehicle& v, double totalDistance);
};

#endif
