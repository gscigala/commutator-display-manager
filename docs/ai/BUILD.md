# Build

Le projet utilise Meson. Les commandes documentées par le dépôt sont :

```sh
meson setup build
meson compile -C build
```

Le `meson.build` déclare un exécutable C++20/C17 et les dépendances Boost (Log, Program Options, System), sdbus-c++ >= 2.0.0, pugixml >= 1.0, OpenCV >= 4.0.0 et nlohmann-json >= 3.11.3. La compilation utilise `-Werror` en C++ et lie `bcm2835` ainsi que `libm`.

Le pilote Waveshare est un sous-module Git et certaines sources de `meson.build` le requièrent. Un chemin de polices `Fonts/fonts.h` est inclus par les sources mais n’apparaît pas dans l’inventaire du dépôt. Vérifier ces entrées ainsi que les paquets système avant de conclure qu’un échec vient du code.

Le binaire est installé dans `/usr/bin`, la configuration dans `/etc/commutator-display-manager` et les ressources sous `ressources/`. L’installation vise un système Linux avec D-Bus et un environnement matériel compatible ; les définitions `RPI` et `USE_BCM2835_LIB` sont définies dans le build.
