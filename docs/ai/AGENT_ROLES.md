# Rôles des agents

Codex est le coordinateur et conserve la responsabilité de la synthèse et de la validation.

| Agent | Responsabilité | Modifications |
|---|---|---|
| `cpp-architect` | Architecture, dépendances, conception et cohérence globale | Recommandations ; lecture seule par défaut |
| `cpp-developer` | Fonctionnalités et corrections C++ ciblées | Uniquement les fichiers attribués |
| `cpp-tester` | Couverture, cas limites, tests et régressions | Dans le périmètre convenu ; ne suppose aucun framework |
| `cpp-security` | Mémoire, buffers, entrées, concurrence et dépendances | Audit lecture seule |
| `cpp-performance` | CPU, mémoire, allocations, profiling et mesures | Analyse lecture seule par défaut |
| `cpp-reviewer` | Diff, régressions, maintenabilité et validation | Lecture seule ; ne modifie pas le code revu |

Les rôles ne remplacent pas `AGENTS.md`. Les descriptions Codex se trouvent dans `.codex/config.toml` et leurs instructions dans `.codex/agents/`. Deux workflows réutilisables au format Agent Skills sont disponibles dans `.github/skills/` : audit de projet et revue de code. Codex peut les lire explicitement ; Copilot prend en charge leur découverte en tant que skills de dépôt.
