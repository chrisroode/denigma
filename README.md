# denigma command line utility

This utility extracts the Enigma XML from a Finale `.musx` file.

## Build Instructions

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

## Visual Studio Code Setup

1. Install the following extensions:
   - C/C++ (from Microsoft)
   - C/C++ Extension Pack (from Microsoft)
   - C/C++ Themes (from Microsoft)
   - CMake (from twxs)
   - CMake Tools (from Microsoft)
   - codeLLDB (from Vadim Chugunov)
2. Use the following `.vscode/tasks.json` file:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "type": "shell",
            "command": "cmake --build build",
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```

3. Use the following `.vscode/launch.json` for debugging:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "C++ Debug (codeLLDB)",
            "type": "lldb",
            "request": "launch",
            "program": "${workspaceFolder}/build/build/denigma",
            "args": [],
            "cwd": "${workspaceFolder}",
            "stopOnEntry": false,
            "env": {},
            "preLaunchTask": "build" // Optional: specify a task to build your program before debugging
        }
    ]
}
```
