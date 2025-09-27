# Simple CLI
Simple CLI è una soluzione semplice per l'utilizzo di interfacce CLI senza la fatica di doverne creare di nuove.
Per ora esistono 3 tipi di interfacce:
## Schermata
Questa interfaccia è la più basilare, ma anche la più potente: qualsiasi sia il contenuto, se generata in maniera standard apparirà nel seguente formato
<pre><code>
----------------
| Hello World! |
-----------------
</code></pre>
Ci sono 3 tipi di funzioni:
- costruttori
  - <code>Schermata(vector<string> contenuto)</code>: Questa funzione inizializza una schermata il cui contenuto sia composto dalle stringhe, che però non devono contenere '\n'
  - <code>Schermata(string contenuto)</code>: Questa funzione è equivalente alla precedente, ma accetta una sola stringa contente '\n' come separatore tra righe
  - <code>Schermata()</code>: Questa funzione inizializza una schermata totalmente vuota
- aggiornamenti
  - <code>aggiorna(vector<string> contenuto)</code>: Questa funzione aggiorna il contenuto secondo il metodo di Schermata(vector<string> contenuto)
  - <code>aggiorna(string contenuto)</code>: Questa funzione aggiorna il contenuto secondo il metodo di Schermata(string contenuto)
- <code>print(bool del)</code>: Questa funzione stampa il contenuto al terminale, se del è vero cancella lo schermo
## Schermata selettore
Questa interfaccia è molto utile per selezionare opzioni, in quanto permette di navigare mediante le frecce (freccia su - freccia giù), ma anche con <code>w</code> e <code>s</code>, il formato è in ogni caso il seguente:
<pre><code>
  Titolo:
  > Opzione 1
  - Opzione 2
  - Opzione 3
</code></pre>
Ci sono due funzioni:
- <code>SchermataSelettore(string  titolo, const vector<string> opzioni)</code>: Questa funzione permette la creazione di una schermata selettore con titolo ed opzioni
- <code>int render()</code>: Questa funzione renderizza automaticamente la schermata finchè non è stata scelta un'opzione, quando sarà premuto invio sarà restituito l'indice dell'elemento selezionato nel vettore iniziale
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
- <code>SchermataSelettoreCustom(string titolo, vector<char> titoliOpzioni, vector<string> opzioni, bool autoRender)</code>: Questa funzione crea una schermata custom con i dati concessi
- <code>render()</code>: Questa funzione renderizza la schermata in maniera statica, poi rimane in attesa di un input
- <code>getResult()</code>: Questa funzione restituisce l'input
## Schermata selettore grande
Questa interfaccia, basata su Schermata selettore, permette l'aggiunta di liste più lunghe, aggiungendo la funzionalità di scroll, il formato è:
<pre><code>
Titolo:
[...]
- Opzione 4
> Opzione 5
- Opzione 6
- Opzione 7
- Opzione 8
- Opzione 9
- Opzione 10
- Opzione 11
- Opzione 12
- Opzione 13
[...]  
</code></pre>
Ci sono due variabili, ossia moreUp e moreDown, che permettono di customizzare i tags <code>[...]</code> che si trovano all'inizio ed alla fine della lista per mostrare che ci sono altre opzioni.
Ci sono 2 funzioni:
- <code>SchermataSelettoreLarge(string titolo, vector<string> opzioni, int size)</code>: Questa funzione crea una schermata selettore grande con i dati concessi, la lunghezza massima di una lista prima di far comparire i tag <code>moreUp</code> e <code>moreDown</code> è size
- <code>int render()</code>: Questa funzione renderizza automaticamente la schermata finchè non è stata scelta un'opzione, quando sarà premuto invio sarà restituito l'indice dell'elemento selezionato nel vettore iniziale
