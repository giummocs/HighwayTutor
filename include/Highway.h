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
    const int MAX_SVINCOLI;
    const int MAX_VARCHI;

    bool loadFromFile(const std::string& filename);
    std::vector<Point> getVarchiInRange(double startKm, double endKm) const;
    Point getPoint(int i);

private:
    std::vector<Point> points;
};


#endif

