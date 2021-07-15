# Svarog

Svarog is an Application Engine written in C++ and OpenGL. It enables creating
3D applications.

The Svarogs' code is based on the [Hazel engine](https://github.com/TheCherno/Hazel).

The name "Svarog" comes from the slavic god of fire and god of the sun and
blacksmithing, equated with Hephaistos and Helios in Russian chronicles.


# Applications

Svarog engine is used to create these applications:

## Rarog

Solar System scene editor. Add asteroid models, move them around, observe
synthetic lightcurves, AO and radar images.

### Features:

* loading / saving scenes
* loading asteroid models
* generating lightcurves
* making Adaptive Optics images
* making radar images
* arranging models in hierarchy
* moving, rotating, scaling models
* textures
* multiple shaders
* synthetic observations' storage
* importing lightcurves for comparison

## meshview

Simple .obj and .shp model viewer. Use mouse to rotate the model, hold `H` for
help.

## meshlc

Program for generating lightcurves from .obj and .shp models. As for now, it
uses Lambert + Lommel-Seeliger scattering law (with an adjustable parameter).



# Installation

## Requirements

Install those packages in your system using system package manager:

* premake
* glm
* glfw-x11
* glew
* cfitsio
* yaml-cpp
* vim (required for `xxd` utility)
* zenity

## Download source

```bash
git clone https://github.com/perkun/svarog.git
cd svarog
git submodule update --init
```

## Installing in the system

Run
```
chmod +x install
./install
```

Do not run as root. It will prompt for password at the end to copy executables
to `/usr/local/bin`. The script creates `/home/$USER/.config/svarog` folder and
copies default `.ini` files and fonts there.

## Compiling in local folder

You can compile the code without installing it. The executables will be located
in `bin/CONFIG/PROGRAM`, where `CONFIG` is `Debug` or `Release`, and `PROGRAM` is
either `rarog`, `meshlc` or `meshview`.

To compile the code, run

```bash
premake5 gmake
```
and then, either
```
make
```
for the `Debug` version, or
```
make config=release
```
for the optimized release version.

The panels in `rarog` program will be a mess. You can copy
```
cp data/rarog_imgui.ini bin/CONFIG/rarog/imgui.ini
```
to have a default panel layout.


If you get errors from `stb_image` library, add `#define STBI_ONLY_PNG` at the
beginning of `Svarog/vendor/stb_image/stb_image.cpp` file. If that does not
help, contact me.





