# bg2 file input/output

Standalone C++ library for reading and writing `.bg2` files, the native file format of the graphics engine [bg2 engine](https://www.bg2engine.org).

## Build and debug

This project is ready for build with Visual Studio Code on Linux and macOS. Select the appropiate debug and/or build task to build it.

You will need the Microsoft extensions for C++. Also, for macOS the project is configured to use the `CodeLLDB` extension for debug, because the Microsoft extension don't work fine on Apple Silicon processors.

For Windows, there is a project for Visual Studio.

However, the code does not have any dependencies, nor does it require any macros to be configured. You can make it work in any other environment by simply adding the C files to the building system or IDE of your choice.

## WebAssembly JavaScript library

You need to install and configure [emscripten](https://emscripten.org) and [nodejs](https://nodejs.org/en/) in your system. The build script is prepared for work on macOS, Linux and WSL, but you probably can create a Windows command line bat script, based on `src/wasm/build.sh`. However, I recommend using WSL if you are on Windows. Note that the resulting code will be platform independent.

```sh
cd wasm
npm ci
npm run build
```

To use the library in your JavaScript code, you need to distribute the following files:

- bg2io-wrapper.js: Include this file in your html page, wepback project, rollup project, etc. Here you have the wrapper utilities to load `.bg2` files in JavaScript
- bg2io.js: This is the emscripten generated glue file that loads the WebAssembly code, and provides the runtime for C and C++
- bg2io.wasm: This is the file that contains the WebAssembly code

