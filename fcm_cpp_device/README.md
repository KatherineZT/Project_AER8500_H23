### Dependencies

Make sure you have Mingw-w64 installed (it is a C++ development environment for Windows, like Visual Studio, it is the program that you need to execute make on Windows)

> Check in your `Path` environment variable if there is a path ending with `mingw64\bin`

> If none is found, download first ZIP file in Assets section of this project (https://github.com/skeeto/w64devkit/releases/tag/v1.17.0), extract to the folder of your choice and add `bin` directory inside the extracted archive to `Path` environment variable

### Compilation

```
x86_64-w64-mingw32-g++ -o fcm_cpp_device.exe main.cpp
```

### Run

```
fcm_cpp_device.exe
```