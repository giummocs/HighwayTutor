
#include "DataProcess.h";

DataProcess::DataProcess() : Highway(){
    currentTime = 0;
}

DataProcess::DataProcess(std::string filenameHighway, std::string filenamePassages) : Highway(filenameHighway){
    currentTime = 0;
    readFromFile(filenamePassages);
    //CHIAMA ANCHE UNA FUNZIONE NUOVA PER PROCESSARE I DATI, PRENDI LALGORITMO DA SET TIME E TOGLILO
}

std::string DataProcess::set_time(int addTime){
    
    if(addTime <= 0){
        throw std::runtime_error("Error! Invalid file format.");
    }

    double newTime = currentTime + addTime;
    std::string output = "\nInfrazioni commesse tra gli istanti "+currentTime+" e "+newTime+":";

    //Complessita' totale O(n), dove n e' il numero delle righe del file passages.txt
    //Scorro la mappa con chiave plate, cosi da scorrere un veicolo per volta
    for (const auto& mapElement : passagesPlateKey) {

        std::size_t totalGates = mapElement.second.size();

        //Scorro tutti i gate attraversati da una macchina precisa, analizzandone due consecutivi alla volta tramite gateIdx e gateIdx+1
        for(std::size_t gateIdx=0; gateIdx+1 < totalGates; gateIdx++){

            double timeGate1 = mapElement.second[gateIdx].time;
            double timeGate2 = mapElement.second[gateIdx+1].time;
            int idGate1 = mapElement.second[gateIdx].id;
            int idGate2 = mapElement.second[gateIdx+1].id;

            //Controllo che tutti i due gate di riferimento siano dentro il range di tempo richiesto dal parametro addTime
            if(timeGate1 > currentTime && timeGate1 < newTime && timeGate2 > currentTime && timeGate2 > newTime){

                double distanceDifference = std::abs(gates[idGate1] - gates[idGate2]); 
                double timeDifference = std::abs(timeGate1 - timeGate2);
                double averageVelocity = distanceDifference / (timeDifference/SECONDS_IN_HOURS);

                if(averageVelocity > 130){
                    output += "\nInfrazione";
                    output += "\nTarga: "+mapElement.first+"\nTratta: varco "+idGate1+" - varco "+idGate2+"\nVelocità media: "+averageVelocity+"\nIstante di passaggio varco "+idGate1+": "+timeGate1+"\nIstante di passaggio varco "+idGate2+": "+timeGate2+"\n";
                }

            }
        }
    }

    if(output == "\nInfrazioni commesse tra gli istanti "+currentTime+" e "+newTime+":") output = "Nessuna infrazione.";

    currentTime = newTime;

    return output;
}

std::string DataProcess::stats(){
    //Complessita' totale O(2n), dove n numero di righe del file passages.txt
    //Scorro la mappa con chiave id, cosi da scorrere un varco per volta
    for (const auto& mapElement : passagesIdKey) {
        std::size_t totalVehicles = mapElement.second.size();
        double minTime = std::min_element(mapElement.second.begin(), mapElement.second.end(), compareTime)->time;
        double maxTime = std::max_element(mapElement.second.begin(), mapElement.second.end(), compareTime)->time;
        double vehiclePerMinute = (std::abs(minTime - maxTime))
    }
    
    //PER FARE IL SECONDO E TERZO PUNTO DEVI USARE UNA NUOVA STRUTTURA DATI CREATA ALLINIZIO QUANDO CALCOLI LE VELOCITA MEDIE
    //PROBLEMA: il prof dice di dare i dati di OGNI varco, pero in passagesIdKey non é detto che ci siano tutti i varchi, se
    //per assurdo un varco non sia attraversato da nessuna macchina, non sará presente in passagesIdKey, quindi devi trovare un algoritmo
    //che ti permetta di scorrere il vettore gates, di modo da scorrere ogni sincolo varco. A questo punto devi valutare se tenere passagesIdKey oppure
    //usare passagesPlateKey in combinazione con gates (ti verrebbe un algoritmo piu pesante ma ti libera MOLTA memoria ridondante)
    //In qualsiasi caso, devi aggiungere due nuove variabili per contenere i risultati da dare in output, devi creare due nuove struct ciascuna contenente
    //i dati scritti nell'elenco puntato della consegna. A questo punto credo convenga: togliere passagesIdKey, creare nella classe due nuove mappe per contenere i dati di output.

}

std::string DataProcess::reset(){
    currentTime = 0;
    return "\nSistema azzerrato con successo!";
}

//Traduce il tempo ricevuto come parametro da stringa a intero
int DataProcess::decodeInput(const std::string& s){
    bool hasM = false;
    std::string numberPart;
    int result;

    //Controllo ultimo carattere e tolgo m
    if (s.back() == 'm') {
        hasM = true;
        numberPart = s.substr(0, s.size() - 1);
    } else {
        numberPart = s;
    }

    //La parte numerica deve esistere
    if (numberPart.empty()) throw std::runtime_error("Error! Invalid file format.");

    //Conversione
    try {
        result = std::stoi(numberPart);
    } catch (...) {
        throw std::runtime_error("Error! Invalid file format.");
    }
    

    //Se aveva m, moltiplico per 60 per convertire in secondi
    if (hasM) {
        result *= 60;
    }

    return result;
}

bool DataProcess::compareTime(const PassageIdKey& a, const PassageIdKey& b) {
    return a.time < b.time;
}

void DataProcess::readFromFile(const std::string& filename){
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Error! Invalid file format.");

    std::string line;
    while (std::getline(file, line)) {
        
        std::stringstream ss(line);
        std::string singleWord;
        std::vector<std::string> words;

        while (ss >> singleWord) {
            words.push_back(singleWord);
        }

        if (words.size() == 3) {
            try {
                int id = std::stoi(words[0]);
                std::string plate = words[1];
                double time = std::stod(words[3]);
                passagesPlateKey[plate].push_back({id, time});
                passagesIdKey[id].push_back({plate, time});
            } catch (...) {
                throw std::runtime_error("Error! Invalid file format.");
            }
        }
    }
}