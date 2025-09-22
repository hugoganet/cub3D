# cub3D – Stratégie d’implémentation (macOS ↔ Linux)

Ce document décrit l’approche retenue pour le projet cub3D, les choix structurants, et les adaptations prévues pour travailler à la fois sur macOS (MiniLibX OpenGL) et Linux (MiniLibX X11). L’objectif est d’obtenir un socle propre, testable et portable dès le début, avec une progression incrémentale vers le rendu texturé.

## 1) Architecture générale

- Modules séparés (compatibles Norme):
  - parser/ – lecture et validation du `.cub` (textures, couleurs, map, position du joueur)
  - render/ – rendu (raycasting + DDA, texturage, dessin dans un buffer image)
  - input/ – gestion des touches (flags appuyé/relâché) et fermeture de fenêtre
  - utils/ – erreurs, mémoire, petites maths, conversions
  - includes/ – headers par sous-système + `cub3d.h` pour les structures centrales
- Boucle principale minimale:
  - hook clavier (KeyPress/KeyRelease) → met à jour `t_keys`
  - loop hook → applique les mouvements/rotations, calcule et dessine la frame, affiche l’image
- Données clés:
  - `t_app`: `mlx`, `win`, `frame` (image offscreen), textures, map, joueur, couleurs, `keys`, vitesses
  - `t_player`: `pos`, `dir`, `plane` (plan caméra ⟂ à dir, échelle du FOV)
  - `t_map`: grille paddée rectangulaire (chars), largeur/hauteur
  - `t_textures`: 4 textures murs (NO/SO/WE/EA) sous forme d’images MLX + pointeurs pixels

Pourquoi ce découpage?

- Lisibilité et évolutivité (tests unitaires ponctuels sur parser/validate, etc.)
- Respect de la Norme (fonctions courtes; responsabilités nettes)
- Gestion mémoire plus sûre (chaque module libère ses ressources)

## 2) Rendu: camera plane + DDA (style LodeV)

- Pour chaque colonne écran x:
  - `cameraX ∈ [-1,1]`, `rayDir = dir + plane * cameraX`
  - DDA sur la grille: `deltaDistX/Y`, `sideDistX/Y`, `stepX/Y` jusqu’à toucher ‘1’
  - distance perpendiculaire (corrige le fish-eye)
  - hauteur de bande et coordonnée X de texture; échantillonnage vertical
- Sol/plafond:
  - Mandatory: couleurs unies (rapide, simple)
  - Bonus possible: floor casting texturé (plus tard, optionnel)
- Double buffering:
  - Rendu dans `frame` (image offscreen) → `mlx_put_image_to_window` 1×/frame

Avantages:

- Pas de trig dans la boucle interne (sin/cos uniquement pour rotations)
- Performant et stable; facile à texturer

## 3) Entrées et mouvements

- Flags de touches (ZQSD/WASD + flèches):
  - `KeyPress` → flag = true; `KeyRelease` → flag = false
  - boucle → lit les flags et applique déplacement/rotation
- Mouvements:
  - avant/arrière: `pos += dir * moveSpeed`
  - strafe: `pos += rot90(dir) * moveSpeed`
  - rotation: rotation de `dir` et `plane` (matrice 2D via cos/sin)
- Collision:
  - Simple (grid-based) au début: on bloque si la cellule visée est un mur
  - “Hitbox” légère ensuite: tester quelques offsets autour de la position cible

## 4) Parsing et validation `.cub`

- En-têtes: chemins XPM `NO/SO/WE/EA`, couleurs `F/C` en `R,G,B`
- Map: lignes contiguës en fin de fichier, exactement 1 joueur `N/S/E/W`
- Validation:
  - Caractères autorisés uniquement
  - Map fermée: flood fill depuis le joueur (ou vérifs d’adjacence) avec padding
  - Normaliser: pad rectangulaire avec espaces pour éviter OOB
- Erreurs:
  - Préfixe `Error` et message explicite
  - Libérer proprement tout ce qui a été alloué avant de sortir

## 5) Cross-plateforme: macOS et Linux

- macOS (début du dev):
  - MiniLibX OpenGL (frameworks AppKit/OpenGL)
  - Pixels: ARGB32; penser à mettre alpha = 0xFF pour voir les couleurs (fait dans `rgb_to_int`)
- Linux (bientôt: VM + Valgrind):
  - MiniLibX X11 (libXext, libX11, libm, zlib)
  - Il faudra adapter le Makefile:
    - Remplacer frameworks par `-lmlx -lXext -lX11 -lm -lz` et inclure `-I/usr/include` / `-L/usr/lib` selon l’install
    - Option: garder deux branches de Makefile (macOS/Linux) basculées via une variable d’environnement (`OS=linux`)
  - Valgrind:
    - Utile pour traquer les fuites dans parser, textures, map, images MLX
    - Attention: certaines libs graphiques peuvent bruiter la sortie; focus sur vos allocations (map, strings, textures)

Conseil: isoler toute la gestion MLX (création image, `mlx_get_data_addr`, destruction) dans des fonctions dédiées pour limiter les différences entre plateformes.

## 6) Roadmap incrémentale

1. Boot minimal (fait): fenêtre, fond ciel/sol, boucle, entrées basiques
2. Parser `.cub` (à faire):
   - En-têtes (textures/couleurs)
   - Map (collecte, pad, validation), position + direction/plane
3. Raycasting sans textures (à faire):
   - DDA, distance perpendiculaire, murs en couleurs unies par côté
4. Texturage murs (à faire):
   - `wallX`, `texX`, pas vertical; chargement XPM; sampling `addr/bpp/line_len`
5. Collision (à faire): grid + petite hitbox
6. Linux/Valgrind (à prévoir tôt):
   - Adapter Makefile Linux, tester `make`, puis valgrind sur parsing et fermeture propre

## 7) Gestion des ressources et erreurs

- Séquence d’init:
  - `mlx_init` → `mlx_new_window` → `mlx_new_image`
  - textures (une par une), parser map
- En cas d’erreur:
  - Détruire images déjà créées, fenêtre, libérer map/strings
  - Retourner un code d’erreur et un message clair
- À la sortie:
  - `mlx_destroy_image` pour chaque image
  - `mlx_destroy_window` (et `mlx_destroy_display` sur Linux)
  - `free` sur map et structures

## 8) Décisions actées dans ce dépôt

- MiniLibX: dossier local `minilibx_opengl` (OpenGL), intégré au build
- Makefile: pas de backend Metal; frameworks AppKit/OpenGL; une seule `put_image_to_window` par frame
- Couleurs: alpha forcé à 0xFF pour l'affichage correct sur macOS
- Skeleton: modules, stubs parser/raycast prêts à implémenter
- Libft: intégrée directement dans le dépôt (dossier `libft`) pour simplifier la gestion du projet

## 9) Étapes suivantes (recommandées)

- Implémenter le parser (headers → map → validation) + erreurs propres
- Écrire un test map minimal (déjà `maps/sample.cub`) et quelques maps de bords (trous, espaces, spawns multiples)
- Implémenter DDA + murs plats, puis texturage
- Préparer Makefile Linux et vérification Valgrind (dès que parser est en place)

---

Ce plan permet de livrer rapidement un rendu minimal et d’itérer en sécurité, tout en garantissant la portabilité macOS ↔ Linux et la compatibilité avec Valgrind dès que possible.
