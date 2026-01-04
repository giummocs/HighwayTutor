//FILE REALIZZATO DA STEFANI GIANMARIA

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
    //Scorre la mappa con chiave plate, cosi da scorrere un veicolo per volta
    //Ogni elemento della mappa, cioè ogni veicolo, corrisponde a un vettore che contiene tutti i varchi che il veicolo ha attraversato
    for (const auto& mapElement : passages) {
        
        //Ordina i passaggi per id, cosi da riempire le mappe con gli id gia ordinati
        std::sort(mapElement.second.begin(), mapElement.second.end(), compareId);

        //Numero di varchi che il veicolo ha attraversato
        std::size_t gatesNumber = mapElement.second.size();

        //Salvo gia il primo varco
        updateStat(mapElement.second[0].id, mapElement.second[0].time);

        //Scorre tutti i varchi attraversati da un veicolo preciso, analizzandone due consecutivi alla volta, tramite i e i+1
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

            //Salva i dati in violations, utile a set_time
            double distanceDifference = hw.getDistanceBetween('V', idGate1, idGate2); //Distanza tra due varchi
            double timeDifference = timeGate1 - timeGate2; //Differenza di tempo tra i due varchi
            double averageVelocity = distanceDifference / (timeDifference/SECONDS_IN_HOURS);

            //Se la velocità media supera i 130 aggiungo una violazione
            if(averageVelocity > 130){
                violations[mapElement.first].push_back({idGate1, idGate2, averageVelocity, timeGate1, timeGate2});
            }

            //Salva i dati in statistics, utile a stats
            updateStat(idGate2, timeGate2);

            //Aggiungo la distanza e il tempo percorso al totale (utile al comando stats)
            totalDistance += distanceDifference;
            totalTime += timeDifference;
        }
    }
    totalAverageVelocity = totalDistance / (totalTime/SECONDS_IN_HOURS);
}


std::string DataProcessor::set_time(const std::string& input){
    //Chiama decodeInput per decodificare l'input
    int addTime = decodeInput(input);

    if(addTime <= 0){
        throw std::runtime_error("Errore! Tempo inserito non valido!");
    }

    double newTime = currentTime + addTime;
    std::string output = "\nInfrazioni commesse tra gli istanti "+std::to_string(currentTime)+" e "+std::to_string(newTime)+":";

    //Scorre tutte le violazioni
    for (const auto& mapElement : violations) {
        std::size_t gatesNumber = mapElement.second.size();

        for(std::size_t i=0; i+1 < gatesNumber; i++){

            int gateStartId = mapElement.second[i].gateStartId;
            int gateEndId = mapElement.second[i].gateEndId;
            double gateStartTime = mapElement.second[i].gateStartTime;
            double gateEndTime = mapElement.second[i].gateEndTime;
            double averageVelocity = mapElement.second[i].averageVelocity;

            //Considera solo le violazioni che rientrano nel range di tempo richiesto da input
            if(gateStartTime > currentTime && gateEndTime < newTime){
                output += "\nInfrazione";
                output += "\nTarga: "+mapElement.first+"\nTratta: varco "+std::to_string(gateStartId)+" - varco "+std::to_string(gateEndId)+"\nVelocità media: "+std::to_string(averageVelocity)+"\nIstante di passaggio varco "+std::to_string(gateStartId)+": "+std::to_string(gateStartTime)+"\nIstante di passaggio varco "+std::to_string(gateEndId)+": "+std::to_string(gateEndTime)+"\n";
            }
        }
    }

    if(violations.size() == 0) output += "\nNessuna infrazione.";

    //Imposta il nuovo tempo
    currentTime = newTime;

    return output;
}

std::string DataProcessor::stats(){
    std::string output = "\nStatistiche autostrada:";
    int totalGates = hw.getSize('V');

    //Scorre tutti i varchi dell'autostrada
    for (std::size_t i=0; i < totalGates; i++) {
        int vehiclesNumber = statistics[i].vehiclesNumber;
        double minTime = statistics[i].minTime;
        double maxTime = statistics[i].maxTime;
        double vehiclePerMinute = 0.0;

        //Se sono transitati piu di un veicolo calcola vehiclePerMinute, altrimenti non e' possibile calcolare un valore valido e lascia 0
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

int DataProcessor::decodeInput(const std::string& input){
    bool hasM = false;
    std::string numberPart;
    int result;

    //Controlla l'ultimo carattere per verificare la presenza di 'm'
    if (input.back() == 'm') {
        hasM = true;
        numberPart = input.substr(0, input.size() - 1);
    } else {
        numberPart = input;
    }

    //La parte numerica deve esistere
    if (numberPart.empty()) throw std::runtime_error("Errore! File non valido: ");

    //Conversione, se la parte numerica non è valida lancia l'eccezione
    try {
        result = std::stoi(numberPart);
    } catch (...) {
        throw std::runtime_error("Errore! File non valido: ");
    }

    //Se è presente m, moltiplico per 60 per convertire in secondi
    if (hasM) {
        result *= SECONDS_IN_MINUTES;
    }

    return result;
}


static bool compareId(const PassageByPlate& p1, const PassageByPlate& p2) {
    return p1.id < p2.id;
}


void DataProcessor::updateStat(int id, double time){
    //Se il varco con questo id non era già presente nella mappa lo aggiunge
    Statistic& stat = statistics[id];

    //Incrementa il numero di veicoli che hanno attraversato questo varco
    stat.vehiclesNumber++;
    
    //Salva l'istante minimo e massimo in cui una macchina ha attraversato questo varco
    stat.minTime = std::min(stat.minTime, time); 
    stat.maxTime = std::max(stat.maxTime, time); 
}


void DataProcessor::loadFromFile(const std::string& filename){
    //Apertura file
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Errore! Impossibile aprire il file!");

    //Legge riga per riga
    std::string line;
    while (std::getline(file, line)) {
        
        std::stringstream ss(line);
        std::string singleWord;
        std::vector<std::string> words;

        //Legge ogni parola della riga e la salva nel vettore
        while (ss >> singleWord) {
            words.push_back(singleWord);
        }

        //Se ci sono esattamente 3 parole prova a convertire nei tipi corrispondenti, in caso di errore lancia un'eccezione
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
    //Chiusura file
    file.close();
}







