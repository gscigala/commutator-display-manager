# Règles de contribution assistée

## Projet

Commutator Display Manager affiche des informations de trafic, de transport et de crues sur un écran e-paper. L’application C++20 est construite avec Meson. Le point d’entrée est `src/main.cpp`; les widgets, les clients Commutator, la supervision et les interfaces d’affichage se trouvent dans `src/`. La configuration est dans `cfg/` et les images dans `res/`. Le pilote Waveshare est référencé comme sous-module `lib/e-Paper`.

Les dépendances déclarées sont Boost (Log, Program Options, System), sdbus-c++, pugixml, OpenCV et nlohmann-json. Le binaire utilise D-Bus et le pilote matériel Raspberry Pi/Waveshare. Voir `docs/ai/ARCHITECTURE.md` et `docs/ai/BUILD.md` pour le contexte détaillé.

## Règles C++ et architecture

- Respecter C++20 et le style local ; conserver les interfaces publiques et le comportement existant, sauf demande explicite.
- Préférer RAII, types valeur et pointeurs intelligents lorsque leur sémantique convient. Éviter la gestion manuelle des ressources, les comportements indéfinis et les accès concurrents non synchronisés.
- Utiliser `const` quand il exprime correctement l’intention ; gérer erreurs et exceptions de manière cohérente avec le module.
- Garder les responsabilités séparées, les dépendances orientées dans un seul sens et les changements ciblés. Éviter les duplications et refactorings massifs non justifiés.
- Documenter dans `docs/ai/DECISIONS.md` les décisions structurantes, leur contexte, les options considérées et leurs conséquences.

## Git et modifications

- Préserver les changements utilisateur. Ne pas écraser de fichiers ou modifications existants.
- Ne jamais faire de reset destructif ni réécrire l’historique Git. Ne pas créer de commit sans demande explicite.
- Ne modifier aucun code métier pour une tâche limitée à l’infrastructure IA.

## Validation

- Pour un changement logiciel, compiler les éléments concernés avec Meson et exécuter les tests pertinents s’ils existent ; vérifier les warnings et régressions.
- Le dépôt audité ne déclare pas de tests Meson. Le pilote référencé en sous-module n’est pas initialisé dans certains environnements ; signaler si cela empêche la compilation et ne pas inventer de résultat.
- Pour les changements de documentation/configuration, vérifier les formats concernés et examiner le diff. Décrire toute validation impossible.
- Lire `docs/ai/BUILD.md` et `docs/ai/TESTING.md` avant de choisir les validations du projet.
