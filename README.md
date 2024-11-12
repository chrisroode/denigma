# denigma command line utility

This utility extracts the Enigma XML from a Finale `.musx` file.

## Build Instructions

Download the GitHub repository and pull the `minizip` submodule/

### macOS-Specific

Install the latest cmake and zlib:

```bash
brew install cmake
brew install zlib
```

---

### Windows-Specific

You will need package managers Choclatey (`choco`) and `vcpkg`.

[Choclatey install instructions](https://chocolatey.org/install)  
[VCPKG install instructions](https://github.com/microsoft/vcpkg)

Install the latest cmake and zlib

```bat
choco install cmake
vcpkg install zlib
```

Create a the file `C:\Users\<your-user>\AppData\Roaming\CMake\CMakeUserPresets.json` with the following contents:

```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "vcpkg-toolchain",
      "hidden": false,
      "description": "Use vcpkg toolchain file",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build",
      "cacheVariables": {
        "CMAKE_TOOLCHAIN_FILE": "C:/path/to/your/vcpkg/scripts/buildsystems/vcpkg.cmake"
      }
    }
  ]
}
```

(Specify the actual path where you installed vcpkg.)

---

On either operating system you may need to install other dependencies. The cmake build will report them if so. You can install additional dependencies with `brew` (macOS) or `vcpkg` (Windows).

---

Build with

```bash
cmake -P build.cmake
```

or (macOS-only)

```bash
./build.cmake
```
---

You can clean the build directory with

```bash
cmake -P build.cmake -- clean
```

or (macOS only)

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
