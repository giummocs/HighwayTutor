#ifndef HIGHWAY_H
#define HIGHWAY_H

#include <fstream>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <unordered_map>

class Highway {
public:
    //Costruttori
    Highway();
    Highway(const std::string& filename);
    void loadFromFile(const std::string& filename);
    //Funzioni getter 
    const std::vector<double>& getJunctions();
    const std::vector<double>& getGates();

    int getSize(char key);
    double getDistance(char key, int id);
    double getDistanceBetween(char key, int id1, int id2);

    //Funzioni aggiuntive non inerenti al nostro progetto ma potrebbero essere utili in un altro contesto
    void printJunctions();
    void printGates();

private:
    //Abbiamo scelto di implementare una mappa contenente due vettori per differenziare al meglio varchi e svincoli le cui chiavi corrispondono all'input del file di testo (S o V).
    //Il che facilita molto la logica negli altri file: codice piu facile da leggere e alleggerito nella ricerca di varchi e svincoli.
    std::unordered_map<char, std::vector<double>> nodes;

    //Helper functions
    bool isDouble(const std::string& s);
};


#endif




































