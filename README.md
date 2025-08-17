# FPSProject

### Build & Install

```sh
$ cmake -B FPSProject-build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=FPSProject-install GDExtensionTemplate
$ cmake --build FPSProject-build -j$(nproc)
$ cmake --install FPSProject-build
```

#### CMake With GDExtension is made by using [GDExtensionTemplate](https://github.com/asmaloney/GDExtensionTemplate)