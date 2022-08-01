# GSP

This is a GBA game made during the GBA game jam 2022

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

## Release

**Commande :**
```
make TARGET=release
```

## Debug

**Commande :**
```
make TARGET=debug
```
