# Frontend pro LLVM 3.8.0 Compiler Infrastructure

Semestralni prace BI-PJP.

## Zadani

Rozsirte skupinu prekladacu gcc/llvm o novy jednoduchy frontend
zpracovavajici vstupni jazyk inspirovany jazykem Pascal. Vystupem
frontendu bude abstraktni syntakticky strom ve formatu GENERIC
prekladacu gcc/IR prekladace llvm. V nasem vstupnim jazyce se omezte
na deklaraci konstant, globalnich promennych a funkci, a datovy typ
int. Ciselne konstanty umoznete zadavat dekadicky, hexadecimalne
a octalove. Podporujte aritmeticke vyrazy s operacemi +, -, *, div,
mod a zavorkami. Z prikazu podporujte prirazeni, if, while, for,
readln a writeln. Samozrejme podporujte deklaraci a volani funkci,
procedur s partametry, i v rekurzi.

Pro vstup a vystup (readln a writeln) vytvarejte uzly stromy
reprezntujici volani standardnich funkci scanf a printf, ktere jsou
k dispozici prilinkovanim v prostredi semestralni ulohy. Nacitejte
a vypisujte vzdy jedno cislo.

## Kompilace

Je treba stahnout a rozbalit LLVM 3.8.0:
http://llvm.org/releases/download.html#3.8.0

    $ mkdir llvm-obj
    $ cd llvm-obj
    $ cmake path/to/llvm/source
    $ cmake --build .
    $ cd ../llvm-sfe/src
    $ make

## Spusteni programu

V adresari llvm-sfe/src bude po kompilaci spustitelny soubor llvm-sfe.

    ./llvm-sfe path/to/yout/source/file

## Popis adresaru

`llvm-3.8.0.src/` zdrojove kody LLVM Compiler Infrastructure

`llvm-sfe/src/` zdrojove kody frontendu

`llvm-sfe/code-samples` ukazkove zdrojove kody zjednoduseneho Pascalu

`llvm-sfe/my-samples` moje ukazkove zdrojove kody

`llvm-sfe/docs` ll(1) gramatika jazyka

`llvm-sfe/include` prevzaty kod pro pouziti JIT kompilatoru
