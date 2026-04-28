# FPSProject

#### This project was made using the following template: [GDExtensionTemplate](https://github.com/asmaloney/GDExtensionTemplate)

### Open the project using the project.godot file in the root directory of the project

- F5 to debug in VSCode.

### Build & Install

```bash
git clone --recursive https://github.com/that404nerd/godot_game
cd godot_game
mkdir build && cd build/
cmake -B FPSProject-build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=FPSProject-install FPSProject ..
cmake --build FPSProject-build -j8
cmake --install FPSProject-build
```

- To recompile any code changes from the src folder just run the following commands (inside the root directory of the project):

```bash
sh build.sh
```

- Incase if you want to recompile your entire project maybe due to the addition of new files or some other reason (inside the root directory of the project):

```bash
sh recompile.sh
# This will not build the project, to build it:
sh build.sh
```

Weapons and Rigs from Sketchfab:
- [Pistol](https://sketchfab.com/3d-models/pistol-animated-f4d14fe2e7d94070afe730ab99458714)
- [Shotgun](https://sketchfab.com/3d-models/shotgun-animated-963e540358b84ca9b72a5e4d59acd57c)
- [Rifle](https://sketchfab.com/3d-models/animated-fps-hands-rifle-animation-pack-5f2d0ed780a94724b36ab505f7564057)
