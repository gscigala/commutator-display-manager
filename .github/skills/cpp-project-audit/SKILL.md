---
name: cpp-project-audit
description: Audit a C or C++ repository before proposing infrastructure or code changes. Use when asked to onboard, map, or document an unfamiliar C++ project.
---

# Audit de projet C++

1. Lire `AGENTS.md` et vérifier l’état Git avant d’explorer. Ne modifier aucun fichier pendant l’audit.
2. Examiner l’arborescence, l’historique pertinent, les instructions IA, le build, le standard C/C++, les dépendances, les exécutables, modules et tests.
3. Lire les fichiers d’entrée et les interfaces pour relier chaque module à une responsabilité observée. Distinguer les faits des hypothèses.
4. Vérifier les contraintes de plateforme, sous-modules et ressources externes ; signaler les limites qui empêchent une compilation complète.
5. Présenter l’architecture, les conventions, les risques connus dans les sources, les validations possibles et un plan avant toute installation demandant une approbation préalable.
6. Ne pas déduire de dépendances, commandes ou composants absents des fichiers. Citer les chemins examinés et préserver les changements utilisateur.

Consulter `docs/ai/ARCHITECTURE.md`, `BUILD.md` et `TESTING.md` pour le contexte établi de ce dépôt.
