# Décisions d’architecture et d’infrastructure

## Registre

### 2026-11-22 — `AGENTS.md` comme contrat commun

- **Contexte :** plusieurs assistants doivent appliquer les mêmes règles de projet, sans maintenir des copies divergentes.
- **Décision :** placer les règles communes dans `AGENTS.md`; les configurations propres aux outils ne contiennent que la découverte, les rôles ou un renvoi. Les détails du projet vivent dans les documents `docs/ai/`.
- **Alternatives considérées :** recopier les règles dans chaque fichier d’outil (risque de divergence) ou n’avoir qu’un document général sans documentation spécialisée (moins opérable).
- **Conséquences :** chaque outil doit explicitement découvrir ou référencer le contrat commun ; les règles ne sont pas dupliquées.

### 2026-11-22 — Rôles analytiques en lecture seule

- **Contexte :** les audits et revues doivent rester indépendants de l’implémentation, et aucun agent ne doit modifier simultanément les mêmes fichiers.
- **Décision :** architecte, sécurité et reviewer sont en sandbox lecture seule ; performance est lecture seule par défaut. Codex limite à trois le nombre d’agents secondaires simultanés.
- **Alternatives considérées :** permettre l’écriture à tous les rôles (risque de conflit et de revue biaisée) ou désactiver la délégation (perte d’analyses indépendantes).
- **Conséquences :** les patchs sont attribués au développeur et Codex coordonne/consolide ; les analyses peuvent être parallélisées.

### 2026-11-22 — Respect des choix de modèle et de sécurité de l’environnement

- **Contexte :** les modèles disponibles, politiques d’approbation et capacités sandbox dépendent de l’installation de l’utilisateur.
- **Décision :** aucun modèle n’est imposé ; aucune politique d’approbation ou sandbox permissive n’est définie au niveau du dépôt.
- **Alternatives considérées :** sélectionner un modèle fixe ou fournir une politique locale par défaut (risque d’incompatibilité ou d’affaiblissement des protections).
- **Conséquences :** Codex conserve les paramètres utilisateur/système et applique uniquement les rôles locaux lorsqu’il fait confiance au dépôt.

## Format des décisions futures

Ajouter une entrée avec date, décision, contexte, alternatives considérées et conséquences (y compris coûts ou limites). Ne consigner que les choix durables qui affectent architecture, interfaces, dépendances ou workflow ; le registre ci-dessus sera complété si les alternatives sont examinées ultérieurement.
