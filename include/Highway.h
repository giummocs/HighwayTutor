#ifndef HIGHWAY_H
#define HIGHWAY_H

#include <vector>
#include <string>

class Highway {
public:
    const int MAX_SVINCOLI;
    const int MAX_VARCHI;

    bool loadFromFile(const std::string& filename);
    std::vector<Point> getVarchiInRange(double startKm, double endKm) const;
    const std::vector<Point>& getSvincoli();
    const std::vector<Point>& getVarchi();

private:
    //Abbiamo scelto di implementare due vettori per differenziare al meglio varchi e svincoli, 
    //il che facilita molto la logica negli altri file: codice piu facile da leggere e alleggerito nella ricerca di varchi e svincoli.
    std::vector<double> svincoli;
    std::vector<double> varchi;

    bool isDouble(const std::string& s);
};


#endif









