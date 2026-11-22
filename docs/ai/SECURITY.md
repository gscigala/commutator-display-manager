# Sécurité

Les entrées externes identifiables comprennent les propriétés D-Bus des services Commutator, les données XML lues par pugixml, la configuration JSON et des ressources images chargées par OpenCV. Les callbacks et la supervision utilisent Boost.Signals2, Boost.Asio et un thread.

Lors d’un audit, examiner validation des données, tailles et indices de buffers, durées de vie capturées par callbacks, synchronisation, gestion des erreurs et versions des dépendances système. Chercher les preuves dans le code et classer les constats par gravité, portée et exploitabilité ; séparer les risques supposés des défauts démontrés.

### Points de contrôle observés le 2026-11-22

Une revue statique a relevé des risques à confirmer, sans démontrer d’exploit : `src/Widget.cpp` indexe une image selon des dimensions demandées sans vérifier d’abord les dimensions réelles ; le drapeau `running` de `src/main.cpp` est partagé avec le gestionnaire de signaux ; les propriétés D-Bus sont converties puis analysées dans les callbacks sans validation complète des types/erreurs ; certains champs de configuration JSON sont utilisés sans valider tous leurs types. Voir les emplacements dans les fichiers cités et confirmer chaque constat avant correction. La gravité dépend du contrôle des ressources/configuration et de la confiance des services D-Bus.

Ne pas modifier un composant durant une revue indépendante. Pour corriger, limiter le patch au problème établi et demander une vérification distincte. Le dépôt ne fournit pas actuellement de manifeste de dépendances verrouillées ou d’outil de scan dans les fichiers examinés.
