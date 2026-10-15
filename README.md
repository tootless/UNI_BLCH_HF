# Maišos funkcija

### Mokymosi tikslais, parašyta C++

# Paleidimo instrukcija
## Reikalingi įrankiai
- CMake
- C++ kompiliatoriaus, pavyzdžiui, GCC arba MSVC

## Windows terminalo žingsniai

- cd C:\idiegto_projekto_vieta
- cmake -S . -B build
- cmake --build build

## Paleidimas MinGW / GCC
- .\build\hash_f.exe  
ir  
- .\build\experiments.exe

## Paleidimas VS
- .\build\Debug\hash_f.exe ir .\build\Debug\experiments.exe
arba
- .\build\Release\hash_f.exe ir .\build\Release\experiments.exe

## Linux (g++ ir cmake) / MacOS (Xcode command line tools ir CMake) terminalo žingsniai

Projekto aplanke:  
- cmake -S . -B build
- cmake --build build

- ./build/hash_f
- ./build/experiments

## Svarbu apie Linux / MacOS:
Visus eksperimentus paleidžiant programa gali veikti be ekrano sustojimo tarp eksperimentų, tad yra rekomenduotina leisti tik po vieną.

# v1.1

## Pakeitimai

- Labiau komplikuotas mixing, paprastas padding, salt, atlikti 7 eksperimentai.

# Eksperimentai
- Eksperimentus galima paleisti atskirai po vieną (įvesti nuo "1" iki "7") arba visus vieną po kito (įvesti "100"), paleidus ```eksperimentai.exe```  
- Eksperimentai buvo daryti įjungus Visual Studio Release, O2 optimizaciją.

## Eksperimentų 

# Eksperimentai 1 - 3
Šie eksperimentai buvo daryti tik v1.1, nes sekančios versijos nepakeitė šių esminių dalykų ir testai visi praėjo sėkmingai.
## E1: Įvesčių paruošimas
Eksperimentas 1 paruošia visas nurodytas įvestis, arba pakeičia jau egzistuojančių failų turinį, ir įtalpina juos į /inputs/ aplanką.  
Įvesčių teisingumas yra patikrintas programoje, pvz. atsitiktinio ASCII teksto vieno baito pakeitimo atveju keitimo metu yra vykdoma patikra, ar bitai netyčia nesutapo, tuomet yra bandoma iš naujo, tad naudotojas nieko nežino.

## E2: Išvesties formatas
Eksperimento 2 rezultatai yra teisingi dėl ```to_hex()``` realizacijos ir ```std::setfill('0')```.  
Kiekvienas nurodytas ilgis duoda 256 bitų ilgio hex maišą.
Rankinė bei failo įvestys sutampa, kai jų baitai vienodi.

## E3: Determinizmas
Eksperimentas 3 buvo atliktas su seka A B A C A C tam, kad itin įsitikinti, kad nėra jokių išliekančių būsenų. Neatitikimų nebuvo.  

# Eksperimentai 4 - 7

## E4: Efektyvumas
Eksperimentas 4 buvo atliktas po trijų apšilimo paleidimų.

### Lentelė


## E5: Kolizijos
Abecėlė: ASCII simboliai 33-126 (94 iš viso). Matuojamas poru skaicius kiekvienam ilgiui: 100000.  
Hash funkcija darytuose testuose neranda kolizijų, tačiau tai nereiškia, kad ji yra criptografiškai saugi. Taip yra todėl, kad yra labai daug galimų skirtingų maišos reikšmių, kadangi yra išvedami 64 hex simboliai, kiekvienas hex simbolis gali turėti 16-a skirtingų reikšmių (maždaug 16^64 galimybių).  
Taip pat kriptografinis saugumas nėra tik kolizijų nebūvimas, gali būti (ir taip šiuo atveju yra), kad yra kitų struktūrinių problemų.

## E6: Lavinos efektas
1 lentelė. Vieno **ASCII** simbolio pakeitimas
| Įvesties ilgis    | Porų skaičius | Bitų skirtumas min. | Bitų skirtumas maks. | Bitų skirtumas vid. | HEX skirtumas min. | HEX skirtumas maks. | HEX skirtumas vid. |
|-------------------|---------------|---------------------|----------------------|---------------------|--------------------|---------------------|--------------------|
| 10 baitų          | 25 000        | 38.6719%            | 65.2344%             | 50.0099%            | 76.5625%           | 100%                | 93.7429%           |
| 100 baitų         | 25 000        | 37.8906%            | 61.7188%             | 50.0058%            | 78.125%            | 100%                | 93.7274%           |
| 500 baitų         | 25 000        | 28.9062%            | 61.7188%             | 49.8851%            | 57.8125%           | 100%                | 93.5231%           |
| 1000 baitų        | 25 000        | 38.2812%            | 62.8906%             | 50.0091%            | 79.6875%           | 100%                | 93.7308%           |
| Bendri rezultatai | 100 000       | 28.9062%            | 65.2344%             | 49.9775%            | 57.8125%           | 100%                | 93.6811%           |

2 lentelė. Vieno atsitiktinio įvesties bito apvertimas
| Įvesties ilgis    | Porų skaičius | Bitų skirtumas min. | Bitų skirtumas maks. | Bitų skirtumas vid. | HEX skirtumas min. | HEX skirtumas maks. | HEX skirtumas vid. |
|-------------------|---------------|---------------------|----------------------|---------------------|--------------------|---------------------|--------------------|
| 10 baitų          | 25 000        | 34.7656%            | 62.1094%             | 50.0329%            | 79.6875%           | 100%                | 93.7474%           |
| 100 baitų         | 25 000        | 35.1562%            | 62.1094%             | 50.0070%            | 78.125%            | 100%                | 93.7521%           |
| 500 baitų         | 25 000        | 29.6875%            | 63.6719%             | 49.8769%            | 57.8125%           | 100%                | 93.5012%           |
| 1000 baitų        | 25 000        | 37.5000%            | 62.8906%             | 50.0309%            | 78.125%            | 100%                | 93.7554%           |
| Bendri rezultatai | 100 000       | 29.6875%            | 63.6719%             | 49.9869%            | 57.8125%           | 100%                | 93.6890%           |

## E7: Spėjimas, vieša druska, slaptas atsitiktinumas


!["e7img"](/assets/v1.1/e7.png)