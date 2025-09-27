# Simple CLI
Simple CLI è una soluzione semplice per l'utilizzo di interfacce CLI senza la fatica di doverne creare di nuove.
Per ora esistono 3 tipi di interfacce:
## Schermata
Questa interfaccia è la più basilare, ma anche la più potente: qualsiasi sia il contenuto, se generata in maniera standard apparirà nel seguente formato
<pre><code>
-----------------
| Hello World! |
-----------------
</code></pre>
Ci sono 3 tipi di funzioni:
- costruttori
  - Schermata(vector<string> contenuto): Questa funzione inizializza una schermata il cui contenuto sia composto dalle stringhe, che però non devono contenere '\n'
  - Schermata(string contenuto): Questa funzione è equivalente alla precedente, ma accetta una sola stringa contente '\n' come separatore tra righe
  - Schermata(): Questa funzione inizializza una schermata totalmente vuota
- aggiornamenti
  - aggiorna(vector<string> contenuto): Questa funzione aggiorna il contenuto secondo il metodo di Schermata(vector<string> contenuto)
  - aggiorna(string contenuto): Questa funzione aggiorna il contenuto secondo il metodo di Schermata(string contenuto)
- print(bool del): Questa funzione stampa il contenuto al terminale, se del è vero cancella lo schermo
## Schermata selettore
Questa interfaccia è molto utile per selezionare opzioni, in quanto permette di navigare mediante le frecce (freccia su - freccia giù), ma anche con 'w' e 's', il formato è in ogni caso il seguente:
<pre><code>
  Titolo:
  > Opzione 1
  - Opzione 2
  - Opzione 3
</code></pre>
Ci sono due funzioni:
- SchermataSelettore(string  titolo, const vector<string> opzioni): Questa funzione permette la creazione di una schermata selettore con titolo ed opzioni
- int render(): Questa funzione renderizza automaticamente la schermata finchè non è stata scelta un'opzione, quando sarà premuto invio sarà restituito l'indice di render nel vettore iniziale
## Schermata selettore custom
Questa interfaccia, simile a Schermata selettore, varia da essa per una sola caratteristica: si basa sull'inserimento diretto dell'indice anzichè selezionare manualmente l'opzione, il formato è:
<pre><code>
Titolo:
1: Opzione 1
2: Opzione 2
a: Opzione a
x: Opzione x
</code></pre>
Ci sono tre funzioni:
- SchermataSelettoreCustom(string titolo, vector<char> titoliOpzioni, vector<string> opzioni, bool autoRender): Questa funzione crea una schermata custom con i dati concessi
- render(): Questa funzione renderizza la schermata in maniera statica, poi rimane in attesa di un input
- getResult(): Questa funzione restituisce l'input
