# WebAssembly ja C++

## Emscripten

Emscripten on kompilaator, mis saab kompileerida nii C ja C++ programmeerimiskeelt, kui ka mistahes keelt, mis kasutab LLVM-i. Kompileeritud väljundiks on WebAssembly kood, mis laseb toetavatel keeltel kasutada WASM käivituskeskkonda (*runtime-environment*). WebAssembly-ks konverteerides saab käivitada programmid läbi brauseri või mujal ning võimalusi on palju - kompileerida saab nii olemasolevad programmibaasid, traditsioonilised graafilised arvuti programmid [[1]](https://emscripten.org/docs/introducing_emscripten/about_emscripten.html) (näiteks Qt või GTK raamistikuga) või ka terved mängud, nagu Doom 3 [[2]](https://wasm.continuation-labs.com/d3demo/)

Emscipteni töövoogu taga on LLVM projekti `clang` kompilaator, mis kompileerib C ja C++ koodi WASM-iks. Kompileerimiseks on vajalik installeerida `emcc` kompilaatori, mis on osa EMSDK (Emscripten Development Kit) arenduskeskkonnast ning annab võimaluse väljastada `.wasm` ja `.js` failid. [[3]](https://emscripten.org/docs/introducing_emscripten/about_emscripten.html)

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

Loe rohkem:
- https://emscripten.org/docs/getting_started/downloads.html

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

Funktsioonide säilitamiseks väljaspool `main()` funktsiooni, `EMSCRIPTEN_KEEPALIVE` makro on kohustuslik iga eksporditud funktsiooni ees. See ütleb kompilaatorile, et teda eksportidakse `.wasm` faili [[4]](https://emscripten.org/docs/api_reference/emscripten.h.html#compiling).

Kohalik arenduskeskond (IDE) ilma WebAssembly pluginadeta võib nende peale näidata vigu või hoiatusi, kuid kõik on hästi, kuna `emcc` kompilaator tuvastab neid ära ning edukalt kompileerib.

- `-o <fail.js>` väljund fail
- `-s[OPTION]` lülita sisse/välja sisseehitatud parameetri, nt `-s NO_EXIT_RUNTIME=1`, et mitte väljuda käivituskeskkonnast või `-s EXPORTED_RUNTIME_METHODS=[ccall]`, et täpsustada exportitud funktsioonid.
- `--help` abi
- `-s INITIAL_MEMORY=3GB -s MAXIMUM_MEMORY=3GB` - Luba kasutada 3GB süsteemi mälu - vaikimisi on 256Mb. Kasutatud näiteks `algorithms.cpp` faili kompileerimiseks.

## C++ funktsioonide väljakutsumine brauseris

### Märkus C++ ja C kohta

Ainuke erand C++ juhul on see, et erinevalt C keelest, funktsiooni nimed on hoopis tükeldatud/rikutud (mangled). See tähendab, et peame kasutama `extern "C"` funktsiooni ees, et selle alles hoida. [[5]](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#calling-compiled-c-functions-from-javascript-using-ccall-cwrap)
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
Need meetodid nõuavad parameetrite tüübid ja tagastusväärtuse JavaScriptis, nagu näha üleval olevates näidistes.

Mõlemad meetodid kutsuvad välja järgmise funktsiooni:
```cpp
EXTERN EMSCRIPTEN_KEEPALIVE
int multiply(int a, int b) {
    return a * b;
}
```

Enne kompilatsiooni:
- `EMSCRIPTEN_KEEPALIVE` makro peab olema funktsioonide ees
- lisa `NO_EXIT_RUNTIME=1 -s EXPORTED_RUNTIME_METHODS=ccall,cwrap` parameetrid kompileerimisel

Loe lisaks:
- https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#calling-compiled-c-functions-from-javascript-using-ccall-cwrap

Kuid siiski on parem viis C/C++ funktsioonide kutsumiseks, mis on näidatud allpool.

## Parem viis C++ funktsioonide välja kutsumiseks - WASM Streaming

Lihtsam ja intuitiivsem viis funktsioonide väljakutsumiseks on *striimida* `.wasm` faile. Niimoodi, JavaScript saab importida WASM funktsioonid ning ei ole vajadust ka kasutada väljastatud `.js` faile. Lisaks sellele on see kõige optimiseeritud viis laadida `.wasm` faile. [[6]](https://developer.mozilla.org/en-US/docs/WebAssembly/JavaScript_interface/instantiateStreaming)

Üks teine eelis on see, et kompileerimiseks ei ole enam tarvis lisada `-s NO EXIT_RUNTIME=1 -s EXPORTED_RUNTIME_METHODS=[ccall]`.

Näidis on demonstreeritud `streaming` kataloogis.

## JavaScript moodulitena

Emscriptenile saab ette öelda, et ta kompileeriks JavaScript moodulina. Nii saab kasutada JavaScripti `import` võtmesõna, et kasutada loodud funktsioone.

Näiteks järgneva käsuga loob Emscripten `.mjs` faili, kus on sees antud faili funktsioonid (märgistatud `EMSCRIPTEN_KEEPALIVE` makroga):

```bash
emcc hello.cpp -o hello.mjs
```

Antud `.mjs` faili saab importida mitmet moodi. Esiteks peab JavaScript olema määratud moodulina:

```js
<script type="module">
```
 See lubab importida skripte.

 Nüüd saab kasutada funktsiooni järgmiselt:

 ```js
import wasm from './algorithms.mjs' // ükskõik mis nimi, 'wasm' on kasutatud näitena

let modules = await wasm() // wasm tagastab Promise()

modules._hello()
modules._add(1, 2)

modules.['_hello']()
modules.['_add'](1, 2)
 ```

 Teine variant:

 ```js
import wasm from './algorithms.mjs' // ükskõik mis nimi, 'wasm' on kasutatud näitena

wasm().then(modules => {
    modules._hello()
    modules._add(1, 2)

    modules.['_hello']()
    modules.['_add'](1, 2)
})
 ```

 Sama meetod oli kasutatud `algorithms` kaustas olevas `index.html` failis.


## Mitme faili kompileerimine

`emcc` paneb kokku terve lähtekoodi. See tähendab, et kui sul on näiteks mitmest osast programm, kus on näiteks abifunktsioonid, siis need on kompileeritavad järgmiselt:

```bash
emcc main.cpp utils.cpp -o output.js
```

Mitme faili kompileerimise näidis on `memory` kaustas, kus `memory.cpp` importib `malloc.cpp` funktsioone. Sama kaust annab ka huvilistele aimu mäluhaldusest WebAssembly-s, mis on juba omalaadi keerulisem teema.

Huvilistele C++ ja WebAssembly mäluhaldusest lähemalt:
- https://www.fastly.com/blog/webassembly-memory-management-guide-for-c-rust-programmers
- https://blog.devgenius.io/part-1-memory-management-in-wasm-52195f9b707f

## Kompilatsiooni optimeerimine

Emscripten kompilaator lubab anda parameetrid, mis optimeerivad loodud masinkoodi. Optimisatsiooni tagajärg on tavaliselt suurem faili suurus iga järgneva optimisatsioonitasega ning kauemad kompileerimisajad. Optimisatsiooni tulemus võib mitmekordistada koodi jõudlust.

Optimisatsioonitasemed on järgmised:  `-O1`, `-O2`, `-Os` ja `-O3`, kus 1 on kõige väiksem tase ning 3 on kõige optimeerituim tase.

Kompileerida on võimalik järgnevalt: 
```bash
emcc -O3 minu_fail.cpp
```

# Allikad

1. https://emscripten.org/docs/introducing_emscripten/about_emscripten.html
2. https://wasm.continuation-labs.com/d3demo/
3. https://emscripten.org/docs/introducing_emscripten/about_emscripten.html
4. https://emscripten.org/docs/api_reference/emscripten.h.html#compiling
5. https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#calling-compiled-c-functions-from-javascript-using-ccall-cwrap
6. https://developer.mozilla.org/en-US/docs/WebAssembly/JavaScript_interface/instantiateStreaming


# Loe rohkem

- https://emscripten.org/docs/porting/connecting_cpp_and_javascript/index.html
- https://developer.mozilla.org/en-US/docs/WebAssembly/C_to_wasm

