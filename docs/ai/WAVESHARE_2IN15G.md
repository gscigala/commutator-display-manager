# Waveshare 2.15inch e-Paper (G)

## Source et portée

- **Document :** *2.15inch e-Paper (G) User Manual*, version 1.0, créé le 2024-09-05.
- **Source officielle :** [PDF Waveshare](https://files.waveshare.com/wiki/2.15inch%20e-Paper%20HAT%2B%20(G)/2.15inch-e-Paper_(G)-user-manual.pdf) ; page produit/documentation : [wiki Waveshare](https://www.waveshare.com/wiki/2.15inch_e-Paper_HAT+_(G)).
- **Copie originale dans ce dépôt :** [`WAVESHARE_2IN15G_ORIGINAL.pdf`](WAVESHARE_2IN15G_ORIGINAL.pdf) (54 pages, environ 13 Mo). Ce PDF est la référence constructeur complète ; cette fiche Markdown en extrait les informations les plus utiles pour la compréhension par une IA et l’intégration au projet.
- **Périmètre :** spécifications du module panneau 2.15inch e-Paper (G). Ce document n’est pas une spécification complète de la carte HAT, du câblage Raspberry Pi ni de l’API du pilote C livré séparément.
- **Version analysée :** 54 pages. Le tableau de commandes, les chronogrammes SPI et les procédures d’initialisation sont principalement rendus sous forme de figures dans le PDF ; leur contenu détaillé n’est donc pas retranscrit ici. Pour les commandes exactes, consulter le PDF et le pilote correspondant.

## Résumé pour l’intégration

Il s’agit d’un écran électrophorétique réfléchissant et bistable, avec pixels noirs, blancs, rouges et jaunes. La couleur disponible dépend de la LUT (lookup table) utilisée. Le panneau contient une RAM d’image, un contrôleur de timing et des tensions de pilotage générées par son circuit intégré. Il se pilote par interface série 3 ou 4 fils ; le manuel décrit notamment le mode SPI 4 fils.

Une mise à jour complète est lente : environ **26 s à 25 °C** selon les caractéristiques du panneau. Prévoir ce délai dans l’ordonnancement et éviter les rafraîchissements fréquents. L’image reste affichée sans alimentation (bistabilité), mais cela ne signifie pas que l’actualisation est instantanée ou sans usure.

## Caractéristiques mécaniques et d’affichage

| Propriété | Valeur documentée |
|---|---|
| Diagonale | 2,15 pouces |
| Résolution | 160 (H) × 296 (V) pixels |
| Densité | 156 DPI |
| Zone active | 26 × 48,1 mm |
| Pas de pixel | 0,1625 × 0,1625 mm |
| Dimensions du panneau | 31,8 × 60,8 × 1,0 mm (H × V × épaisseur) |
| Masse | 3,6 ± 0,5 g |
| Couleurs | Noir, blanc, jaune, rouge (dépend de la LUT) |
| Réflectance blanche typique | 35 % (minimum indiqué : 30 %) |
| Rapport de contraste intérieur | 8:1 |
| Mise à jour typique | 26 s à 25 °C |
| Durée de vie déclarée | 1 000 000 cycles ou 5 ans, condition de fonctionnement indiquée dans le manuel |

Les dimensions et la résolution ci-dessus décrivent le panneau. L’orientation logique dépend de la rotation choisie par le pilote ; ne pas en déduire une orientation d’application sans vérifier le code.

## Électricité et environnement

- Tensions logiques VIO et VDD : **2,3 à 3,6 V**, typique 3,3 V.
- Entrée logique haute : au moins 0,7 × VIO ; entrée basse : au plus 0,3 × VDD.
- Courant de veille indiqué : jusqu’à 5 µA. Puissance de veille profonde typique : 0,003 mW à 3,3 V.
- Le manuel marque puissance et courant de fonctionnement typiques **TBD** : ne pas inventer de budget d’alimentation à partir de ces données.
- Plage de fonctionnement : **0 à +40 °C**.
- Plage de stockage : **−25 à +70 °C** ; condition optimale de stockage indiquée : 23 ± 2 °C et 55 ± 10 % RH.
- Ces valeurs proviennent du manuel du panneau et ne garantissent pas la même plage pour une carte HAT assemblée, le Raspberry Pi ou l’installation complète.

Les valeurs absolues maximales (par exemple −0,3 à +6,0 V pour les alimentations logiques) sont des limites de contrainte, pas des conditions normales d’utilisation.

## Signaux du panneau

| Signal | Direction | Rôle selon le manuel |
|---|---|---|
| BUSY_N | Sortie | Indique l’état du contrôleur |
| RST_N | Entrée | Reset global, actif à l’état bas ; normalement tiré au niveau haut |
| DC | Entrée | Sélection commande / donnée série |
| CSB | Entrée | Sélection du circuit (chip select) |
| SCL | Entrée | Horloge série |
| SDA | E/S | Données série |
| BS | Entrée | Sélection d’interface |
| TSCL, TSDA | Sortie / E/S | Interface I²C maître vers un capteur de température externe ; résistances de tirage nécessaires si utilisée, broches à fixer à un niveau haut ou bas si inutilisées |

Le tableau décrit les broches du panneau COG, pas nécessairement les noms exposés sur le connecteur de la HAT. Le manuel ne suffit pas à déduire le câblage de la carte ou les paramètres SPI côté Raspberry Pi.

## Recommandations d’usage et précautions

1. S’appuyer sur le pilote Waveshare associé à cette variante (2in15 e-Paper G) pour les séquences, LUT, temporisations BUSY et opérations d’alimentation. Ne pas remplacer les séquences par celles d’un modèle monochrome ou d’une autre révision sans validation.
2. Prendre en compte le temps de rafraîchissement d’environ 26 secondes à 25 °C ; cette durée est une valeur typique du panneau, pas un délai maximal garanti.
3. Manipuler avec précaution : le verre peut casser en cas de choc ou de chute ; les surfaces avant sont fragiles et le module est sensible à l’électricité statique.
4. En cas de bris, ne pas toucher le matériau électrophorétique ; en cas de contact, laver à l’eau et au savon.
5. Éviter les gaz corrosifs (notamment acides et alcalins). Ne pas désassembler le module : cela peut l’endommager définitivement et annuler la garantie.

## Correspondance avec ce dépôt

- `meson.build` compile `lib/e-Paper/E-paper_Separate_Program/2in15_e-Paper_G/.../EPD_2in15g.c` et ses auxiliaires.
- `src/EPaper.cpp` utilise l’en-tête `EPD_2in15g.h`.
- `src/EPaperWaveshare.cpp` dimensionne le tampon à partir de `EPD_2IN15G_WIDTH` et `EPD_2IN15G_HEIGHT`.
- `src/EPaperWaveshare.h` associe les couleurs de l’application à l’ordre du pilote.

Ces liens identifient la variante de pilote choisie dans le dépôt ; ils ne prouvent pas à eux seuls que chaque séquence ou choix de LUT respecte tous les détails du manuel. Pour toute modification du rafraîchissement, vérifier le pilote exact et la révision réelle du matériel.

## Références

- Manuel utilisateur, révision 1.0, sections 1–6, 8–10, 12–13.
- [PDF officiel Waveshare](https://files.waveshare.com/wiki/2.15inch%20e-Paper%20HAT%2B%20(G)/2.15inch-e-Paper_(G)-user-manual.pdf)
- [Wiki officiel 2.15inch e-Paper HAT (G)](https://www.waveshare.com/wiki/2.15inch_e-Paper_HAT+_(G))
