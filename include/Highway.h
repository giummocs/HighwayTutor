#ifndef HIGHWAY_H
#define HIGHWAY_H

#include <fstream>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

struct HighwayNode {
    double distance;
    int prev;
    int next;
}

class Highway {
public:
    const int MAX_JUNCTIONS;
    const int MAX_GATES;

    Highway();
    Highway(const std::string& filename);
    void loadFromFile(const std::string& filename);
    const std::vector<HighwayNode>& getJuctions();
    const std::vector<HighwayNode>& getGates();
    double getDistance(char key, int index);
    int getPrev(char key, int index);
    int getNext(char key, int index);

private:
    //Abbiamo scelto di implementare due vettori per differenziare al meglio varchi e svincoli, al posto di una struttura dati univoca per entrambi (struct).
    //Il che facilita molto la logica negli altri file: codice piu facile da leggere e alleggerito nella ricerca di varchi e svincoli.
    std::unordered_map<std::string, std::vector<HighwayNode>> nodes;

    bool isDouble(const std::string& s);
    bool compareDistance(const HighwayNode& a, const HighwayNode& b);
    void setAdjacent(std::vector<HighwayNode> a, std::vector<HighwayNode> b);
};


#endif























