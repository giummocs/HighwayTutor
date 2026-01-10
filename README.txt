Di seguito sono scritti i dettagli di alcune scelte implementative.

Funzione processData (DataProcessor.cpp):
Nella classe DataProcessor, è stato scelto di processare i dati richiesti da set_time e stats ancora prima che le corrispettive funzioni vengano chiamate, 
processando tutto una sola volta all'inizio e salvando il risultato. Questo viene fatto attraverso la funzione processData(), che viene infatti chiamata
direttamente alla creazione dell'oggetto (tramite il costruttore). 
Grazie a questa scelta, in un unico ciclo (O(n) dove n numero di righe di Passages.txt) vengono riempiti sia i sia i dati di stats sia i dati di set_time.
Le corrispettive funzioni poi dovranno solo leggere i dati e selezionare solo quelli che rientrano nel tempo inserito in set_time.
Quindi digitare un gran numero di comandi non ha grande impatto nelle prestazioni, in quanto alla digitazione di un comando vengono letti solo i dati interessati.

Uso di unordered_map (DataProcessor.cpp):
Nel codice viene spesso fatto uso delle map come struttura dati. Questa scelta è stata fatta per ottimizzare al massimo tutti gli algoritmi, 
semplificando notevolmente inserimento e ricerca di dati. Questo perchè le mappe ci hanno permesso di scorrere tutti i dati senza la preocupazione di controllare i duplicati, 
per esempio per scorrere tutti i varchi percorsi da un singolo veicolo, usando la targa del veicolo come chiave.

Uso di unique_ptr (Tutor.cpp):
Nel Tutor viene usato un oggetto di tipo unique_ptr, questa scelta è dovuta al blocco try-catch: non potendo dichiarare l'oggetto DataProcessor dentro al try-catch, 
perchè una volta uscito non sarebbe stato più visibile, è stato scelto di dichiarare un puntatore all'esterno e creare l'oggetto puntato nel try-catch. 
Un'alternativa sarebbe stata dichiarare l'oggetto all'esterno, creare poi un nuovo oggetto temporaneo dentro il try-catch e usare l'operatore di assegnamento per copiare 
tutti i dati dell'oggetto temporaneo nell'oggetto esterno, ma questo richiederebbe ulteriore tempo per la copia di centinaia di migliaia di dati, poco efficiente.
Nello specifico, come puntatore è stato usato proprio un tipo "unique_ptr" in quanto esso si occupa anche della deallocazione della memoria.

Generazione di Runs.txt:
Nella consegna inizialmente viene scritto di inserire nel file Runs.txt anche "data e ora di entrata". Tuttavia poi quando viene illustrato il pattern
che deve avere il file c'è scritto "<istante di partenza>", quindi abbiamo adottato la convenzione di inserire l'istante di partenza in secondi, non data e ora.

Comando stats (DataProcessor.cpp):
Non ci è molto chiaro se nel comando stats bisogni stampare le statistiche dall'istante 0 a quello corrente, o di tutta l'autostrada.
Quindi è stata adottata la convenzione di stampare tutte le statistiche indipendentemente dall'istante corrente, non essendoci una voce nella consegna che
specifichi questo punto.

Inserimento file Highway da riga di comando:
Principalmente per facilitare i test, è stato scelto di dare la possibilità di scegliere il nome del file Highway da selezionare, tramite i parametri della funzione main.
In caso non venga inserito alcun parametro, di default viene selezionato il file con nome "Highway.txt". 

Variabile "passages" in processData() (DataProcessor.cpp):
La mappa "passages" è una variabile temporanea, usata per organizzare tutte le righe di passages.txt per numero di targa, così da facilitare i successivi algoritmi per 
popolare violations e statistics (per esempio in violations serve ottenere tutti i varchi che una singola macchina ha attraversato, facile da fare con la mappa).
Dichiararla in DataProcessor.h sarebbe stato un enorme spreco di memoria in quanto tale variabile, dopo il popolamento dei dati, non viene piu usata.
