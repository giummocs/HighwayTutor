#include "Highway.h"

Highway::Highway() {
    nodes['V'];
    nodes['S'];
}

Highway::Highway(const std::string& filename) { loadFromFile(filename); }

void Highway::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) 
        throw std::runtime_error("Errore! Impossibile aprire file.");

    nodes['V'];
    nodes['S'];
    
    std::string line;
    while (std::getline(file, line)) {
        
        std::stringstream ss(line);
        std::string temp;
        std::vector<std::string> words;
        HighwayNode node;

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

        node.distance = std::stod(words[0]);
        nodes[words[1][0]].push_back(node);
    }

    file.close();
    // Validazione vincoli [cite: 25, 26, 27]
    if (nodes['V'].size() < 2) 
        throw std::runtime_error("Errore! Requisiti non soddisfatti: devono esserci almeno 2 varchi."); // Almeno due gates
    
    // Ordinamento per distanza [cite: 24]
    std::sort(nodes['V'].begin(), nodes['V'].end(), compareDistance);
    std::sort(nodes['S'].begin(), nodes['S'].end(), compareDistance);

    if (nodes['S'][0].distance > nodes['V'][0].distance || nodes['S'][nodes['S'].size()-1].distance < nodes['V'][nodes['V'].size()-1].distance) 
        throw std::runtime_error("Errore! Requisiti non soddisfatti: deve esserci uno svincolo prima del primo varco e uno svincolo dopo l'ultimo varco.");

    setAdjacent(nodes['S'], nodes['V']);
    setAdjacent(nodes['V'], nodes['S']);
}

const std::vector<HighwayNode>& Highway::getJunctions() {
    return nodes['S'];
}

const std::vector<HighwayNode>& Highway::getGates() {
    return nodes['V'];
}

double Highway::getDistance(char key, int index) {
    if (key != 'V' && key != 'S') {
        throw std::invalid_argument("Errore! Chiave non valida");
    }
    
    std::vector<HighwayNode>& v = nodes[key];
    
    if (index < 0 || index >= v.size()) {
        throw std::out_of_range("Errore! Indice non valido");
    }

    return v[index].distance;
}

int Highway::getPrev(char key, int index) {
    if (key != 'V' && key != 'S') {
        throw std::invalid_argument("Errore! Chiave non valida");
    }
    
    std::vector<HighwayNode>& v = nodes[key];
    
    if (index < 0 || index >= v.size()) {
        throw std::out_of_range("Errore! Indice non valido");
    }

    return v[index].prev;
}

int Highway::getNext(char key, int index) {
    if (key != 'V' && key != 'S') {
        throw std::invalid_argument("Errore! Chiave non valida");
    }
    
    std::vector<HighwayNode>& v = nodes[key];
    
    if (index < 0 || index >= v.size()) {
        throw std::out_of_range("Errore! Indice non valido");
    }

    return v[index].next;
}

double Highway::getDistanceBetween(char key, int i, int j) {
    if (key != 'V' && key != 'S') {
        throw std::invalid_argument("Errore! Chiave non valida");
    }
    
    std::vector<HighwayNode>& v = nodes[key];
    
    if (i < 0 || j >= v.size() || i >= j ) {
        throw std::out_of_range("Errore! Indice non valido");
    }

    return v[j].distance - v[i].distance;
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

bool Highway::compareDistance(const HighwayNode& a, const HighwayNode& b) {
    return a.distance < b.distance;
}

void Highway::setAdjacent(std::vector<HighwayNode>& a, std::vector<HighwayNode>& b) {
    for (int i = 0; i < a.size(); ++i) {
        a[i].prev = -2; // Inizializza come non trovato
        a[i].next = -2;

        for (int j = 0; j < b.size(); ++j) {
            double diff = a[i].distance - b[j].distance;

            
            if (diff > -1.0 && diff < 1.0) {
                throw std::runtime_error("Errore! Requisiti non soddisfatti: distanza minima tra uno svincolo e un varco deve essere minimo 1km.");
            }

            
            if (diff >= 1.0) {
                a[i].prev = j; 
            }
            
            else if (diff <= -1.0) {
                a[i].next = j;
                break;
            }
        }
    }
}

void Highway::printJunctions() {
    for (int i = 0; i < nodes['S'].size(); i++) {
        double dist = nodes['S'][i].distance;
        int next = nodes['S'][i].next;
        int prev = nodes['S'][i].prev;
        std::cout << "*****************************************************************\n";
        std::cout << "Svincolo " << i+1 << " distanza : " << dist << "km\n";
        std::cout << "Varco precedente : " << prev+1 << " successivo : " << next+1 << "\n";
    }
    return;
}

void Highway::printGates() {
    for (int i = 0; i < nodes['V'].size(); i++) {
        double dist = nodes['V'][i].distance;
        int next = nodes['V'][i].next;
        int prev = nodes['V'][i].prev;
        std::cout << "*****************************************************************\n";
        std::cout << "Varco " << i+1 << " distanza : " << dist << "km\n";
        std::cout << "Svincolo precedente : " << prev+1 << " successivo : " << next+1 << "\n";
    }
    return;
}
























