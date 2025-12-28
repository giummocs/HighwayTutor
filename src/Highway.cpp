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
        points.push_back({km, type, 0});
    }

    // Ordinamento per distanza [cite: 24]
    std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
        return a.km < b.km;
    });

    int vCount = 1, sCount = 1;
    for (auto& p : points) {
        if (p.type == 'V') p.id = vCount++;
        else p.id = sCount++;
    }

    // Validazione vincoli [cite: 25, 26, 27]
    if (vCount <= 2) return false; // Almeno due varchi
    if (points.front().type != 'S' || points.back().type != 'S') return false; 

    file.close();
    
    return true;

}
