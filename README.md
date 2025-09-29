# Simple CLI
Simple CLI è una soluzione semplice per l'utilizzo di interfacce CLI senza la fatica di doverne creare di nuove.
Per ora esistono 3 tipi di interfacce:

## Lista delle schermate

### Semplici

##### Schermata base (schermate::Schermata):
```text
----------------
| Hello World! |
-----------------
```

##### Schermata scorrevole (in arrivo):
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
##### Selettore semplice (schermate::SchermataSelettore):
```text
Titolo:
> Opzione 1
- Opzione 2
- Opzione 3
```

##### Selettore scorrevole (schermate::SchermataSelettoreLarge):
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

##### Selettore filtrato (schermate::SchermataSelettoreFiltrata):
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

##### Selettore custom (schermate::SchermataSelettoreCustom):
```text
Titolo:
1: Opzione 1
2: Opzione 2
a: Opzione a
x: Opzione x
```

## Funzioni delle schermate

### Semplici
- `costruttori`: creano l'oggetto circondato da bordi, i costruttori standard (senza argomenti) non aggiungono i bordi
- `aggiorna`: aggiorna il contenuto aggiungendo i bordi
- `print`: stampa il contenuto a schermo
- `setContenuto`: aggiorna il contenuto senza aggiungere i bordi
- `getContenuto`: restituisce il contenuto così come viene stampato a schermo

Si può trovare inoltre una variabile pubblica chiamata `printables` di tipo unordered_map, che mappa i vari tipi di oggetti della schermata a singoli caratteri

### Selettori

#### Funzioni comuni:
- `costruttori`: creano un selettore senza renderizzarlo, fa eccezione il selettore custom, in cui si può scegliere se renderizzare o meno
- `render`: renderizza automaticamente la schermata, restituendo l'intero corrispondente all'indice dell'opzione selezionata nel vettore che è stato dato al costruttore

#### Funzioni specifiche:
Selettore custom
- `getResult`: restituisce il carattere associato al risultato

## Installazione
Il pacchetto si può trovare su https://www.nuget.org/packages/Simple_CLI#readme-body-tab.
Installazione con .NET:
``` DOTNET
dotnet add package Simple_CLI --version 1.4.2
```
Installazione con PMC
``` PMC
NuGet\Install-Package Simple_CLI -Version 1.4.2
```