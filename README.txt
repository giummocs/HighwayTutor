Da fare:
-controllare le slide del prof del progetto intermedio con "errori da non fare", magari ci sono indicazioni su che grandezza scrivere i main
-mettere piu try catch, soprattutto nelle conversioni di dati
-commentare
-testare con molti file input
-aggiungere eccezioni e controlli aggiuntivi 
-valutare se rimpiazzare tutti i size_t con int, se possibile e se non crea nessun tipo di problema e se e' corretto
-a fine progetto inserire in README alcuni chiarimenti sui motivi di determinate implementazioni, 
 per esempio PERCHE abbiamo usato le mappe, PERCHE abbiamo fatto questo e quello... . Rimuovere queste spiegazioni dai commenti, 
 e lasciare solo commenti che spiegano COSA facciamo, non PERCHE lo facciamo
-modificare algoritmo per trovare il varco successivo di OGNI svincolo, creare un vector<int> v, dove la cella v[3] contiene l'indice del varco successivo allo svincolo 3 (id=3, che corrisponde anche a nodes['S'][3]).
 la funzione che riempie questo v, dovra essere chiamata una sola volta nel costruttore e deve fare:
 for(int i=0; i< nodes['S'].size()-1;i++){ //-1 perche lultimo svincolo non avra varchi successivi
    int j=0;
    do{
       j++;
    }while(j<nodes['V'].size() && nodes['V'][j]<nodes['S'][i]);
    if(j<nodes['V'].size() && nodes['V'][j]>nodes['S'][i]) v[i] = j;
    else v[i] = -1; //non esiste alcun successivo
 }

ATTENZIONE: bisogna stare attenti quando si usa v perche bisogna verificare anche che il varco successivo a uno svincolo sia compreso tra lo svincolo di entrata e uscita! 


Fatto:
-sistemare magic number 10000 nel for
-mettere un comando per uscire dal ciclo while in tutor.cpp
-Le targhe invece iniziano sempre con AA e finiscono sempre con ZZ, non va bene
