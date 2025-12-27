#ifndef HIGHWAY_H
#define HIGHWAY_H

#include <vector>
#include <string>

struct Point {
    double km;
    char type; // 'V' per Varco, 'S' per Svincolo
    int id;
};

class Highway {
public:
    std::vector<Point> points;
    bool loadFromFile(const std::string& filename);
    std::vector<Point> getVarchiInRange(double startKm, double endKm) const;
};

#endif