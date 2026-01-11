//FILE REALIZZATO DA STEFANI GIANMARIA

#include "DataProcessor.h"

//Costruttore di default
DataProcessor::DataProcessor() : hw(){
    currentTime = 0;
    totalViolations = 0;
}

//Costruttore usato per costruire e processare tutti i dati necessari
DataProcessor::DataProcessor(std::string filenameHighway, std::string filenamePassages) : hw(filenameHighway){
    currentTime = 0;
    totalViolations = 0;
    processData(filenamePassages);
}


void DataProcessor::processData(std::string filenamePassages){
    //Variabile temporanea usata solo per facilitare il popolamento di violations e statistics
    //Contiene tutti i dati letti da passages.txt, organizzati per targa tramite la chiave
    std::unordered_map<std::string, std::vector<PassageByPlate>> passages; 

    //Legge i dati del file
    loadFromFile(filenamePassages, passages);

    //Complessita' totale O(n), dove n e' il numero delle righe del file passages.txt
    //Scorre la mappa con chiave plate, cosi da scorrere un veicolo per volta
    //Ogni elemento della mappa, cioè ogni veicolo, corrisponde a un vettore che contiene tutti i varchi che il veicolo ha attraversato
    for (auto& mapElement : passages) {
        
        //Ordina i passaggi per id, cosi da riempire le mappe con gli id gia ordinati
        std::sort(mapElement.second.begin(), mapElement.second.end(), comparePassage);

        //Numero di varchi che il veicolo ha attraversato
        int gatesNumber = mapElement.second.size();

        //Salva il primo varco
        statistics[mapElement.second[0].id].push_back(mapElement.second[0].time);

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
            statistics[idGate2].push_back(timeGate2);

            segments.push_back({timeGate2, distanceDifference, timeDifference});
            
        }
    }

    //Ordina i tempi per ogni varco, per permettere poi l'uso di upper_bound()
    for (auto& mapElement : statistics) {
        std::sort(mapElement.second.begin(), mapElement.second.end());
    }

    //Ordina i segmenti per ordine di tempo, per permettere poi l'uso di upper_bound()
    std::sort(segments.begin(), segments.end(), compareSegment);
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
            if(gateEndTime > currentTime && gateEndTime <= newTime){
                output << "\n\n-Infrazione-"
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
                totalViolations++; //Totale delle violazioni nel tempo corrente, utile a stats
            }
        }
    }
    //Imposta il nuovo tempo
    currentTime = newTime;

    //Se non e' presente alcuna violazione aggiorna l'output
    if(noViolations) return output.str()+"\nNessuna infrazione nell'intervallo di tempo inserito.";

    return output.str();
}

std::string DataProcessor::stats(){
    //Numero totale di varchi nel file highway
    int totalGates = hw.getSize('V');

    //Costruzione dell'output
    std::ostringstream output;
    output << "\nStatistiche autostrada dall'istante 0 fino a " << currentTime <<":";

    output << buildGateStats();

    output << buildStats();
    
    return output.str();
}

std::string DataProcessor::reset(){
    currentTime = 0;
    totalViolations = 0;
    return "\nSistema azzerrato con successo!";
}

int DataProcessor::decodeInput(const std::string& input){
    bool hasM = false;
    std::string numberPart = "";
    int result = 0;

    //Controlla l'ultimo carattere per verificare la presenza di 'm'
    if (input.back() == 'm') {
        hasM = true;
        numberPart = input.substr(0, input.size() - 1);
    } else {
        numberPart = input;
    }

    //La parte numerica deve esistere
    if (numberPart.empty()) throw std::runtime_error("Errore! Parametro non valido!");

    //Conversione, se la parte numerica non è valida lancia l'eccezione
    if(!stringToInt(numberPart, result)) throw std::runtime_error("Errore! Parametro non valido!");

    //Se è presente m, moltiplico per 60 per convertire in secondi
    if (hasM) {
        result *= SECONDS_IN_MINUTES;
    }

    return result;
}


bool DataProcessor::comparePassage(const PassageByPlate& p1, const PassageByPlate& p2) {
    //Confronto di id
    return p1.id < p2.id;
}

bool DataProcessor::compareSegment(const TripSegment& t1, const TripSegment& t2) {
    //Confronto di time
    return t1.endTime < t2.endTime;
}

bool DataProcessor::compareTime(double value, const TripSegment& t1) {
    //Confronto di time
    return value < t1.endTime;
}

bool DataProcessor::stringToInt(const std::string& numberPart, int& result) {
    //Se std::stoi() lancia un'eccezione, viene catturata e ritorna false
    //Se pos non corrisponde alla lunghezza totale della stringa allora la conversione è fallita
    try {
        //Pos rappresenta la posizione in cui la conversione si ferma se trova un valore non valido
        size_t pos;
        result = std::stoi(numberPart, &pos);
        return pos == numberPart.size();
    } catch (...) {
        return false;
    }
}

bool DataProcessor::stringToDouble(const std::string& numberPart, double& result) {
    //Se std::stod() lancia un'eccezione, viene catturata e ritorna false
    //Se pos non corrisponde alla lunghezza totale della stringa allora la conversione è fallita
    try {
        //Pos rappresenta la posizione in cui la conversione si ferma se trova un valore non valido
        size_t pos;
        result = std::stod(numberPart, &pos);
        return pos == numberPart.size();
    } catch (...) {
        return false;
    }
}

std::string DataProcessor::buildGateStats(){
    std::ostringstream output;
    
    //Se non c'è nessuna statistica
    if(statistics.size() == 0) return output.str()+"\nNessuna statistica disponibile, nessun veicolo transitato in nessun varco.";

    //Scorre tutti i varchi dell'autostrada
    for (int i=1; i <= totalGates; i++) {
        //Accede al vettore dei tempi, del varco corrente
        const std::vector<double>& times = statistics[i];

        //Cerca l'indice del primo tempo piu grande di currentTime
        //Upper_bound restituisce l'iteratore del primo tempo piu grande di currentTime
        //Distance poi restituisce la distanza tra l'inizio e l'iteratore, ovvero l'indice dell'elemento cercato
        int vehiclesCount = std::distance(times.begin(), std::upper_bound(times.begin(), times.end(), currentTime));

        double vehiclePerMinute = 0.0;

        //Se ci sono almeno due veicoli, calcolo la frequenza, altrimenti rimane 0
        if (vehiclesCount > 1) {
            double timeDifference = times[vehiclesCount - 1] - times[0];
            
            if (timeDifference > 0) {
                vehiclePerMinute = vehiclesCount / (timeDifference / SECONDS_IN_MINUTES);
            }
        }

        output << "\nVarco " << i << ": " << vehiclesCount << " veicoli transitati, " 
            << std::fixed << std::setprecision(2) << vehiclePerMinute << " veicoli al minuto.";
    }

    return output.str();
}

std::string DataProcessor::buildStats(){
    std::ostringstream output;
    double totalDistance = 0.0;
    double totalTime = 0.0;
    double totalAverageSpeed = 0.0;

    //Uso distance() e upper_bound() nello stesso modo di prima
    //Cerca l'indice del primo tempo piu grande di currentTime
    int segmentCount = std::distance(segments.begin(), std::upper_bound(segments.begin(), segments.end(), currentTime, compareTime));
    for (int i = 0; i < segmentCount; ++i) {
        totalDistance += segments[i].distance;
        totalTime += segments[i].duration;
    }

    //Se il tempo è valido calcola la velocità media
    if (totalTime > 0) {
        totalAverageSpeed = totalDistance / (totalTime / SECONDS_IN_HOURS);
    }

    output << "\nVelocità media totale: " << std::fixed << std::setprecision(2) << totalAverageSpeed << "km/h.";
    output << "\nNumero di veicoli sanzionati: " << totalViolations << ".";

    return output.str();
}

void DataProcessor::loadFromFile(const std::string& filename, std::unordered_map<std::string, std::vector<PassageByPlate>>& passages){
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
            int id;
            double time;
            std::string plate = words[1];

            //Conversione, se anche solo una conversione fallisce lancia un eccezione
            if(!stringToInt(words[0], id) || !stringToDouble(words[2], time)) throw std::runtime_error("Errore! File non valido: impossibile convertire i numeri!");
            
            passages[plate].push_back({id, time});
        }
        else{
            throw std::runtime_error("Errore! File non valido: numero di valori errato!");
        }
    }
    //Chiusura file
    file.close();
}










































