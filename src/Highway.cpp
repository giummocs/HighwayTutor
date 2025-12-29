#include "Highway.h"

void Highway::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) 
        throw std::runtime_error("Errore! File non aperto correttamente.") ;

    std::string line;
    while (std::getline(file, line)) {
        
        std::stringstream ss(line);
        std::string temp;
        std::vector<std::string> words;

        while (ss >> temp) {
            words.push_back(std::toupper(temp));
        }

        if (words.size() == 2) {
            if (isDouble(words[0])) {
                if (words[1] == "V"){
                    gates.push_back(std::stod(words[0]));
                }
                else if (words[1] == "S") {
                    junctions.push_back(std::stod(words[0]));
                }
            }
        }
    }

    file.close();
    // Validazione vincoli [cite: 25, 26, 27]
    if (gates.size() <= 2) 
        throw std::runtime_error("Errore! Requisiti non soddisfatti (almeno 2 svincoli)."); // Almeno due gates
    
    
    // Ordinamento per distanza [cite: 24]
    std::sort(gates.begin(), gates.end());
    std::sort(junctions.begin(), junctions.end());

    if (junctions[0] > gates[0] || junctions[junctions.size()-1] < gates[gates.size()-1]) 
        throw std::runtime_error("Errore! Requisiti non soddisfatti (almeno uno svincolo precedente al primo varco e almeno uno svincolo successivo all’ultimo varco).");
    
    int i = 0, j = 0;
    double minPrev = 0, minNext = 0;

    // 2. Usiamo due puntatori per scorrere gli array: O(N + M)
    while (i < gates.size() && j < junctions.size()) {
        double diff = gates[i] - junctions[j];

        if (diff > -1.0 && diff < 1.0) {
            throw std::runtime_error("Errore! Requisiti non soddisfatti (distanza minima tra svincolo e varco 1km)."); // Condizione violata
        }

        if (diff < 0.0 && diff > minPrev){
            minPrev = diff;
            adjacentGates[j].prev = i;
        }
        if (diff > 0.0 && diff < minNext){
            minNext = diff;
            adjacentGates[j].next = i;
        }

        // Muoviamo il puntatore dell'elemento più piccolo per avvicinarci all'altro
        if (gates[i] < junctions[j]) {
            
            i++;
        } else {
            j++;
        }
    }

    return;

}

const Highway::std::vector<double>& getJunctions(){
    return junctions;
}

const Highway::std::vector<double>& getGates(){
    return gates;
}

const Highway::std::vector<AdjacentGates>& getAdjacentGates(){
    return adjacentGates;
}

bool Highway::isDouble(const std::string& s) {
    try {
        std::stod(s);
        
    } 
    catch (...) {
        return false;
    }
    return true;
}











