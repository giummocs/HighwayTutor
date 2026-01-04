
#include "DataProcessor.h"

DataProcessor::DataProcessor() : hw(){
    currentTime = 0;
}


DataProcessor::DataProcessor(std::string filenameHighway, std::string filenamePassages) : hw(filenameHighway){
    currentTime = 0;
    loadFromFile(filenamePassages);
    processData();
}


void DataProcessor::processData(){
    double totalDistance = 0.0;
    double totalTime = 0.0;

    //Complessita' totale O(n), dove n e' il numero delle righe del file passages.txt
    //Scorro la mappa con chiave plate, cosi da scorrere un veicolo per volta
    for (const auto& mapElement : passages) {
        //Ordino i passaggi per id, cosi da riempire le mappe con gli id gia ordinati
        std::sort(mapElement.second.begin(), mapElement.second.end(), compareId);

        std::size_t gatesNumber = mapElement.second.size();

        //Salvo gia il primo varco
        updateStat(mapElement.second[0].id, mapElement.second[0].time);

        //Scorro tutti i gate attraversati da una macchina precisa, analizzandone due consecutivi alla volta, tramite i e i+1
        for(std::size_t i=0; i+1 < gatesNumber; i++){
            int idGate1 = mapElement.second[i].id;
            int idGate2 = mapElement.second[i+1].id;
            double timeGate1 = mapElement.second[i].time;
            double timeGate2 = mapElement.second[i+1].time;

            //Se non sono consecutivi lancia un'eccezione
            if(idGate1+1 != idGate2){
                throw std::runtime_error("Errore! File non valido: il veicolo non passa in due varchi consecutivi!");
            }

            //Se il tempo non e' crescente lancia un'eccezione
            if(timeGate1 >= timeGate2){
                throw std::runtime_error("Errore! File non valido: errato tempo di passaggio di due varchi!");
            }

            //Salvo i dati per set_time
            //Non controllo il caso di risultati negativi in quanto e' gia presente qui sopra
            double distanceDifference = hw.getDistanceBetween('V', idGate1, idGate2);
            double timeDifference = timeGate1 - timeGate2;
            double averageVelocity = distanceDifference / (timeDifference/SECONDS_IN_HOURS);

            if(averageVelocity > 130){
                violations[mapElement.first].push_back({idGate1, idGate2, averageVelocity, timeGate1, timeGate2});
            }

            //Salvo i dati per stats
            updateStat(idGate2, timeGate2);

            //Aggiungo la distanza e il tempo percorso dalla macchina al totale (utile al comando stats)
            totalDistance += distanceDifference;
            totalTime += timeDifference;
        }
    }
    totalAverageVelocity = totalDistance / (totalTime/SECONDS_IN_HOURS);
}


std::string DataProcessor::set_time(const std::string& s){
    
    int addTime = decodeInput(s);

    if(addTime <= 0){
        throw std::runtime_error("Errore! Tempo inserito non valido!");
    }

    double newTime = currentTime + addTime;
    std::string output = "\nInfrazioni commesse tra gli istanti "+std::to_string(currentTime)+" e "+std::to_string(newTime)+":";

    
    for (const auto& mapElement : violations) {

        std::size_t gatesNumber = mapElement.second.size();

        //Scorro tutti i gate attraversati da una macchina precisa, analizzandone due consecutivi alla volta tramite i e i+1
        for(std::size_t i=0; i+1 < gatesNumber; i++){

            int gateStartId = mapElement.second[i].gateStartId;
            int gateEndId = mapElement.second[i].gateEndId;
            double gateStartTime = mapElement.second[i].gateStartTime;
            double gateEndTime = mapElement.second[i].gateEndTime;
            double averageVelocity = mapElement.second[i].averageVelocity;

            //Controllo che tutti i due gate di riferimento siano dentro il range di tempo richiesto dal parametro addTime
            if(gateStartTime > currentTime && gateEndTime < newTime){
                output += "\nInfrazione";
                output += "\nTarga: "+mapElement.first+"\nTratta: varco "+std::to_string(gateStartId)+" - varco "+std::to_string(gateEndId)+"\nVelocità media: "+std::to_string(averageVelocity)+"\nIstante di passaggio varco "+std::to_string(gateStartId)+": "+std::to_string(gateStartTime)+"\nIstante di passaggio varco "+std::to_string(gateEndId)+": "+std::to_string(gateEndTime)+"\n";
            }
        }
    }

    if(violations.size() == 0) output += "\nNessuna infrazione.";

    currentTime = newTime;

    return output;
}

std::string DataProcessor::stats(){
    std::string output = "\nStatistiche autostrada:";
    int totalGates = hw.getSize('V');

    for (std::size_t i=0; i < totalGates; i++) {
        int vehiclesNumber = statistics[i].vehiclesNumber;
        double minTime = statistics[i].minTime;
        double maxTime = statistics[i].maxTime;
        double vehiclePerMinute = 0.0;

        //Se sono transitati piu di un veicolo calcolo vehiclePerMinute, altrimenti non e' possibile calcolare un valore valido e lascio 0
        if(vehiclesNumber > 1){
            vehiclePerMinute = vehiclesNumber / ((maxTime-minTime)/SECONDS_IN_MINUTES);
        }

        output += "\nVarco "+std::to_string(i)+": "+std::to_string(vehiclesNumber)+" veicoli transitati, "+std::to_string(vehiclePerMinute)+" veicoli al minuto.";
    }

    output += "\nVelocità media totale: "+std::to_string(totalAverageVelocity)+".";
    output += "\nNumero di veicoli sanzionati: "+std::to_string(violations.size())+".";
    
    return output;
}

std::string DataProcessor::reset(){
    currentTime = 0;
    return "\nSistema azzerrato con successo!";
}

//Traduce il tempo ricevuto come parametro da stringa a intero
int DataProcessor::decodeInput(const std::string& s){
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
    if (numberPart.empty()) throw std::runtime_error("Errore! File non valido: ");

    //Conversione
    try {
        result = std::stoi(numberPart);
    } catch (...) {
        throw std::runtime_error("Errore! File non valido: ");
    }
    

    //Se aveva m, moltiplico per 60 per convertire in secondi
    if (hasM) {
        result *= 60;
    }

    return result;
}


static bool compareId(const PassageByPlate& p1, const PassageByPlate& p2) {
    return p1.id < p2.id;
}


void DataProcessor::updateStat(int id, double time){
    Statistic& stat = statistics[id];
    stat.vehiclesNumber++;
    stat.minTime = std::min(stat.minTime, time); //Salvo istante minimo
    stat.maxTime = std::max(stat.maxTime, time); //Salvo istante massimo
}


void DataProcessor::loadFromFile(const std::string& filename){
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Errore! Impossibile aprire il file!");

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

                passages[plate].push_back({id, time});
            } catch (...) {
                throw std::runtime_error("Errore! File non valido: formato numeri dei valori errato!");
            }
        }
        else{
            throw std::runtime_error("Errore! File non valido: numero di valori errato!");
        }
    }
    file.close();

}



