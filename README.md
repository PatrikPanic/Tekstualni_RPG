# Tekstualni RPG

[![Build i testovi](https://github.com/PatrikPanic/Tekstualni_RPG/actions/workflows/build-and-test.yml/badge.svg)](https://github.com/PatrikPanic/Tekstualni_RPG/actions/workflows/build-and-test.yml)

Seminarski rad (OOP projekt) – konzolna tekstualna RPG igra u kojoj se igrač kreće
po mapi povezanih lokacija, bori se s protivnicima, skuplja opremu i probija se do
završnog protivnika.

Igrač je umirovljeni borac kojemu su oteli suprugu i odveli je u Voidspire Cave.
Cilj je doći do te lokacije i poraziti Voidspire Overlorda, a mjera uspješnosti je
broj osvojenih bodova.

## Funkcionalnost

- likovi izvedeni iz zajedničke apstraktne osnove: igrač i protivnik dijele
  bodove života, napad, obranu, brzinu i ruksak, a razliku čini oprema
- borba po sustavu brojača ovisnog o brzini, u kojoj brži lik može odigrati više
  poteza zaredom; u svom potezu igrač bira napad, obranu ili bijeg
- hijerarhija predmeta s oklopom i oružjem, četiri stupnja rijetkosti te mjesta
  na kojima se predmeti nose (kaciga, prsni oklop, rukavice, hlače, čizme;
  lijeva i desna ruka)
- predlošci predmeta učitavaju se iz vanjskih tekstualnih datoteka pomoću
  predloška klase, pa se novi predmeti dodaju bez izmjene koda
- inventar s ruksakom ograničenog kapaciteta i odvojenim spremnicima za
  opremljeni oklop i oružje; ukupne statistike lika preračunavaju se pri svakoj
  promjeni opreme
- mapa od 15 lokacija izvedena kao graf; igraču se nude samo lokacije dostupne
  iz trenutne, a težina lokacije utječe na vjerojatnost susreta i jačinu protivnika
- protivnici prilagođeni lokaciji, uz dva srednja bossa koji čuvaju prolaz i
  završnog bossa koji zaključuje igru
- pregled ruksaka poraženog protivnika i uzimanje njegove opreme, uz mogućnost
  bacanja vlastitih predmeta kad je ruksak pun
- odmor u gradu koji vraća sve bodove života, ali troši jedan dan
- bodovanje ovisno o razini protivnika i težini lokacije, umanjeno za svaki
  provedeni dan, te lista deset najboljih rezultata spremljena u datoteku
- vlastita hijerarhija iznimaka za greške pri učitavanju datoteka i punom ruksaku
- 94 jedinična testa koji se pokreću automatski pri svakoj izmjeni

## Zahtjevi

- Windows 10 ili noviji, 64-bitni
- Visual Studio 2022 s radnim opterećenjem *Desktop development with C++*
- za pokretanje testova i komponenta *C++ unit test framework*

Igra koristi Windows konzolni API (`conio.h`, `_getch`, `system("cls")`) za
upravljanje strelicama i osvježavanje ekrana, pa se ne prevodi na drugim
operacijskim sustavima. Standard je C++17.

## Prevođenje

1. Otvoriti `Tekstualni_RPG.sln` u razvojnom okruženju Visual Studio 2022.
2. Odabrati konfiguraciju `Debug` ili `Release`; platforma je `x64`.
3. Pokrenuti *Build → Build Solution*.

Datoteke `armor.txt`, `weapon.txt`, `junk.txt` i `Enemy` moraju se nalaziti u
radnom direktoriju programa, jer se iz njih učitavaju predlošci predmeta i imena
protivnika. Ako neka nedostaje, program ispisuje poruku o grešci i uredno se
zatvara umjesto da se sruši.

## Korištenje

Igra se u potpunosti upravlja tipkovnicom:

| Tipka                 | Radnja                          |
| --------------------- | ------------------------------- |
| Strelica gore / dolje | pomicanje po izborniku          |
| Enter                 | potvrda odabrane stavke         |
| Bilo koja tipka       | zatvaranje poruke i nastavak    |

Odabrana stavka označena je znakom `>`. Popis je kružan, pa strelica dolje s
posljednje stavke vraća na prvu.

### Glavni izbornik

- **Start game** – unos imena, uvodna priča i početak nove partije
- **Highscore** – lista deset najboljih rezultata
- **Exit** – izlaz iz programa

### Izbornik igre

Iznad izbornika prikazuju se dan, bodovi života, trenutni rezultat i kazna
prikupljena odmaranjem, a ispod toga mapa svijeta s označenom trenutnom lokacijom.

- **Travel** – putovanje na susjednu lokaciju
- **Inventory** – otvaranje inventara
- **Stats** – prikaz statistika lika, osnovnih i uvećanih opremom
- **Attack** – namjerno traženje borbe na trenutnoj lokaciji
- **Rest** – odmor koji vraća sve bodove života i troši jedan dan
  (stavka se pojavljuje samo dok je igrač u gradu)
- **Back** – povratak u glavni izbornik

**Travel** otvara popis lokacija dostupnih iz trenutne, uz oznaku težine svake
od njih. Dolaskom na novu lokaciju može doći do susreta s protivnikom, a
vjerojatnost raste s težinom lokacije.

### Inventar

Odabirom spremnika (**Backpack**, **Armor** ili **Weapon**) prikazuje se njegov
sadržaj, dok su preostala dva vidljiva u zaglavlju. Nad pojedinim predmetom
dostupne su radnje **Equip**, **Unequip**, **Drop** i **See item stats**.

Za opremanje predmeta u ruksaku mora biti barem jedno slobodno mjesto, jer
predmet koji je već bio u tom slotu odlazi natrag u ruksak.

### Borba

Zaglavlje prikazuje bodove života igrača te razinu i bodove života protivnika.

- **Attack** – šteta je ukupni napad umanjen za protivnikovu obranu
- **Defend** – umanjuje štetu sljedećeg protivničkog napada
- **Run** – pokušaj bijega iz borbe, bez osvojenih bodova

Redoslijed poteza ovisi o brzini, pa brži lik može odigrati dva poteza zaredom.
Nakon pobjede otvara se ruksak poraženog protivnika, iz kojeg se uzima oprema;
ako je vlastiti ruksak pun, iz istog se ekrana može nešto baciti.

### Bodovanje i kraj igre

| Događaj                        | Bodovi                                     |
| ------------------------------ | ------------------------------------------ |
| pobjeda nad protivnikom        | 10 × razina protivnika × (1 + težina lokacije) |
| pobjeda nad srednjim bossom    | dodatnih 200                               |
| pobjeda nad završnim bossom    | dodatnih 500                               |
| preostali bodovi života        | po jedan bod za svaki                      |
| bijeg iz borbe                 | bez bodova                                 |
| svaki dan iznad prvoga         | −25 od konačnog rezultata                  |

Ulaz u Voidspire Cave zapečaćen je dok su živa oba srednja bossa, koji čekaju u
šumama Mirkwood i Thornwood. Igra završava pobjedom nad završnim bossom ili
gubitkom svih bodova života; u oba slučaja rezultat se upisuje u listu najboljih.

## Unit testovi

Testovi su pisani u okviru *Microsoft Native Unit Test* i nalaze se u zasebnom
projektu `Tests` unutar istog solutiona. Pokreću se iz *Test → Test Explorer →
Run All Tests*.

| Datoteka                    | Što pokriva                                                    |
| --------------------------- | -------------------------------------------------------------- |
| `test_item.cpp`             | čitanje predložaka iz datoteka, rijetkosti, slotovi, iznimke   |
| `test_inventory.cpp`        | kapacitet ruksaka, redoslijed predmeta, slotovi, zbroj bonusa  |
| `test_player_character.cpp` | preračun statistika, opremanje, primljena šteta, uzimanje predmeta |
| `test_battle.cpp`           | redoslijed poteza, izračun štete, obrana, uvjeti kraja borbe   |
| `test_map.cpp`              | početna i završna lokacija, veze, dostupnost svih lokacija     |
| `test_score.cpp`            | formula bodovanja, kazna po danima, lista najboljih            |

Uz uobičajene slučajeve pokriveni su i rubni: prazan i pun inventar, pozicija
izvan granica, nepoznate vrijednosti u datotekama, napad slabiji od obrane,
kazna veća od osvojenih bodova te oštećeni zapisi u listi rezultata.

Test projekt uključuje samo datoteke s logikom igre. `UI.cpp`, `Game.cpp`,
`InventoryEngine.cpp` i `main.cpp` izostavljeni su jer ovise o konzoli i vlastitoj
`main` funkciji.

## Struktura projekta

| Datoteka                     | Opis                                                            |
| ---------------------------- | --------------------------------------------------------------- |
| `Item.h/.cpp`                | hijerarhija predmeta i skladište predložaka učitanih iz datoteka |
| `Inventory.h/.cpp`           | generički spremnik te ruksak, spremnik oklopa i spremnik oružja  |
| `Player_Character.h/.cpp`    | apstraktni lik, igrač s opremom i protivnik                      |
| `BattleEngine.h/.cpp`        | tijek borbe i redoslijed poteza ovisan o brzini                  |
| `Location.h/.cpp`            | lokacija, njezin tip i težina                                    |
| `Map.h/.cpp`                 | mapa svijeta kao graf lokacija                                   |
| `EnemyFactory.h/.cpp`        | stvaranje protivnika, srednjih bossova i završnog bossa          |
| `Score.h/.cpp`               | bodovanje, brojač dana i lista najboljih rezultata               |
| `Random.h/.cpp`              | zajednički generator slučajnih brojeva                           |
| `Exception.h`                | hijerarhija iznimaka koje igra prijavljuje                       |
| `Game.h/.cpp`                | glavna petlja i povezivanje svih dijelova igre                   |
| `UI.h/.cpp`                  | konzolno sučelje, izbornici i crtanje mape                       |
| `InventoryEngine.h/.cpp`     | radnje nad predmetima u inventaru                                |
| `main.cpp`                   | ulazna točka i hvatanje iznimaka                                 |
| `armor.txt`, `weapon.txt`, `junk.txt` | predlošci predmeta                                     |
| `Enemy`                      | imena protivnika razvrstana po tipu lokacije                     |
| `Tests/`                     | projekt s jediničnim testovima                                   |

Logika igre ne ovisi o konzolnom sučelju: sav ispis i unos nalaze se u klasi `UI`,
pa se borba, inventar, predmeti i mapa mogu testirati bez nje.

## Format datoteka s predmetima

Prvi redak svake datoteke je zaglavlje i preskače se. Vrijednosti su odvojene
znakom `;`.

**armor.txt**

```
id;name;rarity;defense;speed;slot
1;Leather Helmet;common;2;0.01;helmet
```

Dopuštene vrijednosti: `rarity` – `common`, `rare`, `epic`, `legendary`;
`slot` – `helmet`, `chest`, `gloves`, `pants`, `boots`.

**weapon.txt**

```
id;name;rarity;defense;speed;attack;slot
1;Iron Sword;common;1;0.05;8;right_hand
```

Dopuštene vrijednosti: `slot` – `left_hand`, `right_hand`.

**junk.txt**

```
id;name;rarity
1;Rusty Nail;common
```

**Enemy**

```
type;name
Forest;Sylvan Stalker
```

Dopuštene vrijednosti za tip lokacije: `City`, `Forest`, `Road`, `Cave`.

Za dodavanje novog predmeta ili protivnika dovoljno je dopisati redak u
odgovarajuću datoteku; kod se ne mijenja.
