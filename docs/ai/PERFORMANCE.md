# Performance

Les chemins susceptibles d’être coûteux incluent la composition d’images via OpenCV, le décodage/chargement de ressources et les rafraîchissements de l’écran e-paper. Les redessins écrivent aussi des PNG de diagnostic sous `/tmp` : `Supervisor::redraw()` produit une image de 296×160 pixels et les widgets en produisent de 147×79 (`src/Supervisor.cpp`, `src/Widget.cpp`). Le parcours des pixels et l’I/O peuvent être mesurés séparément. Leur coût réel dépend du matériel et de la fréquence de rafraîchissement.

Avant une optimisation : définir la mesure (durée, CPU, mémoire ou allocations), le matériel, la charge représentative et une référence reproductible. Comparer avant/après et vérifier le rendu ainsi que les limites matérielles. Une intuition ou un microbenchmark isolé ne constitue pas une preuve de gain en usage réel.

Aucun outil de profiling ni benchmark n’est configuré dans les fichiers examinés. Choisir et documenter un outil seulement si la tâche le nécessite.
