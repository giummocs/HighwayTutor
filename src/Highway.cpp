#include "Highway.h"

Highway::Highway() {}

Highway::Highway(const std::string& filename) { loadFromFile(filename); }

void Highway::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) 
        throw std::runtime_error("Errore! File non aperto correttamente.") ;

    nodes['V'] = {};
    nodes['S'] = {};
    
    std::string line;
    while (std::getline(file, line)) {
        
        std::stringstream ss(line);
        std::string temp;
        std::vector<std::string> words;
        HighwayNode node;

        while (ss >> temp) {
            words.push_back(std::toupper(temp));
        }

        if (words.size() == 2) {
            if (isDouble(words[0])) {
                if (words[1] == "V" || words[1] == "S"){
                    node.distance = std::stod(words[0]);
                    nodes[words[1]].push_back(node);
                }
            }
        }
    }

    file.close();
    // Validazione vincoli [cite: 25, 26, 27]
    if (nodes['V'].size() <= 2) 
        throw std::runtime_error("Errore! Requisiti non soddisfatti (almeno 2 svincoli)."); // Almeno due gates
    
    MAX_JUNCTIONS = nodes['S'].size();
    MAX_GATES = nodes['V'].size();
    
    // Ordinamento per distanza [cite: 24]
    std::sort(nodes['V'].begin(), nodes['V'].end(), compareDistance);
    std::sort(nodes['S'].begin(), nodes['S'].end(), compareDistance);

    if (nodes['S'][0] > nodes['V'][0] || nodes['S'][nodes['S'].size()-1] < nodes['V'][nodes['V'].size()-1]) 
        throw std::runtime_error("Errore! Requisiti non soddisfatti (almeno uno svincolo precedente al primo varco e almeno uno svincolo successivo all’ultimo varco).");
    
    int i = 0, j = 0;
    double minPrev = 0.0, minNext = 0.0;

    // 2. Usiamo due puntatori per scorrere gli array: O(N + M)
    while (i < nodes['V'].size() && j < nodes['S'].size()) {
        double diff = nodes['V'][i].distance - nodes['S'][j].distance;

        if (diff > -1.0 && diff < 1.0) {
            throw std::runtime_error("Errore! Requisiti non soddisfatti (distanza minima tra svincolo e varco 1km)."); // Condizione violata
        }

        if (diff < 0.0 && diff > minPrev){
            minPrev = diff;
            nodes['S'][j].prev = i;
        }
        if (diff > 0.0 && diff < minNext){
            minNext = diff;
            nodes['S'][j].next = i;
        }

        // Muoviamo il puntatore dell'elemento più piccolo per avvicinarci all'altro
        if (nodes['V'][i] < nodes['S'][j]) {
            
            i++;
        } else {
            j++;
        }
        
        return;
    }

    i = 0;
    j = 0;
    minPrev = 0.0;
    minNext = 0.0;
    while (i < nodes['S'].size() && j < nodes['V'].size()) {
        double diff = nodes['S'][i].distance - nodes['V'][j].distance;

        if (diff < 0.0 && diff > minPrev){
            minPrev = diff;
            nodes['V'][j].prev = i;
        }
        if (diff > 0.0 && diff < minNext){
            minNext = diff;
            nodes['V'][j].next = i;
        }

        // Muoviamo il puntatore dell'elemento più piccolo per avvicinarci all'altro
        if (nodes['S'][i] < nodes['V'][j]) {
            
            i++;
        } else {
            j++;
        }
    }
}

const Highway::std::vector<double>& getJunctions(){
    return nodes['S'];
}

const Highway::std::vector<double>& getGates(){
    return nodes['V'];
}

double getDistance(char c, int index) {
    if (c != 'V' || c != 'S') {
        throw std::invalid_argument("Errore! Chiave non corretto");
    }
    
    std::vector<HighwayNode>& v = mappa[c];
    
    if (index >= 0 && index < v.size()) {
            return v[index].distance;
    }

    throw std::out_of_range("Errore! Indice invalido");
}

int getPrev(char c, int index) {
    if (c != 'V' || c != 'S') {
        throw std::invalid_argument("Errore! Chiave non corretto");
    }
    
    std::vector<HighwayNode>& v = mappa[c];
    
    if (index >= 0 && index < v.size()) {
            return v[index].prev;
    }

    throw std::out_of_range("Errore! Indice invalido");
}

int Highway::getNext(char c, int index) {
    if (c != 'V' || c != 'S') {
        throw std::invalid_argument("Errore! Chiave non corretto");
    }
    
    std::vector<HighwayNode>& v = mappa[c];
    
    if (index >= 0 && index < v.size()) {
            return v[index].next;
    }

    throw std::out_of_range("Errore! Indice invalido");
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

bool Highway::compareDistance(const HighwatNode& a, const HighwayNode& b) {
    return a.distance < b.distance;
}











