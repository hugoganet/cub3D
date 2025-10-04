# Parcours d'Apprentissage cub3D - Hugo

## 🎯 Focus Actuel

Niveau 6 - Minimap - COMPLÉTÉ ✅ (100%)
Date : 2025-10-04

## ✅ Concepts Maîtrisés

### 🏗️ Niveau 1 : Fondations - MAÎTRISÉ ✅

**Date** : 2025-10-01

### 📄 Niveau 2 : Parsing & Validation - MAÎTRISÉ ✅

**Date** : 2025-10-03 (complété)

### 🎮 Niveau 3 : Raycasting & DDA - MAÎTRISÉ ✅

**Date** : 2025-10-04 (complété)

### 🎨 Niveau 4 : Texture Mapping - MAÎTRISÉ ✅

**Date** : 2025-10-04 (complété)

### 🎮 Niveau 5 : Mouvement & Collision - MAÎTRISÉ ✅

**Date** : 2025-10-04 (complété)

### 🗺️ Niveau 6 : Minimap - MAÎTRISÉ ✅

**Date** : 2025-10-04 (complété)

**Étapes parsing** :

- ✅ Grandes phases : open() → get_next_line() loop → parser ligne → valider données → stocker dans t_app
- ✅ 3 types de lignes : textures (NO/SO/WE/EA), couleurs (F/C), map (0/1/N/S/E/W/ )
- ✅ Gestion lignes vides (ignorées avec `continue`)

**Architecture modulaire (11 fichiers)** :

- ✅ SOC appliqué : `parse_tex.c`, `parse_color.c`, `parse_map.c`, `validate_*.c`
- ✅ Chef d'orchestre : `parse_input.c:parsing()` → `parse_cub_file()` → `validate_map()`
- ✅ Avantages : responsabilités nettes, fonctions < 25 lignes (Norme), testable

**Validation entrée** (`parse_input.c:88-109`) :

- ✅ argc == 2, longueur >= 5 (min: `a.cub`), extension `.cub`
- ✅ `ft_memset(app, 0)` avant init → sécurité (pointeurs NULL, pas de garbage)

**Compteurs & ordre flexible** (`t_parse_counters`) :

- ✅ `texture_count`, `color_count`, `map_started`, `map_line_index`
- ✅ Ordre textures/couleurs libre (sujet ligne 124), mais map toujours en dernier
- ✅ Check `!counters->map_started` → rejette texture/couleur après début map

**Validation finale** (`validate_parsing_completion()`) :

- ✅ Vérifie `texture_count == 4` (détecte doublons si count > 4)
- ✅ Vérifie `color_count == 2`, `map_started == 1`

**🐛 Bugs détectés et corrigés** :

- ✅ Memory leak doublons textures : ajout vérif `if (path != NULL)` dans `parse_tex.c` avant assignation
- ✅ RGB invalides acceptés : `ft_atoi("abc")` retournait 0 au lieu d'erreur
  - Ajout `is_valid_number()` pour valider strings avant conversion
  - Refactorisation `parse_rgb_values()` (18 lignes, < 25) avec `validate_and_convert_rgb()`
  - Maintenant rejette `F abc,100,0` ou `F -50,100,0` correctement

**Parse de couleurs** (`parse_color.c`) :

- ✅ Flow : `parse_color_line()` → `extract_rgb_string()` → `parse_rgb_values()` → `validate_and_convert_rgb()`
- ✅ `extract_rgb_string()` : skip "F"/"C" + espaces, extrait "R,G,B"
- ✅ `ft_split(rgb_str, ',')` → tableau ["R", "G", "B"]
- ✅ Validation : exactement 3 valeurs, que des chiffres, range 0-255
- ✅ Stockage dans `t_color` (r, g, b) puis assignation `app->floor` ou `app->ceil`

**✅ Gestion d'erreurs refactorisée** :

- **Problème initial** : 3 patterns différents (`return -1`, `return 1`, `error_exit()` direct)
- **Solution implémentée** : Fonction `error_msg()` helper dans `errors.c`
  - Affiche "Error" + message spécifique sur stderr
  - Retourne `-1` (pattern uniforme)
  - Pas besoin de passer `app` partout
- **Résultat** : Tous les fichiers parser uniformisés avec `return (error_msg("..."))`

**Architecture générale** :

- ✅ Flow complet du programme : parsing → init → hooks → loop → destroy
- ✅ Structure `t_app` et son contenu (mlx, win, frame, tex, map, player, colors, keys, speeds)
- ✅ Séparation parsing/init : données vs ressources MLX

**Boucle de jeu** :

- ✅ `mlx_loop_hook` : appelée ~60 FPS en continu
- ✅ `mlx_hook` event 2/3 : key_press/release (quand événement se produit)
- ✅ `mlx_hook` event 17 : close_window (croix rouge)
- ✅ Distinction actions directes vs flags (ESC vs WASD)

**Pipeline `app_loop()`** :

- ✅ update_player_movement() → lecture des flags
- ✅ fill_background() → effacer frame
- ✅ render_background() → plafond/sol
- ✅ render_3d_view() → raycasting + murs
- ✅ render_minimap() → overlay 2D
- ✅ mlx_put_image_to_window() → affichage final

**Gestion des inputs** :

- ✅ Événements asynchrones → flags booléens
- ✅ Game loop synchrone → lecture des flags
- ✅ Avantage : mouvements combinés (diagonales)

**Cleanup** :

- ✅ `close_window()` → `app_destroy()` → `exit(0)`
- ✅ Séquence de destruction des ressources MLX
- ✅ Code mort après `mlx_loop()` (jamais atteint)

**MLX Loop vs Loop Hook** :

- ✅ `mlx_loop_hook(mlx, func, param)` : **enregistre** une fonction à appeler en continu (comme donner une recette au chef)
- ✅ `mlx_loop(mlx)` : **démarre** la boucle d'événements infinie (comme ouvrir le restaurant)
- ✅ Différence clé : loop_hook = instructions, loop = exécution
- ✅ Sans loop_hook : fenêtre s'ouvre mais reste noire (aucun rendu)
- ✅ `exit(0)` : fonction libc (stdlib.h) qui **tue le processus entier**, pas une fonction MLX
- ✅ Pourquoi ligne 45 main.c jamais atteinte : `close_window()` appelle `exit(0)` → processus terminé avant retour de `mlx_loop()`

**Parsing de la map** (`parse_map.c`) :

- ✅ Allocation dynamique ligne par ligne (pas de taille fixe)
- ✅ `ensure_grid_capacity()` : alloue nouveau tableau, copie anciennes lignes, libère ancien (remplace realloc)
- ✅ `add_map_line()` : trim \n, duplique ligne, réalloue grid, ajoute ligne + NULL terminator
- ✅ `grid[line_index + 2]` : +1 pour nouvelle ligne, +1 pour NULL terminator
- ✅ Détection début map : `is_map_line()` vérifie que tous les caractères sont valides (0/1/ /N/S/E/W/\n)
- ✅ Flag `map_started` comme verrou unidirectionnel : une fois la map commencée, plus de textures/couleurs acceptées
- ✅ Lignes vides rejetées si `map_started == 1` (évite trous dans la map)

**Validation des caractères** (`validate_chars.c`) :

- ✅ Double vérification `app->map.grid[i] && app->map.grid[i][j]` pour éviter segfault (NULL check)
- ✅ Parcours complet de la grille avec boucles imbriquées
- ✅ Rejet de tout caractère non autorisé (uniquement 0/1/ /N/S/E/W acceptés)

**Validation du joueur** (`validate_player.c` & `find_player.c`) :

- ✅ `find_player()` : parcourt map, compte les joueurs avec `player_count`
- ✅ Erreur si `player_count == 0` (pas de joueur) ou `player_count > 1` (plusieurs joueurs)
- ✅ `process_cell_for_player()` : si caractère joueur trouvé → incrémente compteur + initialise position
- ✅ Position centrée : `pos.x = (double)j + 0.5` et `pos.y = (double)i + 0.5` (centre de la cellule, pas le coin)
- ✅ Vecteurs dir et plane perpendiculaires :
  - `dir` : direction du regard (centre de l'écran)
  - `plane` : largeur du champ de vision (écran plat devant le joueur)
  - Magnitude `plane = 0.66` → FOV ~66°
  - Exemple Nord : `dir=(0,-1)`, `plane=(0.66,0)` (plane perpendiculaire à dir)

**Validation map fermée** (`check_map_closed.c`) :

- ✅ `check_map_closed()` : parcourt toutes les cellules marchables (0 ou joueur)
- ✅ `check_neighbors_of()` : pour chaque cellule marchable, vérifie les 4 voisins (haut/bas/gauche/droite)
- ✅ `get_map_char_or_space()` : retourne caractère à position (x,y), ou `' '` si hors limites
- ✅ Logique : hors map = vide, espace dans map = vide → traités pareil
- ✅ Rejet si case marchable adjacente à espace `' '` (empêche joueur de "sortir")
- ✅ Exemple invalide : `10 1` → le 0 est adjacent à un espace (trou vers l'extérieur)

**Concept du Raycasting** (`raycasting.c`) :

- ✅ Principe : un rayon par colonne d'écran (800 colonnes = 800 rayons)
- ✅ Calcul direction rayon : `camera_x = 2 * x / win_w - 1` (de -1 à +1)
- ✅ Formule : `ray_dir = dir + plane * camera_x`
  - Centre (camera_x=0) : rayon = dir (tout droit)
  - Gauche (camera_x=-1) : rayon = dir - plane
  - Droite (camera_x=+1) : rayon = dir + plane
- ✅ Le vecteur plane permet de "balayer" le champ de vision de gauche à droite

**Algorithme DDA** (`dda.c`) :

- ✅ But : suivre le rayon à travers la grille case par case jusqu'à toucher un mur
- ✅ `delta_dist_x/y` : distance que le rayon doit parcourir pour traverser une cellule complète (formule `1 / ray_dir.x`)
- ✅ `side_dist_x/y` : distance jusqu'à la prochaine ligne de grille
- ✅ Boucle principale : compare `side_dist_x` et `side_dist_y`, avance sur l'axe le plus proche
- ✅ Similaire à l'algorithme de Bresenham (tracé de ligne sur grille discrète)
- ✅ `step_x/y` : direction du pas (-1 ou +1)
- ✅ `side` : 0 = collision verticale (X), 1 = collision horizontale (Y)

**Distance Perpendiculaire** (`dda.c:fill_hit_info()`) :

- ✅ Problème : distance euclidienne crée effet fish-eye (murs courbés)
- ✅ Solution : mesurer distance perpendiculaire au plan caméra, pas depuis le joueur
- ✅ Calcul selon side : `(map_x - pos.x + offset) / ray_dir.x` ou `(map_y - pos.y + offset) / ray_dir.y`
- ✅ Résultat : tous les points d'un mur droit ont la même distance → mur paraît droit

**Projection Perspective** (`projection.c`) :

- ✅ Formule : `line_height = screen_height / perp_dist`
- ✅ Plus proche = plus grand à l'écran, plus loin = plus petit
- ✅ `calculate_wall_bounds()` : centre le mur verticalement, clamp aux limites écran
- ✅ Pipeline complet : rayon → DDA → distance perp → hauteur → dessin colonne

**Coordonnée Horizontale Texture** (`projection.c:get_texture_coord_x()`) :

- ✅ Calcul : `tex_x = wall_x × texture_width`
- ✅ `wall_x` : position exacte où le rayon touche le mur (0.0 → 1.0)
- ✅ `tex_x` : colonne de pixels dans la texture à utiliser
- ✅ Fixe pour toute la colonne verticale à l'écran
- ✅ Exemple : wall_x = 0.75 sur texture 64px → tex_x = 48

**Step et Échantillonnage Vertical** (`projection.c:init_texture_vars()`) :

- ✅ Formule : `step = texture_height / line_height`
- ✅ `step` : combien de pixels texture on avance par pixel écran
- ✅ Si texture plus petite que mur écran → step < 1.0 (avance lentement)
- ✅ Si texture plus grande que mur écran → step > 1.0 (saute des pixels)
- ✅ Exemple : texture 64px, mur écran 128px → step = 0.5

**Position Verticale Texture** (`projection.c:draw_textured_wall_column()`) :

- ✅ `tex_pos` (double) : position continue dans la texture (peut être 2.75, 15.3...)
- ✅ `tex_y` (int) : pixel texture entier, c'est `tex_pos` arrondi : `tex_y = (int)tex_pos`
- ✅ Boucle de dessin : pour chaque pixel écran Y, calcule tex_y, récupère couleur, puis `tex_pos += step`
- ✅ Gestion clipping : `tex_pos = (draw_start - screen_h/2 + line_height/2) × step`
  - Si haut du mur hors écran, commence au milieu de la texture (pas à tex_y = 0)
  - Gère correctement les murs très proches qui dépassent l'écran

**Pipeline Texture Mapping Complet** :

- ✅ 1. Sélection texture selon face mur (N/S/E/W)
- ✅ 2. Calcul tex_x (horizontal, fixe pour la colonne)
- ✅ 3. Calcul step (vitesse d'avancement vertical)
- ✅ 4. Init tex_pos (position de départ, gère clipping)
- ✅ 5. Boucle : tex_y = (int)tex_pos → récupère couleur → dessine → tex_pos += step

**Mouvement Avant/Arrière** (`movement.c:move_player()`) :

- ✅ Formule : `new_pos = pos + (dir × move_speed)`
- ✅ Touche W : avance dans direction dir (+dir)
- ✅ Touche S : recule dans direction opposée (-dir)
- ✅ Exemple : pos=(5.5, 3.2), dir=(0, -1), speed=0.1 → new_pos=(5.5, 3.1)
- ✅ Le vecteur dir détermine la direction du mouvement

**Mouvement Latéral (Strafe)** (`movement.c:apply_movement()`) :

- ✅ Strafe = mouvement perpendiculaire à la direction du regard
- ✅ Strafe gauche (A) : `(dir.y, -dir.x)` rotation -90°
- ✅ Strafe droite (D) : `(-dir.y, dir.x)` rotation +90°
- ✅ Exemple : dir=(0, -1) nord → strafe_gauche=(-1, 0) ouest
- ✅ Permet déplacements diagonaux si plusieurs touches pressées

**Rotation Caméra** (`movement.c:rotate_player()`) :

- ✅ Matrice de rotation 2D classique : angle = rot_speed × direction
- ✅ Formule : `new.x = old.x × cos(θ) - old.y × sin(θ)`
- ✅ Formule : `new.y = old.x × sin(θ) + old.y × cos(θ)`
- ✅ Application sur dir ET plane (doivent rester perpendiculaires)
- ✅ Flèche droite : direction=1 (rotation horaire)
- ✅ Flèche gauche : direction=-1 (rotation anti-horaire)
- ✅ Rotation continue : nord → est → sud → ouest → nord...

**Détection de Collision** (`movement.c:is_valid_position()`) :

- ✅ Vérifie si position (x,y) est traversable avant déplacement
- ✅ Conditions : dans limites grille, ligne existe, caractère ≠ '1' (mur)
- ✅ Test axe par axe indépendamment (X puis Y séparément)
- ✅ **Wall Sliding** : si diagonal bloqué, permet mouvement sur un seul axe
- ✅ Exemple : mouvement (3.1, 2.9) avec mur en X → glisse vers (2.8, 2.9)
- ✅ Empêche blocage total contre les murs en diagonal

**Pipeline Mouvement Complet** :

- ✅ 1. Lecture flags clavier (W/A/S/D/←/→)
- ✅ 2. Calcul vecteur déplacement (apply_movement)
- ✅ 3. Application rotation si flèches (rotate_player)
- ✅ 4. Calcul nouvelles positions X et Y
- ✅ 5. Test collision axe par axe (is_valid_position)
- ✅ 6. Mise à jour pos/dir/plane si valide

**Système de Coordonnées Minimap** (`minimap.c:draw_minimap_tile()`) :

- ✅ Conversion map → écran : `screen = offset + (map_pos × scale)`
- ✅ `MINIMAP_OFFSET_X/Y` = 25 pixels (marge depuis bord écran)
- ✅ `MINIMAP_SCALE` = 10 (1 case map = 10 pixels écran)
- ✅ Exemple : map(3,5) → screen(25 + 3×10, 25 + 5×10) = (55, 75)
- ✅ Chaque case de map devient un carré 10×10 pixels

**Code Couleurs Minimap** (`minimap_utils.c:get_tile_color()`) :

- ✅ '1' (mur) → 0xFFFFFF (blanc)
- ✅ '0' (sol) → 0x404040 (gris foncé)
- ✅ 'N/S/E/W' (spawn) → 0x0000FF (bleu)
- ✅ ' ' (vide hors map) → 0x000000 (noir)
- ✅ autre (erreur) → 0xFF00FF (magenta)
- ✅ Permet identification visuelle rapide des types de cases

**Indicateur Joueur** (`minimap.c:draw_player_on_minimap()`) :

- ✅ Carré rouge 6×6 pixels centré sur position joueur
- ✅ Calcul : `screen = offset + (pos × scale) - 3` (centrage)
- ✅ Point vert 2×2 pixels pour direction du regard
- ✅ Position point : `center + (dir × 10)` (10 pixels dans direction)
- ✅ 20 rayons verts affichent FOV complet (~60°)
- ✅ Visualisation complète : position + orientation + champ de vision

**Pipeline Rendu Overlay** (`loop.c:app_loop()`) :

- ✅ 1. update_player_movement() - mouvement
- ✅ 2. fill_background() - efface écran
- ✅ 3. render_background() - plafond/sol
- ✅ 4. render_3d_view() - vue 3D complète
- ✅ 5. render_minimap() - overlay par-dessus 3D
  - Grille de tuiles colorées
  - Bordure blanche
  - 20 rayons verts (FOV)
  - Joueur rouge + direction verte
- ✅ 6. mlx_put_image_to_window() - affichage final

**Painter's Algorithm** :

- ✅ Ordre de dessin = ordre de profondeur (Z-order)
- ✅ Dessiné en dernier = visible au-dessus (overlay)
- ✅ Minimap DOIT être après vue 3D sinon cachée
- ✅ Principe : comme peintre qui peint couches successives

## 🔄 Zones Nécessitant un Renforcement

### Session 1 - Niveau 1 (2025-10-01)

- ⚠️ Confusion `mlx_loop_hook` appelée 1× vs ~60 FPS → résolu
- ⚠️ Oubli map dans données `parsing()` → résolu (pointé vers `t_map`)
- ✅ Bonne intuition : `app_init()` nécessite pointeur `mlx`

### Session 2 - Niveau 2 (2025-10-01)

- ⚠️ Oubli initial get_next_line() pour lire `.cub` (pensait open() direct sur textures) → corrigé
- ⚠️ Questionnement légitime sur vérif `strlen >= 5` (validé : `a.cub` = 5 chars min)
- ✅ Excellente détection bug memory leak doublons textures
- ✅ Compréhension SOC et responsabilités des 11 fichiers parser

### Session 3 - Niveau 1 : MLX Loop (2025-10-02)

- ⚠️ Confusion initiale sur code mort ligne 45 main.c (erreur du tutor corrigée)
- ✅ Compréhension finale : `exit(0)` tue le processus → `app_destroy()` ligne 45 jamais atteinte
- ✅ Maîtrise différence `mlx_loop()` vs `mlx_loop_hook()` via métaphore restaurant
- ✅ Compréhension que `exit()` est une fonction libc, pas MLX

### Session 4 - Niveau 2 : Parse Couleurs + Gestion Erreurs (2025-10-02)

- ✅ Correction bug RGB invalides : ajout validation `is_valid_number()` avant `ft_atoi()`
- ✅ Refactorisation `parse_color.c` : extraction `validate_and_convert_rgb()` (respect Norme < 25 lignes)
- ✅ Compréhension flow parsing couleurs : extract → split → validate → convert → store
- ✅ Identification problème : gestion d'erreurs incohérente (3 patterns différents)
- ✅ Implémentation `error_msg()` helper + uniformisation complète du parser

### Session 5 - Niveau 2 : Parsing Map + Validation Complète (2025-10-03)

- ✅ Remplacement `realloc()` par allocation manuelle (malloc + copie + free) dans `ensure_grid_capacity()`
- ✅ Compréhension allocation dynamique : ligne par ligne, `grid[line_index + 2]` pour ligne + NULL terminator
- ✅ Compréhension flag `map_started` comme verrou unidirectionnel (textures/couleurs bloquées après début map)
- ✅ Compréhension position joueur centrée : `+0.5` pour centre de cellule vs coin
- ✅ Compréhension vecteurs dir/plane perpendiculaires pour FOV
- ⚠️ Confusion initiale coordonnées (x,y) : haut = y-1 (pas x-1) → résolu
- ✅ Compréhension `get_map_char_or_space()` : hors map = vide = espace
- ✅ Compréhension validation fermeture : case marchable adjacente à espace = erreur
- ✅ **Résultat** : Niveau 2 (Parsing & Validation) complété à 100%

### Session 6 - Niveau 3 : Raycasting & DDA (2025-10-04)

- ✅ Compréhension concept raycasting : un rayon par colonne d'écran
- ✅ Compréhension calcul direction rayon : camera_x de -1 à +1, ray_dir = dir + plane * camera_x
- ⚠️ Difficulté initiale avec delta_dist_x (formule abstraite) → contourné en voyant l'utilisation pratique
- ✅ Excellente intuition : connexion avec algorithme de Bresenham
- ✅ Compréhension boucle DDA : compare side_dist_x/y, avance sur axe le plus proche
- ✅ Compréhension distance perpendiculaire via schémas visuels (euclidienne vs perpendiculaire)
- ✅ Compréhension projection perspective : lineHeight = screenHeight / perpDist
- ✅ Peut résumer pipeline complet du raycasting
- ✅ **Résultat** : Niveau 3 (Raycasting & DDA) complété à 100%

### Session 6 (suite) - Niveau 4 : Texture Mapping (2025-10-04)

- ✅ Compréhension tex_x : wall_x × texture_width (position horizontale fixe)
- ✅ Calcul mental rapide : wall_x=0.75 sur texture 64px → tex_x=48 ✅
- ✅ Compréhension step : texture_h / line_height (vitesse d'avancement)
- ✅ Compréhension clipping : mur hors écran → commence au milieu de texture (pas à tex_y=0)
- ⚠️ Hésitation initiale sur différence tex_pos vs tex_y → résolu avec métaphore tapis roulant
- ✅ Compréhension finale : tex_pos = double continu, tex_y = int arrondi
- ✅ Analogie YouTube : tex_pos = curseur temps (2.75s), tex_y = numéro image (#2 ou #3)
- ✅ Peut expliquer boucle complète : tex_y = (int)tex_pos → couleur → dessine → tex_pos += step
- ✅ **Résultat** : Niveau 4 (Texture Mapping) complété à 100%

### Session 6 (suite) - Niveau 5 : Mouvement & Collision (2025-10-04)

- ✅ Compréhension mouvement avant : new_pos = pos + (dir × speed)
- ✅ Calcul mental correct : pos=(5.5,3.2), dir=(0,-1), speed=0.1 → (5.5,3.1) ✅
- ✅ Compréhension strafe gauche : rotation -90° = (dir.y, -dir.x)
- ✅ Identification correcte : dir=(0,-1) nord → strafe_gauche=(-1,0) ouest ✅
- ✅ Compréhension rotation matrice 2D : dir et plane tournent ensemble
- ✅ Compréhension rotation continue : nord → est → sud → ouest → nord
- ✅ Compréhension wall sliding : test axe par axe permet glissement
- ✅ Scenario correct : diagonal bloqué en X → glisse seulement en Y ✅
- ✅ Comprend pourquoi sans wall sliding = blocage total contre murs
- ✅ **Résultat** : Niveau 5 (Mouvement & Collision) complété à 100%

### Session 6 (suite) - Niveau 6 : Minimap (2025-10-04)

- ✅ Compréhension conversion coordonnées : screen = offset + (map × scale)
- ✅ Calcul assisté : map(3,5) avec scale=10, offset=10 → screen(40,60) ✅
- ⚠️ Confusion initiale code couleurs : '0' = gris foncé (pas noir)
- ✅ Correction immédiate : noir réservé aux espaces ' ' (vide hors map)
- ✅ Identification correcte ligne "1110001" → blanc-blanc-blanc-gris-gris-gris-blanc
- ✅ Compréhension indicateur joueur : carré 6×6 + point direction à +10px
- ✅ Calcul assisté : pos(5.5,3.2), dir(1,0) → point_vert à (75,42) ✅
- ✅ Excellente observation : remarque présence 20 rayons verts + point direction
- ✅ Compréhension Painter's Algorithm : dernier dessiné = au-dessus
- ✅ Identification correcte : minimap avant 3D → cachée (réponse B)
- ✅ **Résultat** : Niveau 6 (Minimap) complété à 100%

## 💡 Métaphores & Analogies Personnalisées

### Sessions 1-2 (2025-10-01)

**Aucune métaphore explicite utilisée** - Apprentissage par Q&A directes sur code.

### Session 3 (2025-10-02)

**Métaphore du Restaurant** :

- `mlx_loop_hook()` = Donner une recette au chef (instructions)
- `mlx_loop()` = Ouvrir le restaurant (exécution)
- Sans loop_hook : restaurant ouvert mais aucun plat préparé (fenêtre noire)
- `exit(0)` = Fermer brutalement le restaurant en éteignant les lumières

### Session 5 (2025-10-03)

**Métaphore de la Caméra (vecteurs dir/plane)** :

- `dir` = Direction du regard (centre de l'écran, où tu pointes)
- `plane` = Largeur de l'écran (écran plat perpendiculaire à dir)
- Perpendiculaires car ton écran est plat devant toi
- Magnitude `0.66` du plane = FOV ~66°

### Session 6 (2025-10-04)

**Métaphore de la Distance Perpendiculaire** :

```
❌ Distance Euclidienne (fish-eye) :
    A ═══ B ═══ C  (Mur)
    ↙     ↓     ↘
Distance A > B < C → mur courbé

✅ Distance Perpendiculaire :
    A ═══ B ═══ C  (Mur)
    |     |     |
Distance A = B = C → mur droit
```

Projection depuis le plan caméra, pas depuis le joueur

**Métaphore du Tapis Roulant** (texture mapping) :

- `tex_pos` = curseur de temps sur YouTube (peut être 2.75s)
- `tex_y` = numéro d'image affichée (image #2 ou #3, pas d'image 2.75)
- On avance `tex_pos` continuellement, on lit l'image `tex_y` correspondante
- Comme un tapis roulant 🎞️ : on déroule progressivement la texture à l'écran

**Métaphore du Peintre** (Painter's Algorithm) :

- Comme un peintre qui peint des couches successives 🎨
- La dernière couche cache les précédentes
- Vue 3D = fond de tableau, Minimap = cadre par-dessus
- Si on inverse : minimap peinte en premier → cachée par la 3D

## 📊 Préparation à l'Évaluation

### Théorie du Raycasting (5/5) : Maîtrisé ✅

- [x] Peut expliquer comment fonctionne le raycasting (un rayon/colonne, ray_dir = dir + plane * camera_x)
- [x] Peut décrire la correction du fish-eye (distance perpendiculaire vs euclidienne)
- [x] Peut expliquer le calcul de perspective (lineHeight = screenHeight / perpDist)
- [x] Peut résumer le pipeline complet (rayon → DDA → distance → hauteur)
- [x] Peut expliquer le rôle des vecteurs dir et plane

### Algorithme DDA (4/5) : Presque maîtrisé ⏳

- [x] Peut expliquer le processus DDA étape par étape (compare side_dist, avance case par case)
- [x] Peut décrire la signification de deltaDistX/Y (distance pour traverser une cellule)
- [x] Peut expliquer le rôle de sideDistX/Y (distance jusqu'à prochaine ligne de grille)
- [x] Comprend la similarité avec Bresenham
- [ ] Peut expliquer en détail les formules d'initialisation (à approfondir)

### Mapping de Textures (5/5) : Maîtrisé ✅

- [x] Peut expliquer le calcul de wall_x (position 0.0→1.0 où rayon touche mur)
- [x] Peut décrire le mapping horizontal (tex_x = wall_x × texture_width)
- [x] Peut expliquer l'échantillonnage vertical (step = texture_h / line_height)
- [x] Peut expliquer la différence tex_pos (double continu) vs tex_y (int pixel)
- [x] Peut expliquer la gestion du clipping (murs hors écran)

### Système de Mouvement (5/5) : Maîtrisé ✅

- [x] Peut expliquer le mouvement avant/arrière (new_pos = pos + dir × speed)
- [x] Peut décrire la mécanique de strafe (vecteurs perpendiculaires à dir)
- [x] Peut expliquer la rotation avec les vecteurs dir/plane (matrice rotation 2D)
- [x] Comprend les formules de rotation -90° et +90° pour strafe
- [x] Comprend pourquoi dir et plane tournent ensemble

### Détection de Collision (5/5) : Maîtrisé ✅

- [x] Peut expliquer la collision basée sur la grille (is_valid_position)
- [x] Peut décrire l'approche de validation (vérif caractère ≠ '1')
- [x] Comprend le test axe par axe indépendant
- [x] Peut expliquer le wall sliding (glissement le long des murs)
- [x] Comprend pourquoi tester X et Y séparément

### Parser (5/5) : Maîtrisé ✅

- [x] Peut expliquer les étapes de validation (open, GNL, parse ligne, validate)
- [x] Peut décrire la gestion des erreurs (error_msg helper, pattern uniforme)
- [x] Peut expliquer le parsing de la map (allocation dynamique, NULL terminator, verrou map_started)
- [x] Peut expliquer la validation joueur (position centrée, vecteurs dir/plane perpendiculaires)
- [x] Peut expliquer la vérification de fermeture de map (check_neighbors_of, get_map_char_or_space)

## 📝 Notes de Session

### Session 1 - Niveau 1 : Fondations (2025-10-01)

**Contexte** :

- Projet entièrement implémenté (toutes fonctionnalités mandatory)
- Objectif : Maîtriser le code pour l'évaluation par les pairs
- Création de `LEARNING_PLAN.md` (8 niveaux) et `TUTOR_GUIDELINES.md`

**Parcours de la session** :

1. **Flow général du programme** (Q&A)
   - Q: "Quelles sont les grandes étapes ?"
   - R: parsing → init → loop → écoute clavier → cleanup ✅

2. **Ce que remplit `parsing()`** (Q&A)
   - Q: "Qu'est-ce que parsing() remplit dans app ?"
   - R: Chemins textures, position joueur, couleurs RGB, direction
   - ⚠️ Oubli initial : la grille de map (corrigé quand pointé vers `t_map`)

3. **Séparation parsing/init** (Q&A)
   - Q: "Pourquoi séparer parsing() et app_init() ?"
   - R: app_init() nécessite le pointeur mlx ✅ (excellente intuition)

4. **Les 4 hooks MLX** (Q&A)
   - Q: "Dans quel ordre ces fonctions sont appelées ?"
   - R initiale: "app_loop appelée une seule fois" ❌
   - Guidage: "Combien de frames pour 10s à 60 FPS ?"
   - R corrigée: "60 fois par seconde" ✅

5. **Architecture input** (Q&A)
   - Q: "Où applique-t-on les mouvements ?"
   - R: "update_player_movement() lit les flags" ✅
   - Q: "Pourquoi ESC direct vs flags pour WASD ?"
   - R: "Sinon délai d'une frame + joueur pourrait bouger avant fermeture" ✅

6. **Cleanup et exit** (Q&A)
   - Q: "Comment app_destroy() ligne 45 peut être atteint ?"
   - R: "Je ne sais pas"
   - Guidage: Lecture de close_window() → découverte de exit(0)
   - Compréhension: Code mort, jamais atteint en pratique ✅

**Résultat** : Niveau 1 maîtrisé

### Session 2 - Niveau 2 : Parsing (2025-10-01)

**Parcours** (format Q&A condensé) :

1. Grandes étapes parsing → R: open→GNL→parse→validate→store ✅ (oubli initial GNL, corrigé)
2. Architecture 11 fichiers → R: SOC, responsabilités séparées ✅
3. `parse_input()` 3 vérifs → R: argc==2, strlen>=5, extension `.cub` ✅
4. Pourquoi `ft_memset(app,0)` → R: éviter garbage values, pointeurs NULL ✅
5. Gestion lignes vides → R: `is_empty_line()` + `continue` ✅
6. Ordre éléments flexible → R: compteurs permettent ordre libre (sauf map en dernier) ✅
7. Détection doublons → R: `texture_count > 4` détecté, mais **memory leak identifié** ⚠️

**Bug détecté ensemble** :

- Doublon texture → 2× `malloc` pour `north_path`, 1er pointeur perdu → leak
- Hugo a suggéré vérif avant parse (excellent réflexe SOC)
- Solution : check `if (north_path != NULL)` dans `parse_texture_line()` avant assign
- Fix partiel déjà ajouté ligne 33 `parse_file.c` (early detection)

---

## 🎓 Objectif Final

Connaître le programme **parfaitement** et être capable d'expliquer chaque ligne de code comme si je l'avais écrit moi-même lors de l'évaluation par les pairs.
