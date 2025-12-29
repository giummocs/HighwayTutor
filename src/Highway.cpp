#include "Highway.h"
#include <fstream>
#include <algorithm>
#include <iostream>

bool Highway::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

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
                    varchi.push_back(std::stod(words[0]));
                }
                else if (words[1] == "S") {
                    svincoli.push_back(std::stod(words[0]));
                }
            }
        }
    }

    file.close();
    // Validazione vincoli [cite: 25, 26, 27]
    if (varchi.size() <= 2) return false; // Almeno due varchi
    
    
    // Ordinamento per distanza [cite: 24]
    std::sort(varchi.begin(), varchi.end());
    std::sort(svincoli.begin(), svincoli.end());

    if (svincoli[0] > varchi[0] || svincoli[svincoli.size()-1] < varchi[varchi.size()-1]) return false;
    
    int i = 0, j = 0;

    // 2. Usiamo due puntatori per scorrere gli array: O(N + M)
    while (i < varchi.size() && j < svincoli.size()) {
        double diff = std::abs(varchi[i] - svincoli[j]);

        if (diff < 1.0) {
            return false; // Condizione violata
        }

        // Muoviamo il puntatore dell'elemento più piccolo per avvicinarci all'altro
        if (varchi[i] < svincoli[j]) {
            
            i++;
        } else {
            j++;
        }
    }

    return true;

}

const Highway::std::vector<double>& getSvincoli(){
    return svincoli;
}

const Highway::std::vector<double>& getVarchi(){
    return varchi;
}

bool Highway::isDouble(const std::string& s) {
    try {
        std::stod(s);
        return true;
    } 
    catch (...) {
        return false;
    }
}








