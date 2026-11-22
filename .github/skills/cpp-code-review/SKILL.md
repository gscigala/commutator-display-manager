---
name: cpp-code-review
description: Review a C or C++ change for correctness, regressions, security, tests, maintainability, and measured performance. Use when asked to review a diff or patch.
---

# Revue de code C++

1. Lire `AGENTS.md` et les documents de `docs/ai/` pertinents ; établir le périmètre du diff sans le modifier.
2. Examiner chaque changement dans son contexte d’appel, ses durées de vie, ses erreurs, ses contrats publics et ses entrées externes.
3. Évaluer indépendamment qualité, sécurité, tests/régressions et performances. Pour la performance, demander des mesures avant de conclure à un gain ou à une régression.
4. Rapporter d’abord les défauts actionnables, avec gravité, emplacement précis et conséquence. Séparer faits prouvés, risques conditionnels et questions.
5. Vérifier les tests/build pertinents uniquement s’ils existent et si l’environnement le permet. Indiquer les commandes réellement exécutées et les limites.
6. En lecture seule, ne pas corriger les fichiers examinés. Préserver les désaccords entre analyses dans la synthèse.
