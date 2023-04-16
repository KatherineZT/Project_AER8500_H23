## Linux (with CMake)

### Compilation

Find and Replace in all files the string `d/MyDocuments/1-Bacc-Genie-Informatique/2023-1-H/AER8500/Projet/Project_AER8500_H23` to the path corresponding to where you store this repository

```
cmake fcm
cd fcm
make
```

You can `make clean` anytime.

### Run

```
./fcm
```

## Windows

### Dependencies

Make sure you have Mingw-w64 installed (it is a C++ development environment for Windows, like Visual Studio, it is the program that you need to execute make on Windows)

> Check in your `Path` environment variable if there is a path ending with `mingw64\bin`

> If none is found, download first ZIP file in Assets section of this project (https://github.com/skeeto/w64devkit/releases/tag/v1.17.0), extract to the folder of your choice and add `bin` directory inside the extracted archive to `Path` environment variable

### Compilation

```
x86_64-w64-mingw32-g++ -o fcm.exe main.cpp
```

### Run

```
fcm.exe
```