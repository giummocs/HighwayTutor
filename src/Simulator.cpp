#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>  // Per rand() e srand()
#include <ctime>    // Per time()
#include <cmath>    // Per calcoli matematici
#include <iomanip>  // Per formattazione output

#include "Vehicle.h"
#include "Highway.h"

using namespace std;

// --- Costanti di Configurazione ---
const int NUM_VEICOLI = 10000;          // Numero totale di veicoli da generare
const double MIN_GAP_TEMPORALE = 0.5;   // Secondi minimi tra due veicoli
const double MAX_GAP_TEMPORALE = 10.0;  // Secondi massimi tra due veicoli

// Range Velocità (km/h)
const int MIN_VELOCITA = 80;
const int MAX_VELOCITA = 190;

// Range Durata intervallo velocità (minuti)
const int MIN_DURATA_MIN = 5;
const int MAX_DURATA_MIN = 15;

// --- Funzioni di Utilità ---

// Genera un numero double casuale tra min e max
double randomDouble(double min, double max) {
    double f = (double)rand() / RAND_MAX;
    return min + f * (max - min);
}

// Genera un numero intero casuale tra min e max
int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Genera una targa casuale nel formato AA 000 AA
string generaTarga() {
    string targa = "";
    // Prime due lettere
    targa += (char)('A' + rand() % 26);
    targa += (char)('A' + rand() % 26);
    targa += " ";
    // Tre cifre
    targa += to_string(rand() % 10);
    targa += to_string(rand() % 10);
    targa += to_string(rand() % 10);
    targa += " ";
    // Ultime due lettere
    targa += (char)('A' + rand() % 26);
    targa += (char)('A' + rand() % 26);
    return targa;
}

int main() {
    
    srand((unsigned int)time(NULL));

    cout << "--- Avvio Simulatore Autostradale ---" << endl;

  
    Highway autostrada;
    string highwayFile = "Data/Highway.txt";
    
    
    if (!autostrada.loadFromFile(highwayFile)) {
        cerr << "Errore: Impossibile caricare il file " << highwayFile << endl;
        return 1;
    }


    const vector<Point>& punti = autostrada.getPoints();
    vector<Point> svincoli;

    for (size_t i = 0; i < punti.size(); i++) {
        if (punti[i].type == 'S') {
            svincoli.push_back(punti[i]);
        }
    }

    if (svincoli.size() < 2) {
        cerr << "Errore: Non ci sono abbastanza svincoli nell'autostrada per simulare un percorso." << endl;
        return 1;
    }

   
    string runsFile = "Data/Runs.txt";
    ofstream outFile(runsFile);
    if (!outFile.is_open()) {
        cerr << "Errore: Impossibile creare il file " << runsFile << endl;
        return 1;
    }

   
    double tempoCorrenteSimulazione = 0.0;

    cout << "Generazione di " << NUM_VEICOLI << " veicoli in corso..." << endl;

    for (int i = 0; i < NUM_VEICOLI; i++) {
        Vehicle veicolo;

        
        veicolo.plate = generaTarga();
        int idxIngresso = randomInt(0, svincoli.size() - 2);
        
       
        int idxUscita = randomInt(idxIngresso + 1, svincoli.size() - 1);

        Point svincoloIngresso = svincoli[idxIngresso];
        Point svincoloUscita = svincoli[idxUscita];

        veicolo.startSvincolo = svincoloIngresso.id;
        veicolo.endSvincolo = svincoloUscita.id;
        tempoCorrenteSimulazione += randomDouble(MIN_GAP_TEMPORALE, MAX_GAP_TEMPORALE);
        veicolo.startTime = tempoCorrenteSimulazione;

        double distanzaTotaleDaPercorrere = svincoloUscita.km - svincoloIngresso.km;
        double distanzaCoperta = 0.0;

        while (distanzaCoperta < distanzaTotaleDaPercorrere) {
            SpeedInterval intervallo;
            
            intervallo.speed = (double)randomInt(MIN_VELOCITA, MAX_VELOCITA);
            
            double minuti = (double)randomInt(MIN_DURATA_MIN, MAX_DURATA_MIN);
            intervallo.duration = minuti * 60.0; 
            double ore = minuti / 60.0;
            double distanzaIntervallo = intervallo.speed * ore;

            distanzaCoperta += distanzaIntervallo;
            veicolo.profile.push_back(intervallo);
        }

        outFile << veicolo.plate << " "
                << veicolo.startSvincolo << " "
                << veicolo.endSvincolo << " "
                << fixed << setprecision(2) << veicolo.startTime;

        for (size_t k = 0; k < veicolo.profile.size(); k++) {
            outFile << " " << veicolo.profile[k].speed 
                    << " " << veicolo.profile[k].duration;
        }
        
        outFile << endl;
    }

    outFile.close();
    cout << "Simulazione completata con successo. Dati salvati in " << runsFile << endl;

    return 0;
}

