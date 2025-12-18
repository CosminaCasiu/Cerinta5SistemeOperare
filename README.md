
1. Introducere Obiectivul a fost calcularea numerelor prime până la 10.000 folosind 10 procese separate care comunică rezultatele printr-un mecanism de tip Pipe.

2. Comparație între Sisteme de Operare

Linux (POSIX): Experiența a fost mai intuitivă datorită funcției fork(), care copiază spațiul de adrese. Pipe-urile sunt tratate ca descriptori de fișiere simpli.

Windows: A reprezentat o provocare mai mare din cauza managementului riguros al "Handles" și necesității de a specifica explicit dacă un handle este moștenit (bInheritHandle = TRUE).

3. Dificultăți Întâmpinate

Sincronizarea citirii: Asigurarea că părintele nu închide pipe-ul înainte ca toți copiii să termine.

Gestionarea memoriei și a resurselor (închiderea corectă a descriptorilor).

4. Concluzii Utilizarea proceselor multiple scade timpul de execuție pe sistemele multi-core, însă overhead-ul creat de IPC (Inter-Process Communication) trebuie luat în calcul pentru intervale mici de numere.
