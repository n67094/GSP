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

**Note:** this step is only need if you add/modify data

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
make TARGET=release
```
or
```
make
```

## Debug

**Commande :**
```
make TARGET=debug
```
This will includes to possibility to use log and assert, c.f (src/debug/)

## Note

**Recommanded mGBA version:** 0.9.3

Durgin the development we noticed that mGBA 0.9.2 and mGBA 0.10.x had unexpected behavoir, we recommand you to use the version 0.9.3, or the hardware

