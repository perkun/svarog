# Svarog

## TODO:

- [ ] ogarnąć observera, target i światła w Scene i Sherderach. Wywalić ze Sceny
	może, bo tylko MainLayer tego używa
- [ ] wykresy z https://github.com/epezent/implot
- [ ] texture wrapping poprawić w shaderach



Svarog is an Application Engine.

The name "Svarog" comes from the slavic god of fire and god of the sun and
blacksmithing, equated with Hephaistos and Helios in Russian chronicles.






## Rarog

In Slavic mythology, the Raróg (Russian: Рарог) is a fire demon, often depicted
as a fiery falcon.




### Desired Features:

- [X] branie opcji z lini komend (nazwa pliku z modelem, tekstura, pozycje, ...)
- [X] GUI:
    - pozycje planetoidy,
    - kąty
    - pozycja obserwatora
- [X] wczytywanie modelu (File->Open)
- [X] wczytywanie tekstury
- [X] wczytywanie pliku z pozycjami
- [X] ustawianie obiektów w scenie ręcznie: rzut "z góry", przeciąganie obiektów
- [ ] przełączanie shaderów
- [ ] shading goły ("naukowy") i "filmowy" (wyglądający cool)
- [X] zapisywanie zrzutu sceny do .png
- [X] zapisywanie/wczytywanie sceny
- [X] generowanie i wyświetlanie krzywej zmian jasności w danej konfiguracji
- [ ] operacje na modelu
    - [ ] Perlin noise
    - [ ] fraktale
    - [ ] catmull-clark
    - [ ] laplacian smoothing
    - [ ] normalizacja
    - [ ] liczenie wspł. tekstóry
- [o] formaty plików
    - [X] OBJ
    - [X] SHP
    - [ ] chmara punktów
- [ ] Marching Cubes na chmurze punktów


# Installation

## Requariments

Install those packages in your system using system package manager:

* premake
* glm
* glfw-x11
* glew
* cfitsio
* yaml-cpp

also, install this library:
[https://github.com/perkun/cppargs.git](https://github.com/perkun/cppargs.git)

and this one:
[https://github.com/gabime/spdlog.git](https://github.com/gabime/spdlog.git)

Note: after compiling as described in spdlog's README, do `make install` (as
root)

## Compilation

Download source

```bash
git clone https://github.com/perkun/svarog.git
cd svarog
git submodule update --init
```

Edit the two lines near the end of  `premake5.lua`:

```
		defines "SV_CONFIG_FONT_PATH=\"/home/perkun/projects/svarog/Svarog/assets/fonts\""
```

the path should begin with the path to the directory where svarog project was
cloned

Then, compile

```bash
premake5 gmake
make
```

If you get errors from `stb_image` library, add `#define STBI_NO_JPEG` ate the
beginning of `Svarog/vendor/stb_image/stb_image.cpp` file. If that does not
help, contact me.



