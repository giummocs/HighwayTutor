#include "DataGenerator.h"


DataGenerator::DataGenerator(std::string filenameHighway, std::string filenamePassages) : hw(filenameHighway){
    currentTime = 0;
    gates = hw.getGates();
    junctions = hw.getJunctions();
}
int DataGenerator::findFirstGate(const std::vector<double>& gates, double kmEntry)
{
    int firstGateIdx = 0;
    while (gateIdx < gates.size() && gates[gateIdx] < kmEntry) 
    {
        firstGateIdx++;
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
void DataGenerator::generatePassages(std::ofstream& outFile, const Vehicle& vehicle, const std::vector<double>& gates, double kmEntry, double kmExit)
{
    int gateIdx = findFirstGate(gates, kmEntry);
    
    double currentKm = kmEntry;
    double currentTime = vehicle.startTime;

    for (int i = 0; i < vehicle.profile.size(); i++) {
        double speedKmS =  vehicle.profile[i].speed / HOURS_IN_SECOND ; 
        double duration = vehicle.profile[i].duration;   
        
        double nextKm = currentKm + (speedKmS * duration);

        while( gateIdx < gates.size() && gates[gateIdx] <= nextKm && gates[gateIdx] < kmExit )
        {    
            double gateKm = gates[gateIdx];
            int gateInstant = currentTime + ((gateKm - currentKm) / speedKmS);
            
            outFile << gateIdx + 1 << " "
                    << vehicle.plate << " " 
                    <<gateInstant;
            
            outFile << std::endl;
            
            gateIdx++;
        }
        
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
    plate += static_cast<char>('A' + std::rand() % NUMERO_LETTERE);
    plate += static_cast<char>('A' + std::rand() % NUMERO_LETTERE);
    plate += " ";
    plate += std::to_string(std::rand() %  NUMERO_CIFRE);
    plate += std::to_string(std::rand() %  NUMERO_CIFRE);
    plate += std::to_string(std::rand() %  NUMERO_CIFRE);
    plate += " ";
    plate += static_cast<char>('A' + std::rand() %  NUMERO_LETTERE);
    plate += static_cast<char>('A' + std::rand() % NUMERO_LETTERE);
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
void DataGenerator::startHighwaySimulation(std::ofstream& runsOut, std::ofstream& passOut)
{
     for (int i = 0; i < NUM_VEHICLES; i++) 
    {
        Vehicle vehicle;

        std::string plate = "";

        do{
            plate = generatePlate();
        }while(map[plate]);

        map[plate] = true;
        
        vehicle.plate = plate;
        
        int entryIdx = randomInt(0, static_cast<int>(junctions.size()) - 2);
        int exitIdx = randomInt(entryIdx + 1, static_cast<int>(junctions.size()) - 1);

        double kmEntry = hw.getDistance('S', entryIdx);
        double kmExit = hw.getDistance('S', exitIdx);
        
        currentSimulationTime += randomDouble(MIN_TIME_GAP, MAX_TIME_GAP);
        vehicle.startTime = currentSimulationTime;

        double totalDistanceToCover = kmExit - kmEntry;
        generateProfile(vehicle, totalDistanceToCover);
        generateRunsLine(runsOut, vehicle);
        generatePassages(passOut, vehicle, gates, kmEntry, kmExit);
        
    }
}
