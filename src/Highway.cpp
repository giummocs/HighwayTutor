#include "Highway.h"
#include <fstream>
#include <algorithm>
#include <iostream>

bool Highway::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    double km;
    char type;
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
                if ( words[1] == "V" || words[1] == "S"){
                    km = std::stod(words[0]);
                    type = words[1][0];
                    points.push_back({km, type, 0});
                }
            }
        }
        else {
            continue;
        }
    }

    file.close();
    
    // Ordinamento per distanza [cite: 24]
    std::sort(points.begin(), points.end(), confrontaPerKm);

    int vCount = 1, sCount = 1;
    for (int i = 0; i < points.size(); i++) {
        if (points[i].type == 'V') points[i].id = vCount++;
        else points[i].id = sCount++;
    }

    // Validazione vincoli [cite: 25, 26, 27]
    if (vCount <= 2) return false; // Almeno due varchi
    if (points.front().type != 'S' || points.back().type != 'S') return false;

    MAX_VARCHI = --vCount;
    MAX_SVINCOLI = --sCount;
    
    return true;

}

const Highway::std::vector<Point>& getPoints(){
    return points;
}

bool Highway::confrontaPerKm(const Point& a, const Point& b) {
    return a.km < b.km;
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



