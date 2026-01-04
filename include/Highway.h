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
    const std::vector<double>& getJunctions();
    //Restituisce il vettore con chiave 'S' (svincoli)
    
    const std::vector<double>& getGates();
    //Restituisce il vettore con chiave 'V' (varchi)

    int getSize(char key);
    //Restituisce il numero di passaggi di un dato tipo (key)
    //Lancia un eccezione se key diverso da 'S' o 'V'

    double getDistance(char key, int id);
    //Restituisce la distanza del passaggio numero id del dato tipo (key)
    //Lancia un eccezione se key diverso da 'S' o 'V'
    //Lancia un eccezione se id non e' presente

    double getDistanceBetween(char key, int id1, int id2);
    //Restituisce la distanza tra il passaggio numero id1 e numero id2 del dato tipo (key)
    //Lancia un eccezione se key diverso da 'S' o 'V'
    //Lancia un eccezione se id1 e id2 non sono presenti

    //Funzioni aggiuntive non inerenti al nostro progetto ma potrebbero essere utili in un altro contesto
    void printJunctions();
    //Stampa a schermo la lista degli svincoli con la propria distanza

    void printGates();
    //Stampa a schermo la lista dei varchi con la propria distanza

private:
    //Variabile privato
    std::unordered_map<char, std::vector<double>> nodes;

    //Helper functions
    bool isDouble(const std::string& s);
    //Restituisce true se una stringa e' convertibile in double, false altrimenti
};

#endif







































