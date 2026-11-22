# Architecture observée

## Vue d’ensemble

Le dépôt produit un exécutable unique, `commutator-display-manager`, à partir de `src/` et du pilote Waveshare référencé en sous-module. `main.cpp` lit la configuration et assemble la supervision, les widgets et l’affichage.

## Responsabilités visibles

- `Supervisor` coordonne les widgets, les mises à jour et le rafraîchissement périodique.
- `Display` définit les dimensions et les opérations de rendu utilisées par les widgets.
- `EPaper`, `EPaperFake` et `EPaperWaveshare` portent les interfaces et variantes d’affichage.
- Les paires `Widget*` rendent Sytadin, Vigicrues et IDFM ; les paires `Commutator*` lisent les propriétés D-Bus correspondantes.
- `IdfmTransportMode` et `IdfmLineStatus` représentent les données de transport IDFM.
- `cfg/config.json` choisit les widgets et leurs paramètres ; `res/` contient leurs images.

Cette description reflète les fichiers et symboles observés, pas une spécification métier complète. Les responsabilités peuvent évoluer ; vérifier les appels réels avant une modification.

## Dépendances et plateforme

Meson déclare Boost, sdbus-c++, pugixml, OpenCV et nlohmann-json. Le binaire lie `bcm2835` et active des définitions Raspberry Pi. Le sous-module `lib/e-Paper` fournit le pilote matériel. Les widgets Commutator communiquent via le bus système D-Bus.

## Couplage à surveiller

La supervision dépend concrètement des widgets et de l’affichage ; le rendu des widgets utilise OpenCV. Ce sont des observations, pas des défauts avérés. Préférer des changements locaux et documenter toute évolution structurante dans `DECISIONS.md`.
