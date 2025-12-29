#ifndef HIGHWAY_H
#define HIGHWAY_H

#include <vector>
#include <string>

struct VarchiAdiacenti {
    int prev;
    int next;
}

class Highway {
public:
    const int MAX_SVINCOLI;
    const int MAX_VARCHI;

    bool loadFromFile(const std::string& filename);
    const std::vector<double>& getSvincoli();
    const std::vector<double>& getVarchi();
    const std::vector<VarchiAdiacenti>& getVarchiAdiacenti();

private:
    //Abbiamo scelto di implementare due vettori per differenziare al meglio varchi e svincoli, al posto di una struttura dati univoca per entrambi (struct).
    //Il che facilita molto la logica negli altri file: codice piu facile da leggere e alleggerito nella ricerca di varchi e svincoli.
    std::vector<double> svincoli;
    std::vector<double> varchi;
    std::vector<VarchiAdiacenti> varchiAdiacenti;

    bool isDouble(const std::string& s);
};


#endif















