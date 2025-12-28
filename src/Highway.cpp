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
    
    // Ordinamento per distanza [cite: 24]
    std::sort(varchi.begin(), varchi.end());
    std::sort(svincoli.begin(), svincoli.end());

    // Validazione vincoli [cite: 25, 26, 27]
    if (varchi.size() <= 2) return false; // Almeno due varchi
    if (svincoli[0] > varchi[0] || svincoli[svincoli.size()-1] < varchi[varchi.size()-1]) return false;
    
    return true;

}

const Highway::std::vector<Point>& getSvincoli(){
    return svincoli;
}

const Highway::std::vector<Point>& getVarchi(){
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





