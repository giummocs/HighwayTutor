#include "Highway.h"
#include <iostream>

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


    std::vector<HighwayNode> svincoli = highway.getJunctions();
    std::vector<HighwayNode> varchi = highway.getGates();
    
    for (int i = 0; i < svincoli.size(); i++){
        double dist = svincoli[i].distance;
        int next = svincoli[i].next;
        int prev = svincoli[i].prev;
        std::cout << "*****************************************************************\n";
        std::cout << "Svincolo " << i+1 << " distanza : " << dist << "km\n";
        std::cout << "Varco precedente : " << prev+1 << " successivo : " << next+1 << "\n";
    }
    std::cout << "\n##############################################################\n\n";
    for (int i = 0; i < varchi.size(); i++){
        double dist = varchi[i].distance;
        int next = varchi[i].next;
        int prev = varchi[i].prev;
        std::cout << "*****************************************************************\n";
        std::cout << "Varco " << i+1 << " distanza : " << dist << "km\n";
        std::cout << "Svincolo precedente : " << prev+1 << " successivo : " << next+1 << "\n";
    }

    std::cout << "//////////////////////////////////////////////////////////////\n\n";
    std::cout << "=================== Trovare la distanza dei passaggi e i numeri suoi passaggi adiacenti ===================\n\n";


    char key;
    int index;
    std::cout << "Inserisci la chiave (S per svincoli o V per varchi)" << "\n";
    std::cin >> key;
    std::cout << "Inserisci il numero del passaggio" << "\n";
    std::cin >> index;
    std::cout << "Il passaggio " << key << " numero "<< index << " ha : \n";
    // secondo la convenzione del testo la numerazione dei passaggi cominciano da 1 e quindi l'indice del vettore sara' (numero - 1)
    index--;

    double dist = highway.getDistance(key, index);
    int prev = highway.getPrev(key, index);
    int next = highway.getNext(key, index);
    
    std::cout << "distanza " << dist << "km | varco precendente " << prev+1 << " successivo " << next+1 << "\n";

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

    std::cout << "Distanza tra passaggi " << key << " numero " << index1 << " e numero " << index2 << " : " << dist << "km\n";

    std::cout << "//////////////////////////////////////////////////////////////\n\n\n";
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::cout << "=================== Sovrascrittura di Highway ===================\n\n";

    std::cout << "Inserisci nome file da importare e sovrascrive quello vecchio\n";
    std::cin >> filename;

    highway.loadFromFile(filename);

    std::cout << "//////////////////////////////////////////////////////////////\n\n";
    std::cout << "=================== Stampa a schermo degli svincoli e dei varchi ===================\n\n";


    highway.printJunctions();
    std::cout << "##############################################################\n\n";
    highway.printGates();

    std::cout << "//////////////////////////////////////////////////////////////\n\n";
    std::cout << "=================== Creazione e stampa dei due vettori di svincoli e varchi ===================\n\n";


    svincoli = highway.getJunctions();
    varchi = highway.getGates();
    
    for (int i = 0; i < svincoli.size(); i++){
        double d = svincoli[i].distance;
        int n = svincoli[i].next;
        int p = svincoli[i].prev;
        std::cout << "*****************************************************************\n";
        std::cout << "Svincolo " << i+1 << " distanza : " << d << "km\n";
        std::cout << "Varco precedente : " << p+1 << " successivo : " << n+1 << "\n";
    }
    std::cout << "##############################################################\n\n";
    for (int i = 0; i < varchi.size(); i++){
        double d = varchi[i].distance;
        int n = varchi[i].next;
        int p = varchi[i].prev;
        std::cout << "*****************************************************************\n";
        std::cout << "Varco " << i+1 << " distanza : " << d << "km\n";
        std::cout << "Svincolo precedente : " << p+1 << " successivo : " << n+1 << "\n";
    }

    std::cout << "//////////////////////////////////////////////////////////////\n\n";
    std::cout << "=================== Trovare la distanza dei passaggi e i numeri suoi passaggi adiacenti ===================\n\n";

    std::cout << "Inserisci la chiave (S per svincoli o V per varchi)" << "\n";
    std::cin >> key;
    std::cout << "Inserisci il numero del passaggio" << "\n";
    std::cin >> index;
    std::cout << "Il passaggio " << key << " numero "<< index << " ha : \n";
    // secondo la convenzione del testo la numerazione dei passaggi cominciano da 1 e quindi l'indice del vettore sara' (numero - 1)
    index--;

    dist = highway.getDistance(key, index);
    prev = highway.getPrev(key, index);
    next = highway.getNext(key, index);
    
    std::cout << "distanza " << dist << "km | varco precendente " << prev+1 << " successivo " << next+1 << "\n";

    std::cout << "//////////////////////////////////////////////////////////////\n\n";
    std::cout << "=================== Trovare la distanza tra due passaggi dello stesso tipo ===================\n\n";

    std::cout << "Inserisci la chiave (S per svincoli o V per varchi)" << "\n";
    std::cin >> key;
    std::cout << "Inserisci i numeri del passaggi separati da uno spazio" << "\n";
    std::cin >> index1;
    std::cin >> index2;
    dist = highway.getDistanceBetween(key, index1-1, index2-1);

    std::cout << "Distanza tra passaggi " << key << " numero " << index1 << " e numero " << index2 << " : " << dist << "km\n";

    return 0;
}