#ifndef HIGHWAY_H
#define HIGHWAY_H

#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <stdexcept>

class Highway {
public:
    //Costruttori
    Highway();
    Highway(const std::string& filename);

    //Funzione per importare dati da file (helper function per il costruttore)
    void loadFromFile(const std::string& filename);

    //Funzioni getter
    int getSize(char key);
    //Restituisce il numero di passaggi di un dato tipo (key)
    //Lancia un eccezione se key diverso da 'S' o 'V'

    //Convenzione: id parte da 1 e quindi corrisponde a indice+1
    double getDistance(char key, int id);
    //Restituisce la distanza del passaggio numero id del dato tipo (key)
    //Lancia un eccezione se key diverso da 'S' o 'V'
    //Lancia un eccezione se id non e' presente

    double getDistanceBetween(char key, int id1, int id2);
    //Restituisce la distanza tra il passaggio numero id1 e numero id2 del dato tipo (key)
    //Lancia un eccezione se key diverso da 'S' o 'V'
    //Lancia un eccezione se id1 e id2 non sono presenti

private:
    //Variabili private

    std::unordered_map<char, std::vector<double>> nodes;


    //Helper functions

    bool isDouble(const std::string& s);
    //Restituisce true se una stringa e' convertibile in double, false altrimenti

    bool isValidKey(char key);
    //Restituisce true se la chiave è valida ('S' o 'V'), false altrimenti

    bool isValidId(int id, std::vector<double> v);
    //Restituisce true se id esiste, false altrimenti
};

#endif















































