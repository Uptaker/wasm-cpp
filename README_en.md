# WebAssembly for C++

## Emscripten

Emscripten is a compiler that can compile any language that uses LLVM, including C and C++, into WebAssembly. This enables such languages to run using the WASM runtime and even convert whole codebases and programs (Such as Qt-based desktop programs) or even games, such as [Doom 3](https://wasm.continuation-labs.com/d3demo/) - to be able to run on the browser or elsewhere.

Behind the hood, it uses the LLVM project's `clang` compiler to compile C/C++ to WASM. Using it requires installation of the `emcc` compiler, which is part of the EMSDK toolkit and can be used to generate `.js` and `.wasm` files.  
## Emscripten installation

1. First, clone the official WASM compiler repository:

```bash
git clone https://github.com/emscripten-core/emsdk.git
```

2. Install the latest version:

```bash
cd emsdk && ./emsdk install latest
```

3. Activate it

```bash
./emsdk activate latest
```

4. Source the required environment variables (as said during installation)

5. Check installation:
```bash
emcc -v
```

**Success!**

You may need to re-source `emcc` for every new terminal to run the compiler, do so again using
```bash
source "[EMSDK_ROOT]/emsdk_env.sh"
```
Where [EMSDK_ROOT] is the installation root directory.

## Test WASM on the "Hello World" program

If it compiles, that means Emscripten is successfully installed!

```bash
emcc hello_world/hello.cpp
```

If you compile the `hello.cpp` using g++/gcc, notice how the outputs are the same:
```bash
./hello.out
"Hello world!"
```

```bash
node ./a.out.js
"Hello world!"
```

In this case, the `.js` file is essentially a wrapper, which allows binary code (.wasm) to be executed by JavaScript, and therefore can be invoked using an HTML file or JS.

## Compiling C/C++ and common emcc parameters

Compiling C/C++ requires adding the following header package in each file:
```cpp
#include <emscripten.h>
```

To preserve functions outside of `main()`, the `EMSCRIPTEN_KEEPALIVE` macro is required. This tells the compiler to export it for usage in your `.wasm` file.

Your local IDE may show warnings or errors, but emcc will recognize it and compile it successfully.

- `-o <file> output file
- `-s[OPTION]` switch an option, eg `-s NO_EXIT_RUNTIME=1` for not exiting the runtime, or `-s EXPORTED_RUNTIME_METHODS=[ccall]` for specifying export functions.
- `--help` manual

## Calling C++ functions

### A note about C++ vs C

The only exceptin with C++ is that, unlike C, function names are mangled during compilation, which means we need to use `extern "C"` in front of a function to keep it.

A developer friendly way to export C++ functions is to define a `EXTERN` macro, like this:
```cpp
#define EXTERN extern "C
```

and then just pass it before the exported function, like this:
```cpp
EXTERN EMSCRIPTEN_KEEPALIVE
void log(string message) {
	cout << message << endl;
}
```

### Calling functions

The traditional way of calling C/C++ functions is to import the compiled `.js` file and call either `cwrap()` or `ccall()`

An example of their usages is present in the **wrapping** directory.

`ccall()` directly calls the compiled C/C++ function.

```js
const result = Module.ccall('multiply', 'number', ['number', 'number'], [num1.value, num2.value])
```

`cwrap()` also directly calls the compiled C/C++ function, but allows you to wrap it, which is more useful when repeatedly called
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

Before compilation:
- `EMSCRIPTEN_KEEPALIVE` macro is required to be in front of functions.
- pass `NO_EXIT_RUNTIME=1 -s EXPORTED_RUNTIME_METHODS=ccall,cwrap` parameters when compiling

However, there is a better way calling of C/C++ functions, shown below.

## A better way to call C++ functions - WASM Streaming

An easier and more natural way of calling functions is to also *stream* the `.wasm` files. This way, JS can use WASM functions and there is no need for using the compiled `.js`.

Another advantage is not having to specify `-s NO EXIT_RUNTIME=1 -s EXPORTED_RUNTIME_METHODS=[ccall]` during compilation.

An example is seen under the `streaming` directory.

## Compiling multiple files

`emcc` concatenates all source files, which means if you have multiple helper files, they can easily be compiled as such:

```bash
emcc main.cpp utils.cpp -o output.js
```

See the `memory` folder and how `memory.cpp` imports `malloc.cpp` functions.

