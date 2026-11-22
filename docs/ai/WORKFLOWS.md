# Workflows de travail

## A — Nouvelle fonctionnalité

1. Codex clarifie le comportement attendu et inspecte les conventions existantes.
2. `cpp-architect` analyse les fichiers, dépendances et interfaces touchés ; il recommande sans modifier le métier.
3. Codex fixe le périmètre et attribue les fichiers. L’implémentation est faite par `cpp-developer`.
4. `cpp-tester` examine les cas limites et les moyens de validation existants.
5. `cpp-reviewer` contrôle le diff en lecture seule ; Codex consolide, traite les désaccords et rapporte validations et limites.

## B — Correction de bug

1. Reproduire le problème et conserver les étapes/données utiles.
2. Localiser la cause dans le code ; solliciter une analyse indépendante si plusieurs hypothèses persistent.
3. Attribuer le patch ciblé à `cpp-developer` et ajouter une non-régression si le projet permet de la tester.
4. Compiler et exécuter les vérifications applicables ; faire relire le diff en lecture seule.
5. Rapporter la cause, le correctif et les résultats exacts.

## C — Audit sécurité

1. Demander à `cpp-security` une analyse lecture seule des composants sensibles.
2. Vérifier chaque preuve et classer gravité, portée et exploitabilité ; lister les fichiers concernés.
3. Proposer les corrections, puis les implémenter uniquement dans une tâche distincte attribuée.
4. Faire vérifier les corrections par un agent indépendant et lancer les validations pertinentes.

## D — Optimisation

1. Mesurer la référence sur une charge représentative et un matériel décrit.
2. Faire identifier les goulots par `cpp-performance` et comparer plusieurs options avec leurs impacts.
3. Implémenter seulement une optimisation étayée par des mesures, en attribuant clairement les fichiers.
4. Refaire les mesures dans les mêmes conditions, puis vérifier tests, rendu et régressions.

## E — Revue de code

1. Fournir le même diff et un périmètre précis à des analyses indépendantes qualité, sécurité, tests et performances.
2. Maintenir ces rôles en lecture seule ; aucun agent ne modifie les fichiers examinés.
3. Codex consolide les constats par gravité, indique les désaccords sans les effacer et distingue faits et hypothèses.

## Orchestration

Paralléliser les analyses indépendantes, pas les changements concurrents sur les mêmes fichiers. Pour chaque délégation, donner contexte, question, fichiers en lecture/écriture, livrable attendu et validation. Le coordinateur attend les résultats nécessaires, tranche les divergences avec leurs preuves et reste responsable de la revue finale. Un travail d’agent n’est pas validé avant vérification.
