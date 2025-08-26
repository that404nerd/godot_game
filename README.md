# FPSProject

#### This project was made using the following template: [GDExtensionTemplate](https://github.com/asmaloney/GDExtensionTemplate)

### Build & Install

```bash
mkdir build && cd build/
cmake -B FPSProject-build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=FPSProject-install FPSProject ..
cmake --build FPSProject-build -j8
cmake --install FPSProject-build
```

- To recompile any code changes from the src folder just run the following commands (inside the root directory of the project):
```bash
sh build.sh
```