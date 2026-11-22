# Tests et validation

À l’audit initial, aucun fichier/répertoire de tests ni cible `test()` Meson n’a été trouvé. Ne pas annoncer l’exécution d’une suite inexistante. Si un changement justifie l’ajout de tests, examiner d’abord les possibilités déjà installées et proposer le minimum adapté au périmètre.

Pour une modification de code, utiliser les commandes de build de `BUILD.md` lorsque les dépendances et sources externes sont disponibles, puis vérifier les warnings et les régressions pertinentes. Les fonctionnalités liées à D-Bus ou au matériel peuvent nécessiter un système cible ; préciser ce qui a effectivement été exercé.

Pour des fichiers de configuration ou documentation, vérifier leur syntaxe, les liens et la cohérence des chemins. Rapporter les commandes réellement exécutées et les validations impossibles, avec leur cause.
