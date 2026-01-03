#include "DataGenerator.h"


DataGenerator::DataGenerator(const std::string& filenameHighway) : hw(filenameHighway){
    currentTimeSimulation = 0;
    int junctionsSize = hw.getSize('S');
    int gatesSize = hw.getSize('V');
    firstGateForJunction.resize(junctionsSize + 1);

    for (int j_id = 1; j_id <= junctionsSize; j_id++) {
        // Highway riceve l'ID (1-based) e restituisce i km
        double junctionsKm = hw.getDistance('S', j_id); 
        int g_id = 1;
        
        while (g_id <= gatesSize && hw.getDistance('V', g_id) < junctionsKm) {
            g_id++;
        }

        if (g_id <= gatesSize) {
            firstGateForJunction[j_id] = g_id; // Salviamo l'ID del varco
        } else {
            firstGateForJunction[j_id] = -1; 
        }
    }
}
int DataGenerator::findFirstGateForJunctions(int junctionId) 
{
    // 1. Otteniamo il chilometro dello svincolo usando il suo ID
    double junctionKm = hw.getDistance('S', junctionId);
    
    // 2. Otteniamo il numero totale di varchi nell'autostrada
    int gatesSize = hw.getSize('V');
    
    // 3. Partiamo dal primo varco (ID 1)
    int g_id = 1; 

    // 4. Ciclo che scorre i varchi finché non ne trova uno 
    // che si trova dopo il km dello svincolo d'ingresso
    while (g_id <= gatesSize && hw.getDistance('V', g_id) < junctionKm) 
    {
        g_id++;
    }

    // 5. Se g_id è ancora nei limiti, abbiamo trovato il varco. 
    // Altrimenti significa che non ci sono varchi dopo questo svincolo.
    if (g_id <= gatesSize) 
    {
        return g_id; // Restituisce l'id del primo varco utile
    } 
    else 
    {
        return -1;   // Non esiste alcun varco successivo (fine autostrada)
    }
}
double DataGenerator::randomDouble(double min, double max) 
{
    double f = static_cast<double>(std::rand()) / RAND_MAX;
    return min + f * (max - min);
}

int DataGenerator::randomInt(int min, int max) 
{
    return min + std::rand() % (max - min + 1);
}
void DataGenerator::generatePassages(std::ofstream& outFile, const Vehicle& vehicle, double kmEntry, double kmExit) 
{
    // Otteniamo l'ID del primo varco utile usando la funzione di ricerca
    int g_id = findFirstGateForJunctions(vehicle.startSvincolo);
    
    // Se non ci sono varchi tra l'ingresso e l'uscita, usciamo subito
    if (g_id == -1) return;

    int gatesSize = hw.getSize('V');
    double currentKm = kmEntry;
    double currentTime = vehicle.startTime;

    // Scorriamo ogni intervallo di velocità del profilo del veicolo
    for (int i = 0; i < vehicle.profile.size(); i++) {
        double speedKmS = vehicle.profile[i].speed / 3600.0; 
        double duration = vehicle.profile[i].duration;   
        
        // Calcoliamo dove si troverà il veicolo alla fine di questo intervallo
        double nextKm = currentKm + (speedKmS * duration);

        // Finché l'ID del varco è valido e il varco si trova entro il tratto percorso
        while (g_id <= gatesSize && hw.getDistance('V', g_id) <= nextKm) 
        {    
            double gateKm = hw.getDistance('V', g_id);
            
            // Se il varco si trova oltre il punto in cui il veicolo esce dall'autostrada, fermati
            if (gateKm >= kmExit) break;

            // Calcoliamo l'istante esatto del passaggio (tempo = distanza / velocità)
            int gateInstant = static_cast<int>(currentTime + ((gateKm - currentKm) / speedKmS));
            
            // Scrittura sul file: ID varco (già 1-based), targa e tempo
            outFile << g_id << " " << vehicle.plate << " " << gateInstant << std::endl;
            
            // Passiamo al varco successivo (ID incrementato)
            g_id++;
        }
        
        // Aggiorniamo posizione e tempo per il prossimo intervallo del profilo
        currentKm = nextKm;
        currentTime += duration;
    }
}
void DataGenerator::generateRunsLine(std::ofstream& outFile, const Vehicle& vehicle)
{
    // Scriviamo i dati base
    outFile << vehicle.plate << " "
            << vehicle.startSvincolo << " "
            << vehicle.endSvincolo << " "
            << std::fixed << std::setprecision(2) << vehicle.startTime;

    // Scriviamo tutto il profilo di velocità (v1, t1, v2, t2...)
    for (std::size_t k = 0; k < vehicle.profile.size(); k++) {
        outFile << " v" << k + 1 << " " << vehicle.profile[k].speed 
                << " t" << k + 1 << vehicle.profile[k].duration;
    }
    outFile << std::endl;
}
std::string DataGenerator::generatePlate() 
{
    std::string plate = "";
    plate += static_cast<char>('A' + std::rand() % NUM_OF_LETTERS);
    plate += static_cast<char>('A' + std::rand() % NUM_OF_LETTERS);
    plate += " ";
    plate += std::to_string(std::rand() %  NUM_OF_DIGITS);
    plate += std::to_string(std::rand() %  NUM_OF_DIGITS);
    plate += std::to_string(std::rand() %  NUM_OF_DIGITS);
    plate += " ";
    plate += static_cast<char>('A' + std::rand() %  NUM_OF_LETTERS);
    plate += static_cast<char>('A' + std::rand() % NUM_OF_LETTERS);
    return plate;
}
void DataGenerator::generateProfile(Vehicle& v, double totalDistance) {
    double coveredDistance = 0.0;

    while (coveredDistance < totalDistance) {
        SpeedInterval interval;
        
        // Genera velocità casuale tra MIN_SPEED e MAX_SPEED
        interval.speed = static_cast<double>(randomInt(MIN_SPEED, MAX_SPEED));
        
        // Genera durata casuale in minuti e la converte in secondi
        double minutes = static_cast<double>(randomInt(MIN_DURATION_MIN, MAX_DURATION_MIN));
        interval.duration = minutes * 60.0; 

        // Calcola la distanza percorsa in questo intervallo (v * t)
        // Usiamo (minutes / 60.0) perché la velocità è in km/h
        coveredDistance += interval.speed * (minutes / 60.0);
        
        // Aggiunge l'intervallo al profilo del veicolo
        v.profile.push_back(interval);
    }
}
void DataGenerator::startHighwaySimulation(std::ofstream& runsOut, std::ofstream& passOutS)
{
     for (int i = 0; i < NUM_VEHICLES; i++) 
    {
        Vehicle vehicle;

        std::string plate = "";

        do{
            plate = generatePlate();
        }while(plates[plate]);

        plates[plate] = true;
        
        vehicle.plate = plate;
        char J = 'S';
        int entryIdx = randomInt(1, static_cast<int>(hw.getSize(J)) - 2);
        int exitIdx = randomInt(entryIdx + 1, static_cast<int>(hw.getSize(J)) - 1);

        double kmEntry = hw.getDistance('S', entryIdx);
        double kmExit = hw.getDistance('S', exitIdx);
        
        currentSimulationTime += randomDouble(MIN_TIME_GAP, MAX_TIME_GAP);
        vehicle.startTime = currentSimulationTime;

        double totalDistanceToCover = kmExit - kmEntry;
        generateProfile(vehicle, totalDistanceToCover);
        generateRunsLine(runsOut, vehicle);
        generatePassages(passOut, vehicle, kmEntry, kmExit);
        
    }
}
