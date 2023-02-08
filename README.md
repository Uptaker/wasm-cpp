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

C/C++ faili compileerimiseks on vajalik importida järgmist *header* faili igas failis: 
```cpp
#include <emscripten.h>
```

Funktsioonide säilitamiseks väljaspool `main()` funcktsiooni, `EMSCRIPTEN_KEEPALIVE` makro on kohustuslik iga eksporditud funktsiooni ees. See ütleb kompilaatorile, et ta eksportiga teda väljastatud `.wasm` faili.

Kohalik arenduskeskond (IDE) ilma WebAssembly pluginadeta võib nende peale näidata vigu või hoiatusi, kuid kõik on hästi, kuna `emcc` kompilaator tuvastab neid ära ning edukalt kompileerib.

- `-o <file> väljund fail
- `-s[OPTION]` lülita sisse/välja sisseehitatud parameetri, nt `-s NO_EXIT_RUNTIME=1`, et mitte väljuda käivituskeskkonnast või `-s EXPORTED_RUNTIME_METHODS=[ccall]`, et täpsustada exportitud funktsioonid.
- `--help` manual

## C++ funktsioonide väljakutsumine brauseris

### Märkus C++ ja C kohta

Ainuke erand C++ juhul on see, et erinevalt C keelest, funktsiooni nimed on hoopis tükeldatud/rikutud (mangled). See tähendab, et peame kasutama `extern "C"` funktsiooni ees, et selle alles hoida.
Üks arendussõbralik viis exportida C++ funktsioone on defineerida `EXTERN` makro, nagu siin:
```cpp
#define EXTERN extern "C
```

ning siis lisa selle funktsiooni ette, nagu allolevas näides:
```cpp
EXTERN EMSCRIPTEN_KEEPALIVE
void log(string message) {
	cout << message << endl;
}
```

### Funktsioonide väljakutsumine

Traditsiooniline viis C/C++ funktsioonide väljakutsumiseks on importida loodud `.js` faili ning kutsuda välja antud `cwrap()` või `ccall()` JavaScriptis.

Näidis nende kasutamisest on **wrapping** kataloogis.

`ccall()` otseselt kutsub välja kompileeritud C/C++ funktsiooni.

```js
const result = Module.ccall('multiply', 'number', ['number', 'number'], [num1.value, num2.value])
```

`cwrap()` samuti otseselt kutsub välja kompileeritud C/C++ funktsiooni, kuid laseb sul defineerida `wrapper` funktsiooni, mis on kasulikum, kui on vajadus funktsiooni kutsuda välja mitu korda.
```js
const cwrapMultiply = Module.cwrap('multiply', 'number', ['number', 'number'])
const result = cwrapMultiply(num1.value, num2.value)
```
These methods require you to explicitly pass the parameter types, as well as the return value in JavaScript.

Both of these call the equivalent C++ function:
```cpp
EXTERN EMSCRIPTEN_KEEPALIVE
int multiply(int a, int b) {
    return a * b;
}
```

Enne kompilatsiooni:
- `EMSCRIPTEN_KEEPALIVE` makro peab olema funktsioonide ees
- lisa `NO_EXIT_RUNTIME=1 -s EXPORTED_RUNTIME_METHODS=ccall,cwrap` parameetrid kompileerimisel

Kuid siiski on parem viis C/C++ funktsioonide kutsumiseks, mis on näidatud allpool.

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

