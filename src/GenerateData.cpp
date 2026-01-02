



GenerateData::GenerateData(std::string filenameHighway, std::string filenamePassages) : hw(filenameHighway){
    currentTime = 0;
    readFromFile(filenamePassages);
    gates = hw.getGates();
}
int GenerateData::findFirstGate(const std::vector<double>& gates, double kmEntry)
{
    int firstGateIdx = 0;
    while (gateIdx < gates.size() && gates[gateIdx] < kmEntry) 
    {
        firstGateIdx++;
    }
}
double GenerateData::randomDouble(double min, double max) 
{
    double f = static_cast<double>(std::rand()) / RAND_MAX;
    return min + f * (max - min);
}

int GenerateData::randomInt(int min, int max) 
{
    return min + std::rand() % (max - min + 1);
}
void GenerateData::generatePassages(std::ofstream& outFile, const Vehicle& vehicle, const std::vector<double>& gates, double kmEntry, double kmExit)
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
void GenerateData::generateRunsLine(std::ofstream& outFile, const Vehicle& vehicle)
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
std::string GenerateData::generatePlate() 
{
    std::string plate = "";
    plate += static_cast<char>('A' + std::rand() % 26);
    plate += static_cast<char>('A' + std::rand() % 26);
    plate += " ";
    plate += std::to_string(std::rand() % 10);
    plate += std::to_string(std::rand() % 10);
    plate += std::to_string(std::rand() % 10);
    plate += " ";
    plate += static_cast<char>('A' + std::rand() % 26);
    plate += static_cast<char>('A' + std::rand() % 26);
    return plate;
}

