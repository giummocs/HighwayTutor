#include "Highway.h"

Highway::Highway() {
    //Inizializzazione a valori di default 
    nodes['V'];
    nodes['S'];
}

Highway::Highway(const std::string& filename) { loadFromFile(filename); }

void Highway::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) 
        throw std::runtime_error("Errore! Impossibile aprire file.");

    nodes['S'].clear();
    nodes['V'].clear();
    
    std::string line;
    while (std::getline(file, line)) {
        
        std::stringstream ss(line);
        std::string temp;
        std::vector<std::string> words;
        double distance;

        while (ss >> temp) {
            for (int i = 0; i < temp.length(); i++) {
                temp[i] = (char)std::toupper(static_cast<unsigned char>(temp[i]));
            }
            words.push_back(temp);
        }

        if (words.size() != 2) {
            throw std::runtime_error("Errore! Ogni riga deve contenere 2 argomenti.");
        }
        if (!isDouble(words[0])) {
            throw std::runtime_error("Errore! Il primo argomento deve essere un valore Double.");
        }
        if (std::stod(words[0]) < 0) {
            throw std::runtime_error("Errore! Il primo argomento non deve essere un valore negativo.");
        }
        if ((words[1] != "V" && words[1] != "S")) {
            throw std::runtime_error("Errore! Il secondo argomento deve essere 'S' oppure 'V' (case-insensitive).");
        }
        
        distance = std::stod(words[0]);
        nodes[words[1][0]].push_back(distance);
    }

    file.close();
    // Validazione vincoli [cite: 25, 26, 27]
    if (nodes['V'].size() < 2) 
        throw std::runtime_error("Errore! Requisiti non soddisfatti: devono esserci almeno 2 varchi."); // Almeno due gates
    
    // Ordinamento per distanza [cite: 24]
    std::sort(nodes['V'].begin(), nodes['V'].end());
    std::sort(nodes['S'].begin(), nodes['S'].end());

    if (nodes['S'][0] > nodes['V'][0] || nodes['S'][nodes['S'].size()-1] < nodes['V'][nodes['V'].size()-1]) 
        throw std::runtime_error("Errore! Requisiti non soddisfatti: deve esserci uno svincolo prima del primo varco e uno svincolo dopo l'ultimo varco.");
}

const std::vector<double>& Highway::getJunctions() {
    return nodes['S'];
}

const std::vector<double>& Highway::getGates() {
    return nodes['V'];
}

int Highway::getSize(char key) {
    if (key == 'V') {
        return nodes['V'].size();
    }
    if (key == 'S') {
        return nodes['S'].size();
    }
    throw std::invalid_argument("Errore! Chiave non valida");
}

double Highway::getDistance(char key, int id) {
    if (key != 'V' && key != 'S') {
        throw std::invalid_argument("Errore! Chiave non valida");
    }
    
    std::vector<double>& v = nodes[key];
    
    if (id < 1 || id > v.size()) {
        throw std::out_of_range("Errore! Id non valido");
    }

    return v[id-1];
}

double Highway::getDistanceBetween(char key, int id1, int id2) {
    if (key != 'V' && key != 'S') {
        throw std::invalid_argument("Errore! Chiave non valida");
    }
    
    std::vector<double>& v = nodes[key];
    
    if (id1 < 1 || id2 > v.size() || id1 >= id2 ) {
        throw std::out_of_range("Errore! Indice non valido");
    }

    return v[id2-1] - v[id1-1];
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

void Highway::printJunctions() {
    for (int i = 0; i < nodes['S'].size(); i++) {
        double dist = nodes['S'][i];
        std::cout << "*****************************************************************\n";
        std::cout << "Svincolo " << i+1 << " distanza : " << dist << "km\n";
    }
    return;
}

void Highway::printGates() {
    for (int i = 0; i < nodes['V'].size(); i++) {
        double dist = nodes['V'][i];
        std::cout << "*****************************************************************\n";
        std::cout << "Varco " << i+1 << " distanza : " << dist << "km\n";
    }
    return;
}






























