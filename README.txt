Da fare:
-sistemare la generazione casuale dei dati in simulator (le targhe e i valori dell'istante di passaggio non sono molto diversi tra loro non so perchè, tipo tutti i double finiscono con .69 o .31 o .46, sempre con questi decimali. 
 Le targhe invece iniziano sempre con AA e finiscono sempre con ZZ, non va bene)
-dare una controllata a come funziona la questione del tempo, controllare se va bene come la gestisce gemini (tramite variabile double) e se va bene che la incrementi ogni volta di 0.5
-nel file Runs.txt generato da simulator manca la parte "<v0 t0>, <v1 t1>, ..., <vN-1 tN-1>" (vedi specifiche del file Runs.txt nella consegna per capire)
-i comandi nel sistema tutor sono completamente sbagliati, gemini non ha capito niente, "set_time" "reset" e "stats" sono sbagliati. 
 Magari farei dei metodi fuori dal main chiamandoli set_time() reset() e stats() che svolgono le richieste della consegna, e dentro al ciclo while nel main li andiamo a chiamare quando l'utente digita il comando corrispondente.


-commentare
-testare con molti file input
-aggiungere eccezioni e controlli aggiuntivi (per esempio IOException per la lettura e scrittura da file)



Fatto:
-sistemare magic number 10000 nel for
-mettere un comando per uscire dal ciclo while in tutor.cpp
