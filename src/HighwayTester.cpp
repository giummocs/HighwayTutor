#include "Highway.h"

int main() {
    std::cout << "=================== Tester di Highway ===================\n\n";

    std::string filename;

    std::cout << "Inserisci nome file da importare\n";
    std::cin >> filename;

    Highway highway(filename);

    std::cout << "//////////////////////////////////////////////////////////////\n\n";
    std::cout << "=================== Stampa a schermo degli svincoli e dei varchi ===================\n\n";


    highway.printJunctions();
    std::cout << "##############################################################\n\n";
    highway.printGates();

    std::cout << "//////////////////////////////////////////////////////////////\n\n";
    std::cout << "=================== Creazione e stampa dei due vettori di svincoli e varchi ===================\n\n";


    std::vector<double> svincoli = highway.getJunctions();
    std::vector<double> varchi = highway.getGates();

    std::cout << "*****************************************************************\n\n";
    
    for (int i = 0; i < svincoli.size(); i++){
        double dist = svincoli[i];
        std::cout << "Svincolo " << i+1 << " distanza : " << dist << "km\n\n";
    }
    
    std::cout << "\n##############################################################\n\n";
    std::cout << "*****************************************************************\n\n";
    
    for (int i = 0; i < varchi.size(); i++){
        double dist = varchi[i];
        std::cout << "Varco " << i+1 << " distanza : " << dist << "km\n\n";
    }

    std::cout << "//////////////////////////////////////////////////////////////\n\n";
    std::cout << "=================== Numero di svincoli e varchi ===================\n\n";

    char key;
    
    std::cout << "Inserisci la chiave (S per svincoli o V per varchi) di cui vuoi sapere il numero\n";

    std::cin >> key;
    int n = highway.getSize(key);

    std::cout << "Numero di passaggi " << key << " : " << n << "\n\n";
    
    std::cout << "//////////////////////////////////////////////////////////////\n\n";
    std::cout << "=================== Trovare la distanza dei passaggi ===================\n\n";

    int index;
    std::cout << "Inserisci la chiave (S per svincoli o V per varchi)" << "\n";
    std::cin >> key;
    std::cout << "Inserisci il numero del passaggio" << "\n";
    std::cin >> index;
    std::cout << "\nIl passaggio " << key << " numero "<< index << " ha : \n";
    // secondo la convenzione del testo la numerazione dei passaggi cominciano da 1 e quindi l'indice del vettore sara' (numero - 1)
    index--;

    double dist = highway.getDistance(key, index);
    
    std::cout << "distanza " << dist << "km \n";

    std::cout << "//////////////////////////////////////////////////////////////\n\n";
    std::cout << "=================== Trovare la distanza tra due passaggi dello stesso tipo ===================\n\n";

    int index1;
    int index2;
    std::cout << "Inserisci la chiave (S per svincoli o V per varchi)" << "\n";
    std::cin >> key;
    std::cout << "Inserisci i numeri del passaggi separati da uno spazio" << "\n";
    std::cin >> index1;
    std::cin >> index2;
    dist = highway.getDistanceBetween(key, index1-1, index2-1);

    std::cout << "\nDistanza tra passaggi " << key << " numero " << index1 << " e numero " << index2 << " : " << dist << "km\n";
    
    return 0;

}
