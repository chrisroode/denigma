## denigma command line utility

This utility extracts the Enigma XML from a Finale `.musx` file.

### Build Instructions

Currently the only working build is for macOS.

Download the GitHub repository and pull the `minizip` submodule/

You will need to install the latest cmake and zlib:

```bash
brew install cmake
brew install zlib
```

You may need to install other dependencies.

---

Build with

```bash
cmake -P build.cmake
```

or

```bash
./build.cmake
```
---

You can clean the build directory with

```bash
cmake -P build.cmake -- clean
```

or

```bash
./build.cmake -- clean
```
