# GSP

This is a GBA game made for the GBA game jam 2022

# Build

Once the project is cloned submodule should be initilisazed

**Commande :**
```
git submodule update --init --recursive
```

## libseven

Before building the project, you will need to build libseven

**Commande :**
```
cd externals/libseven/
make
```
## bin2s

To convert binary file to asm we use bin2s from the DevKipPro tools.

**Commande :**
```
cd tools/bin2s
make
```

The tool is build you will be able to run **make bin** to convert bin file to **.h** and **.s** files

## Release

**Commande :**
```
make bin //optional if up to date
make TARGET=release
```
or
```
make bin //optional if up to date
make
```

## Debug

**Commande :**
```
make bin //optional if up to date
make TARGET=debug
```
this will includes to possibility to use log and assert, c.f (src/debug/)

## Note

Durgin the development we noticed that mGBA 0.9.2 had unexpected behavoir, we recommand you to use the version 0.9.3, or the hardware
