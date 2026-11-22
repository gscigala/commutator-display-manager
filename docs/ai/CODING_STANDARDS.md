# Standards de code

- Le standard effectif est C++20 (et C17 pour le code C du pilote), défini dans `meson.build`.
- Suivre le style local des fichiers voisins ; ne pas formater massivement des fichiers hors périmètre.
- Utiliser RAII et les types de propriété adaptés ; préférer les valeurs et les références quand elles suffisent. Justifier les pointeurs intelligents selon le partage ou le transfert de propriété.
- Respecter `const`, les durées de vie, les erreurs et les exceptions selon les conventions du module.
- Éviter les comportements indéfinis, les accès concurrents non synchronisés et les conversions risquées.
- Préserver interfaces publiques, contrats D-Bus et compatibilité de la configuration sans demande de changement.
- Réduire le couplage, éviter la duplication et garder les modifications ciblées.
- Le build active `-Werror` pour C++ : ne pas masquer un avertissement sans justification vérifiable.
