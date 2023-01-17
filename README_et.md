# WebAssembly ja C++

## Emscripten

Emscripten on kompilaator, mis saab kompileerida nii C ja C++ programmeerimiskeelt, kui ka mistahes keelt, mis kasutab LLVM-i. Kompileeritud väljundiks on WebAssembly kood, mis laseb toetavatel keeltel kasutada WASM käivituskeskkonda (*runtime-environment*). WebAssembly-ks konverteerides saab käivitada programmid läbi brauseri või mujal ning võimalusi on palju - kompileerida saab nii olemasolevad programmibaasid, traditsioonilised graafilised arvuti programmid (näiteks Qt või GTK raamistikuga) või ka terved mängud, nagu [Doom 3](https://wasm.continuation-labs.com/d3demo/)

Emscipteni töövoogu taga on LLVM projekti `clang` kompilaator, mis kompileerib C ja C++ koodi WASM-iks. Kompileerimiseks on vajalik installeerida `emcc` kompilaatori, mis on osa EMSDK (Emscripten Development Kit) arenduskeskkonnast ning annab võimaluse väljastada `.wasm` ja `.js` failid.

## Emscripteni paigutus

Antud paigutamise juhend on loodud Linux/Unix süsteemidele.

Kõige soovitatavam viis teha tööd Emscripteniga on läbi EMSDK.

1. Esmaseks sammuks on vajalik kloonida ametliku EMSDK repositooriumi.

```bash
git clone https://github.com/emscripten-core/emsdk.git
```

2. Installeri kõige viimase versiooni:

```bash
cd emsdk && ./emsdk install latest
```

3. Pärast installatsiooni, lõpuks aktiveeri

```bash
./emsdk activate latest
```

4. Et alustada tööd praeguses terminalis, on vajalik väljastada `source` nii, nagu oli kirjas pärast eelneva käsu käivitamist.

5. Kontrolli installatsiooni: 
```bash
emcc -v
```

**Paigaldatud!**

**NB!** Võib olla vajadus uuesti käivitada `source` käsku iga uue terminali avamisega, et `emcc` kompilaatori uuesti tööle panna. Tee seda kasutades järgmise käsu:
```bash
source "[EMSDK_ROOT]/emsdk_env.sh"
```

..kus [EMSDK_ROOT] on EMSDK installatsiooni kataloogi asukoht.

## Testi WASM "Hello World" programmiga

Kui testprogramm kompileerib, siis Emscripten on edukalt paigaldatud!

```bash
emcc hello_world/hello.cpp
```

Kui kompileerida `hello.cpp` kasutades traditsioonilist g++/gcc kompilaatori, siis märka, kuidas järgmised väljundid on samad:
```bash
./hello.out
"Hello world!"
```

```bash
node ./a.out.js
"Hello world!"
```

Sel juhul, väljustatud `.js` kood on põhimõtteliselt koodi ümbris (*wrapper*), mis laseb binaarsel WASM koodil käivitada läbi NodeJS käivituskeskkonna ning seetõttu saab olla käivitatud läbi JavaScript või HTML faili.

## Levinud emcc parameetrid

- `-o <file> väljund fail
- `-s[OPTION]` lülita sisse/välja sisseehitatud parameetri, nt `-s NO_EXIT_RUNTIME=1`, et mitte väljuda käivituskeskkonnast või `-s EXPORTED_RUNTIME_METHODS=[ccall]`, et täpsustada exportitud funktsioonid.
- `--help` manual

## C++ funktsioonide väljakutsumine brauseris

TODO C++ calling bit (from compiled .js files) using ccall and cwrap

## Parem viis C++ funktsioonide välja kutsumiseks - WASM Streaming

Lihtsam ja intuitiivsem viis funktsioonide väljakutsumiseks on *striimida* `.wasm` faile. Niimoodi, JavaScript saab importida WASM funktsioonid ning ei ole vajadust ka kasutada väljastatud `.js` faile.

Üks teine eelis on see, et kompileerimiseks ei ole enam tarvis lisada `-s NO EXIT_RUNTIME=1 -s EXPORTED_RUNTIME_METHODS=[ccall]`.

Näidis on demonstreeritud `streaming` kataloogis.

## Mitme faili kompileerimine

`emcc` paneb kokku terve lähtekoodi. See tähendab, et kui sul on näiteks mitmest osast programm, kus on näiteks abifunktsioonid, siis need on kompileeritavad järgmiselt:

```bash
emcc main.cpp utils.cpp -o output.js
```

Näidis on `memory` kaustas, kus `memory.cpp` importib `malloc.cpp` funktsioone.

