#include "Highway.h"

Highway::Highway() {}

Highway::Highway(const std::string& filename) { loadFromFile(filename); }

void Highway::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) 
        throw std::runtime_error("Error! Unable to open file.");

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

        if (words.size() != 2 || !isDouble(words[0]) || words[1] != "V" || words[1] != "S") {
            throw std::runtime_error("Error! Invalid file format.");
        }

        node.distance = std::stod(words[0]);
        nodes[words[1]].push_back(node)
    }

    file.close();
    // Validazione vincoli [cite: 25, 26, 27]
    if (nodes['V'].size() <= 2) 
        throw std::runtime_error("Error! Requirements not met: at least two junctions required."); // Almeno due gates
    
    MAX_JUNCTIONS = nodes['S'].size();
    MAX_GATES = nodes['V'].size();
    
    // Ordinamento per distanza [cite: 24]
    std::sort(nodes['V'].begin(), nodes['V'].end(), compareDistance);
    std::sort(nodes['S'].begin(), nodes['S'].end(), compareDistance);

    if (nodes['S'][0] > nodes['V'][0] || nodes['S'][nodes['S'].size()-1] < nodes['V'][nodes['V'].size()-1]) 
        throw std::runtime_error("Error! Requirements not met: at least one junction before the first gate and at least one junction after the last gate.");

    setAdjacent(nodes['S'], nodes['V']);
    setAdjacent(nodes['V'], nodes['S']);
}

const Highway::std::vector<double>& getJunctions() {
    return nodes['S'];
}

const Highway::std::vector<double>& getGates(){
    return nodes['V'];
}

double Highway::getDistance(char key, int index) {
    if (key != 'V' || key != 'S') {
        throw std::invalid_argument("Error! Invalid key");
    }
    
    std::vector<HighwayNode>& v = nodes[key];
    
    if (index >= 0 && index < v.size()) {
            return v[index].distance;
    }

    throw std::out_of_range("Error! Invalid index");
}

int Highway::getPrev(char key, int index) {
    if (key != 'V' || key != 'S') {
        throw std::invalid_argument("Error! Invalid key");
    }
    
    std::vector<HighwayNode>& v = nodes[key];
    
    if (index >= 0 && index < v.size()) {
            return v[index].prev;
    }

    throw std::out_of_range("Error! Invalid index");
}

int Highway::getNext(char key, int index) {
    if (key != 'V' || key != 'S') {
        throw std::invalid_argument("Error! Invalid key");
    }
    
    std::vector<HighwayNode>& v = nodes[key];
    
    if (index >= 0 && index < v.size()) {
            return v[index].next;
    }

    throw std::out_of_range("Error! Invalid index");
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

void Highway::setAdjacent(std::vector<HighwayNode> a, std::vector<HighwayNode> b) {
    int i = 0, j = 0;
    double minPrev = 0.0, minNext = 0.0;
    while (i < a.size() && j < b.size()) {
        double diff = a[i].distance - b[j].distance;

        if (diff > -1.0 && diff < 1.0) {
            throw std::runtime_error("Error! Requirements not met: minimum distance between interchange and gate must be at least 1km."); // Condizione violata
        }
        
        if (diff < 0.0 && diff > minPrev){
            minPrev = diff;
            a[j].prev = i;
        }
        if (diff > 0.0 && diff < minNext){
            minNext = diff;
            b[j].next = i;
        }

        // Muoviamo il puntatore dell'elemento più piccolo per avvicinarci all'altro
        if (a[i] < b[j]) {
            
            i++;
        } 
        else {
            j++;
        }
    }

    return;
}















