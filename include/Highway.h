#ifndef HIGHWAY_H
#define HIGHWAY_H

#include <iostream>
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

    //Funzione per caricare dati da file (helper function per il costruttore)
    void loadFromFile(const std::string& filename);

    //Funzioni getter 

    //Restituisce il vettore con chiave 'S' (svincoli)
    const std::vector<double>& getJunctions();
    //Restituisce il vettore con chiave 'V' (varchi)
    const std::vector<double>& getGates();

    //Funzioni getter che accedono alla data chiave della mappa
    //Lanciano eccezioni se key diverso da 'S' o 'V'
    int getSize(char key);
    double getDistance(char key, int id);
    double getDistanceBetween(char key, int id1, int id2);

    //Funzioni aggiuntive non inerenti al nostro progetto ma potrebbero essere utili in un altro contesto
    void printJunctions();
    void printGates();

private:
    //Variabile privato
    std::unordered_map<char, std::vector<double>> nodes;

    //Helper function per il controllo se una stringa e' convertibile in double
    bool isDouble(const std::string& s);
};

#endif






































