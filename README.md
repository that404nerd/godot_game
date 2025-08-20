# FPSProject

#### This project was made using the following template: [GDExtensionTemplate](https://github.com/asmaloney/GDExtensionTemplate)

### Build & Install

```bash
mkdir build && cd build/
cmake -B FPSProject-build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=FPSProject-install FPSProject ..
cmake --build FPSProject-build -j$(nproc)
cmake --install FPSProject-build
```

- To recompile any code changes from the src folder just run the following commands (inside build/FPSProject-install/FPSProject):
```bash
sh build.sh
```
### Make sure to create your godot project inside the build/FPSProject-install/FPSProject folder.
