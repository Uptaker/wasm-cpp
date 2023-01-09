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
