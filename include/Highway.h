#ifndef HIGHWAY_H
#define HIGHWAY_H

#include <vector>
#include <string>

struct AdjacentGates {
    int prev;
    int next;
}

class Highway {
public:
    const int MAX_JUNCTIONS;
    const int MAX_GATES;

    bool loadFromFile(const std::string& filename);
    const std::vector<double>& getJuctions();
    const std::vector<double>& getGates();
    const std::vector<VarchiAdiacenti>& getAdjacentGates();

private:
    //Abbiamo scelto di implementare due vettori per differenziare al meglio varchi e svincoli, al posto di una struttura dati univoca per entrambi (struct).
    //Il che facilita molto la logica negli altri file: codice piu facile da leggere e alleggerito nella ricerca di varchi e svincoli.
    std::vector<double> Junctions;
    std::vector<double> Gates;
    std::vector<AdjacentGates> adjacentGates;

    bool isDouble(const std::string& s);
};


#endif
















