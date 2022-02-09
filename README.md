# bg2 file input/output

Standalone C++ library for reading and writing `.bg2` files, the native file format of the graphics engine [bg2 engine](https://www.bg2engine.org).

## Build and debug

This project is ready for build with Visual Studio Code on Linux and macOS. Select the appropiate debug and/or build task to build it.

You will need the Microsoft extensions for C++. Also, for macOS the project is configured to use the `CodeLLDB` extension for debug, because the Microsoft extension don't work fine on Apple Silicon processors.

For Windows, there is a project for Visual Studio.

However, the code does not have any dependencies, nor does it require any macros to be configured. You can make it work in any other environment by simply adding the C files to the building system or IDE of your choice.

