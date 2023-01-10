# wasm-cpp

## WASM installation

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

5. Check installation using `emcc -v`

**Success!**

You may need to re-source `emcc` for every new terminal to run the compiler, do so again using
```bash
source "[EMSDK_ROOT]/emsdk_env.sh"
```

## Test WASM on the "Hello World" program

If it compiles, that means WASM is successfully installed!

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

In this case, WASM is essentially a wrapper, which allows binary code (.wasm) to be executed by JavaScript, and therefore can be invoked using an HTML file.

## Common emcc parameters

- `--help` manual
- `-o <file> output file
- `-s[OPTION]` switch an option, eg `-s NO_EXIT_RUNTIME=1` for not exiting the runtime, or `-s EXPORTED_RUNTIME_METHODS=[ccall]` for specifying export functions.

## Calling C++ functions

TODO C++ calling bit (from compiled .js files)

## WASM Streaming

An easier and more natural way of calling functions is to also *stream* the `.wasm` files. This way, JS can use WASM functions and there is no need for using the compiled `.js`.

Another advantage is not having to specify `-s NO EXIT_RUNTIME=1 -s EXPORTED_RUNTIME_METHODS=[ccall]` during compilation.

An example is seen under the `streaming` directory.

## Compiling multiple files

`emcc` concatenates all source files, which means if you have multiple helper files, they can easily be compiled as such:

```bash
emcc main.cpp utils.cpp -o output.js
```

