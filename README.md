# Simple CLI
Simple CLI è una soluzione semplice per l'utilizzo di interfacce CLI senza la fatica di doverne creare di nuove.
Per ora esistono 3 tipi di interfacce:

## Lista delle schermate

### Semplici

##### Schermata base `schermate::Schermata`:
```text
----------------
| Hello World! |
-----------------
```

##### Schermata scorrevole `in arrivo`:
```text
---------------
| Intestazione|
|             |
| Riga1       |
| Riga2       |
| Riga3       |
| Riga4       |
| Riga5       |
| Riga6       |
| v           |
---------------
```

### Selettori
##### Selettore semplice `schermate::SchermataSelettore`:
```text
Titolo:
> Opzione 1
- Opzione 2
- Opzione 3
```

##### Selettore scorrevole `schermate::SchermataSelettoreLarge`:
```text
Titolo:
^
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
v
```

##### Selettore filtrato `schermate::SchermataSelettoreFiltrata`:
```text
Titolo:
filtro
- opzioneFiltro1
- opzioneFiltro2
> opzioneFiltro3
- opzioneFiltro4
- filtro5
- filtro 6
- Questo è un filtro
- filtro1
v
```

##### Selettore custom `schermate::SchermataSelettoreCustom`:
```text
Titolo:
1: Opzione 1
2: Opzione 2
a: Opzione a
x: Opzione x
```

### Questionari

##### Schermata questionario `schermate::SchermataQuestionario`:
```text
Titolo:
> Questo è un selettore booleano: [X]

- Questo è un altro selettore booleano: [ ]

- Questo è un selettore di scala, 
pti: 12345
    [-----]
   
- Questa è una casella di inserimento di testo: Hello World!
```

## Funzioni delle schermate

### Semplici
- `costruttori`: creano l'oggetto circondato da bordi, i costruttori standard (senza argomenti) non aggiungono i bordi
- `aggiorna`: aggiorna il contenuto, permette di scegliere se avere o meno i bordi
- `print`: stampa il contenuto a schermo
- `setContenuto`: aggiorna il contenuto senza aggiungere i bordi
- `getContenuto`: restituisce il contenuto così come viene stampato a schermo

Si può trovare inoltre una variabile pubblica chiamata `printables` di tipo unordered_map, che mappa i vari tipi di oggetti della schermata a singoli caratteri

### Selettori

#### Funzioni comuni:
- `costruttori`: creano un selettore senza renderizzarlo, fa eccezione il selettore custom, in cui si può scegliere se renderizzare o meno
- `render`: renderizza automaticamente la schermata, restituendo l'intero corrispondente all'indice dell'opzione selezionata nel vettore che è stato dato al costruttore.
La funzione permette di scegliere se si desidera aggiungere un bordo alla schermata

#### Funzioni specifiche:
Selettore custom
- `getResult`: restituisce il carattere associato al risultato

#### Meccaniche speciali:
- La schermata questionario ha un funzionamento particolare: a rappresentare
le varie opzioni ci sono oggetti chiamati `formElement`, che contengono tutti
i dati che possono servire, nel formato
  - `titolo`: l'intestazione dell'opzione
  - `questionType`: accetta un intero o un `questionTypes`, e rappresenta
il tipo di elemento
  - `size`: serve solo se questionType è `SCALA` e dichiara la dimensione
dell'elemento
  - `outInt`: accetta una variabile `int` e la modifica: per i booleani sarà
0 oppure 1, e per le scale rappresenterà l'indice (a partire da 0)
dell'elemento selezionato
  - `outString`: accetta una variabile `string` e la modifica: essa assumerà
il valore in ingresso di un elemento di inserimento

## Installazione
Il pacchetto si può trovare su https://www.nuget.org/packages/Simple_CLI.
Installazione con .NET:
``` DOTNET
dotnet add package Simple_CLI --version 1.5.0
```
Installazione con PMC
``` PMC
NuGet\Install-Package Simple_CLI -Version 1.5.0
```