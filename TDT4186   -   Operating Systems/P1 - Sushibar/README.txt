TDT4186 - Operativsystemer
Vår 2019
Praktisk øving P1

Niklas Sølvberg

__________________________________________________________________________________


Implementasjonen av løsningen min er som følger.

Døren (Door) skal slippe inn nye kunder (Customer) med jevne mellomrom.
Når døren ønsker å slippe inn en ny kunde kalles metoden enter(Customer).
Denne metoden er synchronized, som gjør at kun en tråd kan kjøre kodesnutten
av gangen. Dersom venterommet (WaitingRoom) er fullt, kalles wait() inne i en
while-løkke i enter(Customer), slik at det blir kalt notifyAll() når det er plass,
slik at kunden blir lagt til i venterommet så snart det blir ledig plass.

Venterommet har en metode som blir brukt for å bestemme hvilken kunde som
er den neste til å bli hentet av en servitrise (Waitress). Denne kalles next(),
og returnerer det første elementet (en Customer) i venterommets kø-objekt (Queue).

En servitrise-tråd ser etter den neste kunden så lenge sushibaren er åpen og det
er kunder i venterommet. Dersom en servitrise er ledig vil denne tråden hente ut
den neste kunden, og vil være opptatt med denne kunden helt til kunden har forlatt
sushibaren. Fra servitrisen henter kunden til kunden får spise, simuleres denne
ventetiden ved å kalle Thread.sleep(long), hvor argumentet er forhåndsbestemt.
Deretter kalles order()-metoden på kunde-objektet. Denne metoden bruker
ThreadLocalRandom (en måte å generere tilfeldige tall lokalt for en tråd) for å
bestemme hvor mange bestillinger kunden vil spise i baren, og hvor mange
bestillinger kunden vil ta med hjem. Inne i order() kalles addOrders(int,int) som
oppdaterer de globale variablene som teller hvor mange bestillinger som er gjort.
Deretter kalles Thread.sleep(long) igjen, men denne gangen er argumentet en
forhåndsbestemt variabel ganget med antall bestillinger denne kunden skal spise i
baren. Det blir logget hver gang en kunde blir hentet, bestiller, og forlater
sushibaren.

Klokke- og heltalls-objektene (Clock, SynchronizedInteger) er ikke endret.

Sushibar-klassen (SushiBar) inneholder alle globale variabler, en main(String[]),
en write(String)-metode (for å logge underveis), samt en metode kalt lineBreak()
som kun er med for syns skyld angående loggingen. I main(String[]) opprettes
nye objekter (WaitingArea, ArrayList<Waitress>, Thread (døren)). Det siste jeg har
implementert er å kalle join()-metoden på alle tråder, det vil si på døren og alle
servitrisene. Dette er for å forhindre at programmet kjører videre til printingen
av statistikken før trådene er ferdige. Statistikken skal printes til slutt, da
noe annet ville gitt lite mening, så derfor må vi bruke join()s.

__________________________________________________________________________________


wait(), notify(), og notifyAll()

wait()-metoden sørger for at den gjeldende tråden venter med videre utførelse til
notify() eller notifyAll() blir kalt. wait() får en tråd til å vente, notify() og
notifyAll() brukes for å vekke tråder. notifyAll() vekker alle tråder som venter
på objektets monitor (herfra er det førstemann til mølla), mens notify() kun
vekker en tilfeldig tråd som venter. I de aller fleste situasjoner vil det ikke
være noen grunn til å bruke notify() over notifylAll().

__________________________________________________________________________________


Delte variabler

Variabler som blir delt på tvers av trådene er customerCounter, servedOrders,
takeawayOrders, totalOrders, og waitingArea. customerCounter blir brukt til å
generere nye ID-er til nye kunder, og increment() blir kalt hver gang en ny bruker
får tilegnet seg en ID. servedOrders, takeawayOrders, og totalOrders håndterer
antall bestillinger gjort på tvers av tråder, og metoden add(int) blir kalt når en
kunde har lagt til en bestilling.

__________________________________________________________________________________


Statistikk

main-metoden vil håndtere printing og logging av statistikk (ved å kalle
write(String)), og tidspunktet for når dette skal gjøres blir bestemt av når alle
servitrise-trådene og dør-tråden er ferdige å kjøre. Dette blir gjort ved hjelp av
join()s.