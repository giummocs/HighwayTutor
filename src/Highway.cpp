#include "Highway.h"

Highway::Highway() {
    //Creazione dei due vettori vuoti (li svuota se esistono)
    nodes['V'].clear();
    nodes['S'].clear();
}

Highway::Highway(const std::string& filename) { loadFromFile(filename); }

void Highway::loadFromFile(const std::string& filename) {
    //Apertura file
    std::ifstream file(filename);
    if (!file.is_open()) 
        throw std::runtime_error("Errore! Impossibile aprire file.");

    //Creazione dei due vettori vuoti (li svuota se esistono) (permette l'overwrite dei dati)
    nodes['S'].clear();
    nodes['V'].clear();

    //Lettura da file
    std::string line;
    while (std::getline(file, line)) {
        
        std::stringstream ss(line);
        std::string temp;
        std::vector<std::string> words;
        double distance;

        while (ss >> temp) {
            for (int i = 0; i < temp.length(); i++) {
                //Trasformazione in UpperCase delle stringhe in modo da rendere case-insensitive il programma
                temp[i] = (char)std::toupper(static_cast<unsigned char>(temp[i]));
            }
            words.push_back(temp);
        }
        
        //Verifica della formattazione del contenuto
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
    
    //Verifica dei requisiti
    if (nodes['V'].size() < 2) 
        throw std::runtime_error("Errore! Requisiti non soddisfatti: devono esserci almeno 2 varchi."); // Almeno due gates
    
    //Ordinamento per distanza
    std::sort(nodes['V'].begin(), nodes['V'].end());
    std::sort(nodes['S'].begin(), nodes['S'].end());

    //Verifica dei requisiti
    if (nodes['S'][0] > nodes['V'][0] || nodes['S'][nodes['S'].size()-1] < nodes['V'][nodes['V'].size()-1]) 
        throw std::runtime_error("Errore! Requisiti non soddisfatti: deve esserci uno svincolo prima del primo varco e uno svincolo dopo l'ultimo varco.");
}

const std::vector<double>& Highway::getJunctions() {
    //[] invece che at() perche' elemento con chiave 'S' viene sicuramente creato con qualunque costruttore
    return nodes['S'];
}

const std::vector<double>& Highway::getGates() {
    //[] invece che at() perche' elemento con chiave 'S' viene sicuramente creato con qualunque costruttore
    return nodes['V'];
}

int Highway::getSize(char key) {
    if (!isValidKey(key)) {
        throw std::invalid_argument("Errore! Chiave non valida");
    }
    return nodes[key].size();
}

double Highway::getDistance(char key, int id) {
    if (!isValidKey(key)) {
        throw std::invalid_argument("Errore! Chiave non valida");
    }
    
    std::vector<double>& v = nodes[key];
    
    if (!isValidId(id, v)) {
        throw std::out_of_range("Errore! Id non valido");
    }

    return v[id-1];
}

double Highway::getDistanceBetween(char key, int id1, int id2) {
    if (!isValidKey(key)) {
        throw std::invalid_argument("Errore! Chiave non valida");
    }
    
    std::vector<double>& v = nodes[key];
    
    if (isValidId(id1, v) && isValidId(id2, v)) {
        return std::abs(v[id2-1] - v[id1-1]);
    }

    throw std::out_of_range("Errore! Indice non valido");
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

bool Highway::isValidKey(char key) {
    return (key == 'S' || key == 'V');
}

bool Highway::isValidId(int id, std::vector<double> v) {
    return (id >= 1 && id <= v.size());
}


































