#include "Highway.h"
#include <fstream>
#include <algorithm>
#include <iostream>

bool Highway::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    double km;
    char type;
    while (file >> km >> type) {
        points.push_back({km, toupper(type), 0});
    }

    // Ordinamento per distanza [cite: 24]
    std::sort(points.begin(), points.end(), confrontaPerKm);

    int vCount = 1, sCount = 1;
    for (int i = 0; i < points.size(); i++) {
        if (points[i].type == 'V') points[i].id = vCount++;
        else points[i].id = sCount++;
    }

    MAX_VARCHI = --vCount;
    MAX_SVINCOLI = --sCount;
    
    // Validazione vincoli [cite: 25, 26, 27]
    if (vCount <= 2) return false; // Almeno due varchi
    if (points.front().type != 'S' || points.back().type != 'S') return false; 

    file.close();
    
    return true;

}

const Highway::std::vector<Point>& getPoints(){
    return points;
}

bool Highway::confrontaPerKm(const Point& a, const Point& b) {
    return a.km < b.km;
}

