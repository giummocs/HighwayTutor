//FILE REALIZZATO DA STEFANI GIANMARIA

#include "DataProcessor.h"

//Costruttore di default
DataProcessor::DataProcessor() : hw(){
    currentTime = 0;
}

//Costruttore usato per costruire e processare tutti i dati necessari
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
    for (auto& mapElement : passages) {
        
        //Ordina i passaggi per id, cosi da riempire le mappe con gli id gia ordinati
        std::sort(mapElement.second.begin(), mapElement.second.end(), compareId);

        //Numero di varchi che il veicolo ha attraversato
        int gatesNumber = mapElement.second.size();

        //Salva il primo varco
        updateStat(mapElement.second[0].id, mapElement.second[0].time);

        //Scorre tutti i varchi attraversati da un veicolo preciso, analizzandone due consecutivi alla volta, tramite i e i+1
        for(int i=0; i+1 < gatesNumber; i++){
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
            double timeDifference = timeGate2 - timeGate1; //Differenza di tempo tra i due varchi
            double averageSpeed = distanceDifference / (timeDifference/SECONDS_IN_HOURS);

            //Se la velocità media supera i 130 aggiungo una violazione
            if(averageSpeed > 130){
                violations[mapElement.first].push_back({idGate1, idGate2, averageSpeed, timeGate1, timeGate2});
            }

            //Salva i dati in statistics, utile a stats
            updateStat(idGate2, timeGate2);

            //Aggiunge la distanza e il tempo percorso al totale (utile al comando stats)
            totalDistance += distanceDifference;
            totalTime += timeDifference;
        }
    }
    totalAverageSpeed = totalDistance / (totalTime/SECONDS_IN_HOURS);
}


std::string DataProcessor::set_time(const std::string& input){
    bool noViolations = true;
    
    //Chiama decodeInput per decodificare l'input
    int addTime = decodeInput(input);
    if(addTime <= 0){
        throw std::runtime_error("Errore! Tempo inserito non valido!");
    }
    double newTime = currentTime + addTime;

    //Costruzione dell'output
    std::ostringstream output;
    output << "\nInfrazioni commesse tra gli istanti " << currentTime << " e " << newTime << ":";

    //Scorre tutte le violazioni
    for (const auto& mapElement : violations) {
        int gatesNumber = mapElement.second.size();

        for(int i=0; i < gatesNumber; i++){
            int gateStartId = mapElement.second[i].gateStartId;
            int gateEndId = mapElement.second[i].gateEndId;
            double gateStartTime = mapElement.second[i].gateStartTime;
            double gateEndTime = mapElement.second[i].gateEndTime;
            double averageSpeed = mapElement.second[i].averageSpeed;

            //Considera solo le violazioni che rientrano nel range di tempo richiesto da input
            if(gateStartTime > currentTime && gateEndTime < newTime){
                output << "\nInfrazione n." << (i + 1)
                    << "\nTarga: " << mapElement.first
                    << "\nTratta: varco " << gateStartId
                    << " - varco " << gateEndId
                    << "\nVelocità media: "
                    << std::fixed << std::setprecision(2) << averageSpeed
                    << "\nIstante di passaggio varco " << gateStartId << ": "
                    << std::fixed << std::setprecision(2) << gateStartTime
                    << "\nIstante di passaggio varco " << gateEndId << ": "
                    << std::fixed << std::setprecision(2) << gateEndTime;
                
                noViolations = false; //E' presente almeno una violazione
            }
        }
    }

    //Se non e' presente alcuna violazione aggiorna l'output
    if(noViolations) return "\nNessuna infrazione nell'intervallo di tempo inserito.";

    //Imposta il nuovo tempo
    currentTime = newTime;

    return output.str();
}

std::string DataProcessor::stats(){
    int totalGates = hw.getSize('V');

    //Costruzione dell'output
    std::ostringstream output;
    output << "\nStatistiche autostrada:";

    //Se non c'è nessuna statistica
    if(statistics.size() == 0) output << "\nNessun veicolo transitato in nessun varco.";

    //Scorre tutti i varchi dell'autostrada
    for (int i=1; i <= totalGates; i++) {
        int vehiclesNumber = statistics[i].vehiclesNumber;
        double minTime = statistics[i].minTime;
        double maxTime = statistics[i].maxTime;
        double vehiclePerMinute = 0.0;

        //Se sono transitati piu di un veicolo allora calcola vehiclePerMinute, altrimenti non e' possibile calcolare un valore valido e lascia 0
        if(vehiclesNumber > 1){
            vehiclePerMinute = vehiclesNumber / ((maxTime-minTime)/SECONDS_IN_MINUTES);
        }

        output << "\nVarco " << i << ": " << vehiclesNumber << " veicoli transitati, " 
            << std::fixed << std::setprecision(2) << vehiclePerMinute << " veicoli al minuto.";
    }

    output << "\nVelocità media totale: " << std::fixed << std::setprecision(2) << totalAverageSpeed << ".";
    output << "\nNumero di veicoli sanzionati: " << violations.size() << ".";
    
    return output.str();
}

std::string DataProcessor::reset(){
    //Resetta il tempo a 0
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
    if (numberPart.empty()) throw std::runtime_error("Errore! Comando non valido!");

    //Conversione, se la parte numerica non è valida lancia l'eccezione
    try {
        result = std::stoi(numberPart);
    } catch (...) {
        throw std::runtime_error("Errore! Comando non valido!");
    }

    //Se è presente m, moltiplico per 60 per convertire in secondi
    if (hasM) {
        result *= SECONDS_IN_MINUTES;
    }

    return result;
}


bool DataProcessor::compareId(const PassageByPlate& p1, const PassageByPlate& p2) {
    //Confronto di id
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
                double time = std::stod(words[2]);

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

























