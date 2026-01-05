Di seguito sono scritti i dettagli di alcune scelte implementative.

Funzione processData in DataProcessor:
Nella classe DataProcessor, è stato scelto di processare i dati richiesti da set_time e stats ancora prima che le corrispettive funzioni vengano chiamate, 
processando tutto una sola volta all'inizio e salvando il risultato. Questo viene fatto attraverso la funzione processData(), che viene infatti chiamata
direttamente alla creazione dell'oggetto (tramite il costruttore). 
Questa scelta è stata fatta per semplificare sia l'algoritmo di riempimento dei dati in processData() (un unico ciclo per riempire
sia i dati di stats sia i dati di set_time), sia le funzioni set_time() e stats() per il ritorno dei dati (essendo i dati già pronti). 
Inoltre, avendo salvato i dati fin dall'inizio, digitare un gran numero di comandi non ha grande impatto nelle prestazioni, 
in quanto alla digitazione di un comando i dati vengono solo letti.

Uso di unordered_map:
Nel codice viene spesso fatto uso delle map come struttura dati. Questa scelta è stata fatta per ottimizzare al massimo tutti gli algoritmi, 
semplificando notevolmente inserimento e ricerca di dati. Questo perchè le mappe ci hanno permesso di scorrere tutti i dati senza la preocupazione di controllare i duplicati, 
per esempio per scorrere tutti i varchi percorsi da un singolo veicolo, usando la targa del veicolo come chiave.

Uso di unique_ptr in Tutor:
Nel Tutor viene usato un oggetto di tipo unique_ptr, questa scelta è dovuta al blocco try-catch: non potendo dichiarare l'oggetto DataProcessor dentro al try-catch, 
perchè una volta uscito non sarebbe stato più visibile, è stato scelto di dichiarare un puntatore all'esterno e creare l'oggetto puntato nel try-catch. 
Un'alternativa sarebbe stata dichiarare l'oggetto all'esterno, creare poi un nuovo oggetto temporaneo dentro il try-catch e usare l'operatore di assegnamento per copiare 
tutti i dati dell'oggetto temporaneo nell'oggetto esterno, ma questo richiederebbe ulteriore tempo per la copia di centinaia di migliaia di dati, poco efficiente.
Nello specifico, come puntatore è stato usato proprio un tipo "unique_ptr" in quanto esso si occupa anche della deallocazione della memoria.
