# 🎓 Plan d'Apprentissage cub3D - Hugo

**Objectif** : Maîtriser tous les aspects du projet cub3D pour l'expliquer parfaitement lors de l'évaluation.

**Méthodologie** : Pour chaque module, on couvrira :
1. 🧠 **Concept de haut niveau** - La théorie et le "pourquoi"
2. 💻 **Implémentation concrète** - Le "comment" dans ton code
3. 🏗️ **Architecture & choix** - Pourquoi cette structure ?
4. 🛡️ **Gestion erreurs/mémoire** - Robustesse et propreté
5. 🎯 **Questions d'évaluation** - Ce qu'on te demandera

---

## 📚 Progression Recommandée

### Niveau 1️⃣ : Fondations (Vue d'ensemble)
Comprendre le flow général du programme et les structures de données principales.

### Niveau 2️⃣ : Parsing & Validation
Maîtriser l'entrée du programme : lecture, validation, gestion d'erreurs.

### Niveau 3️⃣ : Mathématiques du Raycasting
Comprendre les vecteurs, la géométrie 2D, et les bases du raycasting.

### Niveau 4️⃣ : Algorithme DDA
Plonger dans le cœur du moteur : comment les rayons trouvent les murs.

### Niveau 5️⃣ : Projection 3D & Rendu
Transformer les données DDA en colonnes de murs à l'écran.

### Niveau 6️⃣ : Textures
Habiller les murs avec les bonnes images.

### Niveau 7️⃣ : Inputs & Mouvement
Gérer les touches, déplacer le joueur, rotation, collisions.

### Niveau 8️⃣ : Architecture Globale
Vision système : boucle de jeu, MLX, initialisation/destruction.

---

## 1️⃣ FONDATIONS - Vue d'Ensemble

### 🧠 Concepts de Haut Niveau
- **Qu'est-ce qu'un raycaster ?**
  * Simulation 3D à partir d'une carte 2D
  * Lancer un rayon par colonne d'écran (1024 rayons pour 1024 pixels de largeur)
  * Calculer la hauteur du mur en fonction de la distance

- **Pipeline complet du programme** :
  ```
  main()
    → parsing(argv)
    → app_init()
    → mlx_loop()
      → app_loop() chaque frame :
        1. update_player_movement()
        2. fill_background()
        3. render_background() (plafond/sol)
        4. render_3d_view() (murs texturés)
        5. render_minimap()
        6. mlx_put_image_to_window()
    → app_destroy()
  ```

### 💻 Implémentation Concrète
- **Fichiers clés** :
  * `main.c:28-52` - Point d'entrée
  * `loop.c` - Boucle de jeu
  * `init.c` - Initialisation MLX

- **Structure centrale : `t_app`** (cub3d.h:250-265)
  ```c
  typedef struct s_app {
      void      *mlx;        // Connexion MLX
      void      *win;        // Fenêtre
      t_img     frame;       // Buffer offscreen (double buffering)
      t_textures tex;        // 4 textures (NO/SO/WE/EA)
      t_map     map;         // Grille 2D
      t_player  player;      // Position, direction, camera plane
      t_color   floor, ceil; // Couleurs F/C
      t_keys    keys;        // État des touches
      double    move_speed;  // 0.03
      double    rot_speed;   // 0.03
  } t_app;
  ```

### 🏗️ Architecture & Choix
- **Pourquoi cette structure modulaire ?**
  * Parser séparé → testable indépendamment
  * Render séparé → focus sur l'affichage
  * Input séparé → gestion événements propre
  * Utils → DRY (Don't Repeat Yourself)

- **Pourquoi double buffering (`frame`) ?**
  * Éviter le flickering (scintillement)
  * Dessiner d'abord dans un buffer offscreen
  * Afficher l'image complète en une seule fois

### 🛡️ Gestion Erreurs/Mémoire
- **Séquence d'init** (init.c:30-44) :
  ```
  mlx_init() → load_textures() → mlx_new_window() → mlx_new_image()
  ```
  Si une étape échoue → cleanup des ressources déjà allouées

- **Cleanup complet** (init.c:65-81) :
  ```c
  app_destroy():
    1. free_textures()      // Détruit les 4 images MLX
    2. gnl_free(NULL)        // Libère buffer GNL statique
    3. mlx_destroy_image()   // Détruit frame
    4. mlx_destroy_window()
    5. mlx_destroy_display()
    6. free(mlx)
    7. free_map()            // Libère grille 2D
  ```

### 🎯 Questions d'Évaluation
- ❓ "Explique-moi le flow général de ton programme."
- ❓ "Que contient la structure `t_app` et pourquoi ?"
- ❓ "Comment gères-tu la mémoire si `mlx_new_window()` échoue ?"
- ❓ "C'est quoi le double buffering et pourquoi tu l'utilises ?"

---

## 2️⃣ PARSING & VALIDATION

### 🧠 Concepts de Haut Niveau
- **Format `.cub`** :
  ```
  NO ./path/to/north_texture.xpm
  SO ./path/to/south_texture.xpm
  WE ./path/to/west_texture.xpm
  EA ./path/to/east_texture.xpm

  F 220,100,0    (Floor RGB)
  C 225,230,255  (Ceiling RGB)

  1111111
  1000001
  1000N01
  1111111
  ```

- **3 phases de parsing** :
  1. **Lecture** : get_next_line() ligne par ligne
  2. **Extraction** : chemins textures, couleurs RGB, map
  3. **Validation** : caractères valides, map fermée, 1 seul joueur

### 💻 Implémentation Concrète
- **Architecture parsing/** (11 fichiers) :
  ```
  parse_input.c       → Vérification arguments (argc, extension .cub)
  parse_file.c        → Boucle principale GNL
  parse_tex.c         → Extraction chemins NO/SO/WE/EA
  parse_color.c       → Parsing RGB (F/C)
  parse_map.c         → Accumulation lignes de map
  validate_chars.c    → Vérification caractères autorisés (0,1, ,N,S,E,W)
  validate_player.c   → Exactement 1 joueur, orientation
  check_map_closed.c  → Map entourée de murs/vides
  find_player.c       → Trouve position et set dir/plane
  map_neighbors.c     → Helpers vérification adjacence
  parsing_utils.c     → is_map_line(), is_empty_line(), etc.
  ```

- **Flow parsing** :
  ```c
  parsing(app, argc, argv):
    1. parse_input(argc, argv)  // Vérifie args
    2. parse_cub_file(app, path)
       → open(path)
       → while (get_next_line())
          → parse_single_line()
            → parse_texture_line() ?
            → parse_color_line() ?
            → add_map_line() ?
    3. validate_map(app)
       → check_valid_chars()
       → find_player()
       → check_map_closed()
  ```

### 🏗️ Architecture & Choix
- **Pourquoi padding rectangulaire ?** (map normalisée)
  * Éviter out-of-bounds lors du raycasting
  * Toutes les lignes ont la même longueur (remplies avec espaces)
  * Accès sûr : `map.grid[y][x]`

- **Pourquoi `t_parse_counters` ?**
  * Suivre combien de textures/couleurs trouvées
  * Détecter les doublons (2×NO → erreur)
  * Savoir quand la map commence

### 🛡️ Gestion Erreurs/Mémoire
- **Cas d'erreur gérés** :
  * ❌ Mauvaise extension (`.cub` requis)
  * ❌ Fichier inexistant / permissions refusées
  * ❌ RGB invalide (hors [0,255] ou format incorrect)
  * ❌ Texture manquante/invalide
  * ❌ Map non fermée (espace adjacent au bord)
  * ❌ 0 ou 2+ joueurs
  * ❌ Caractère invalide dans la map

- **Stratégie de cleanup** :
  * Toute erreur → message `Error\n<description>` sur stderr
  * Libération de tout ce qui a été alloué avant l'erreur
  * `return (1)` pour signaler l'échec

- **Tests Valgrind** :
  * ✅ 0 bytes leaked sur tous les cas d'erreur

### 🎯 Questions d'Évaluation
- ❓ "Comment valides-tu que la map est fermée ?"
- ❓ "Que se passe-t-il si le fichier contient 2 fois 'NO' ?"
- ❓ "Comment gères-tu les erreurs de parsing ?"
- ❓ "Pourquoi normaliser la map en rectangle ?"
- ❓ "Montre-moi la fonction qui trouve le joueur et explique-la."

---

## 3️⃣ MATHÉMATIQUES DU RAYCASTING

### 🧠 Concepts de Haut Niveau
- **Système de coordonnées** :
  * Map 2D : grille de caractères `map.grid[y][x]`
  * Positions joueur : coordonnées flottantes (ex: `pos = {3.5, 2.5}`)
  * Vecteurs : `t_vec2 {x, y}` (direction, vitesse, etc.)

- **3 vecteurs clés du joueur** (cub3d.h:153-158) :
  ```c
  t_player:
    t_vec2 pos;    // Position dans la map (ex: {3.5, 2.5})
    t_vec2 dir;    // Direction de vue (vecteur unitaire normalisé)
    t_vec2 plane;  // Plan caméra (perpendiculaire à dir)
  ```

- **Comprendre `dir` et `plane`** :
  ```
  Si joueur regarde vers le NORD :
    dir   = {0, -1}   // Vecteur unitaire vers le haut
    plane = {0.66, 0} // Perpendiculaire, magnitude ~0.66 → FOV 60°

  Pourquoi plane perpendiculaire ?
    → Définit la "largeur" du champ de vision
    → magnitude(plane) contrôle le FOV
    → magnitude 0.66 ≈ tan(FOV/2) pour FOV=60°
  ```

- **Calcul d'un rayon pour la colonne `x`** :
  ```c
  cameraX = 2 * x / screen_width - 1;  // ∈ [-1, 1]
  ray_dir.x = dir.x + plane.x * cameraX;
  ray_dir.y = dir.y + plane.y * cameraX;
  ```
  * `cameraX = -1` → rayon gauche de l'écran
  * `cameraX = 0`  → rayon central (dir)
  * `cameraX = +1` → rayon droit de l'écran

### 💻 Implémentation Concrète
- **Initialisation des vecteurs** :
  * `find_player.c:set_player_orientation()` (ligne ~35-70)
    ```c
    Si map contient 'N' :
      dir   = {0, -1}  // Regarde vers le haut
      plane = {0.66, 0}

    Si map contient 'S' :
      dir   = {0, 1}   // Regarde vers le bas
      plane = {-0.66, 0}

    Si map contient 'E' :
      dir   = {1, 0}
      plane = {0, 0.66}

    Si map contient 'W' :
      dir   = {-1, 0}
      plane = {0, -0.66}
    ```

- **Calcul de ray_dir** :
  * `raycasting.c:calculate_ray_dir()` (ligne ~15-23)

### 🏗️ Architecture & Choix
- **Pourquoi `t_vec2` partout ?**
  * Abstraction propre pour les opérations vectorielles
  * Code plus lisible : `pos.x` vs `pos[0]`
  * Facilite les rotations (voir section Mouvement)

- **Pourquoi séparer `dir` et `plane` ?**
  * `dir` → direction centrale du regard
  * `plane` → définit le FOV (field of view)
  * Rotation → tourner les 2 vecteurs simultanément

### 🛡️ Gestion Erreurs/Mémoire
- Pas d'allocation dynamique pour les vecteurs (structs directes)
- Validation : vérifier que le joueur est bien sur un '0' ou un espace

### 🎯 Questions d'Évaluation
- ❓ "C'est quoi `dir` et `plane` dans `t_player` ?"
- ❓ "Pourquoi `plane` est perpendiculaire à `dir` ?"
- ❓ "Comment calcules-tu la direction d'un rayon pour une colonne `x` ?"
- ❓ "Que représente `cameraX` et pourquoi il varie de -1 à +1 ?"
- ❓ "Explique-moi le lien entre la magnitude de `plane` et le FOV."

---

## 4️⃣ ALGORITHME DDA (Cœur du Raycasting)

### 🧠 Concepts de Haut Niveau
- **DDA = Digital Differential Analyzer**
  * Algorithme pour parcourir une grille en suivant une ligne (le rayon)
  * Objectif : trouver quelle cellule de la grille est un mur ('1')
  * Principe : avancer cellule par cellule (soit en X, soit en Y) jusqu'à collision

- **Variables clés** (cub3d.h:184-195) :
  ```c
  t_ray_vars:
    int    map_x, map_y;       // Coordonnées grille actuelle
    double delta_dist_x;       // Distance pour traverser 1 cellule en X
    double delta_dist_y;       // Distance pour traverser 1 cellule en Y
    int    step_x, step_y;     // Direction (+1 ou -1)
    double side_dist_x;        // Distance jusqu'au prochain bord X
    double side_dist_y;        // Distance jusqu'au prochain bord Y
    int    side;               // 0=collision sur bord vertical, 1=horizontal
  ```

- **Concept `deltaDist`** :
  * "Si je me déplace de 1 unité sur l'axe X de la grille, combien de distance mon rayon parcourt ?"
  * Formule : `deltaDist.x = sqrt(1 + (ray_dir.y / ray_dir.x)²)`
  * Simplifié : `deltaDist.x = fabs(1.0 / ray_dir.x)`

- **Concept `sideDist`** :
  * Distance du joueur jusqu'au prochain bord de cellule (X ou Y)
  * Initialisation dépend de la direction du rayon (`step`)

- **Boucle DDA** :
  ```
  Tant que pas de mur :
    1. Comparer side_dist_x vs side_dist_y
    2. Avancer dans la direction la plus proche :
       - Si side_dist_x < side_dist_y :
           map_x += step_x
           side_dist_x += delta_dist_x
           side = 0 (vertical)
       - Sinon :
           map_y += step_y
           side_dist_y += delta_dist_y
           side = 1 (horizontal)
    3. Vérifier si map.grid[map_y][map_x] == '1'
  ```

### 💻 Implémentation Concrète
- **Fichier** : `render/dda.c`
- **Fonction principale** : `cast_ray(t_app *app, t_vec2 ray_dir, t_ray_hit *hit)`

- **Étapes** :
  1. **Initialisation** (`map_x`, `map_y` = position entière du joueur)
  2. **Calcul `deltaDist`** (lignes ~25-28)
     ```c
     delta_dist_x = fabs(1.0 / ray_dir.x);
     delta_dist_y = fabs(1.0 / ray_dir.y);
     ```
  3. **Calcul `step` et `sideDist` initial** (lignes ~30-50)
     ```c
     if (ray_dir.x < 0) {
         step_x = -1;
         side_dist_x = (player.pos.x - map_x) * delta_dist_x;
     } else {
         step_x = 1;
         side_dist_x = (map_x + 1.0 - player.pos.x) * delta_dist_x;
     }
     // Pareil pour Y
     ```
  4. **Boucle DDA** (lignes ~52-70)
  5. **Calcul distance perpendiculaire** (correction fish-eye)
     ```c
     if (side == 0)  // Collision verticale
         perp_dist = (map_x - pos.x + (1 - step_x) / 2) / ray_dir.x;
     else            // Collision horizontale
         perp_dist = (map_y - pos.y + (1 - step_y) / 2) / ray_dir.y;
     ```
  6. **Détermination de la face** (N/S/E/W)
  7. **Remplissage `t_ray_hit`** (retour)

### 🏗️ Architecture & Choix
- **Pourquoi DDA vs autres algos ?**
  * Très rapide (pas de calcul trigonométrique dans la boucle)
  * Parcours exact de la grille (pas de "trous")
  * Standard pour raycasting 2.5D

- **Pourquoi `t_ray_hit` séparée de `t_ray_vars` ?**
  * `t_ray_vars` : variables temporaires locales à DDA
  * `t_ray_hit` : résultat persistant pour le rendu

### 🛡️ Gestion Erreurs/Mémoire
- **Protection contre boucle infinie** :
  * Vérifier `map_x` et `map_y` dans les limites avant accès grille
  * Dans ton code : retour si out-of-bounds

- **Division par zéro ?**
  * Si `ray_dir.x == 0` → `delta_dist_x` devient infini → DDA n'avance jamais en X (OK)

### 🎯 Questions d'Évaluation
- ❓ "Explique-moi l'algorithme DDA étape par étape."
- ❓ "C'est quoi `delta_dist_x` et comment tu le calcules ?"
- ❓ "C'est quoi `side_dist_x` et pourquoi on l'initialise différemment selon `ray_dir.x` ?"
- ❓ "Comment sais-tu si tu as touché un mur vertical ou horizontal ?"
- ❓ "Pourquoi calcule-t-on la distance perpendiculaire plutôt que la distance euclidienne ?"
- ❓ "Montre-moi la boucle DDA dans ton code."

---

## 5️⃣ PROJECTION 3D & RENDU

### 🧠 Concepts de Haut Niveau
- **De la distance au mur à la hauteur à l'écran** :
  ```
  Plus le mur est loin → plus il paraît petit à l'écran
  Hauteur = screen_height / perp_dist
  ```

- **Projection perspective** :
  * On veut simuler la perspective humaine
  * Objets proches → grands, objets lointains → petits
  * Formule simple : `lineHeight = (int)(screen_h / perp_dist)`

- **Calcul des bornes de dessin** :
  ```c
  int lineHeight = (int)(screen_h / perp_dist);
  int drawStart = -lineHeight / 2 + screen_h / 2;
  int drawEnd = lineHeight / 2 + screen_h / 2;

  // Clamp dans [0, screen_h]
  if (drawStart < 0) drawStart = 0;
  if (drawEnd >= screen_h) drawEnd = screen_h - 1;
  ```

- **Rendu d'une colonne** :
  * Pour la colonne d'écran `x` :
    1. Lancer rayon → obtenir `perp_dist`, `side`, `wall_x`
    2. Calculer `lineHeight` et bornes
    3. Dessiner colonne de pixels de `drawStart` à `drawEnd`
    4. Chaque pixel = échantillonner la texture verticalement

### 💻 Implémentation Concrète
- **Architecture render/** :
  ```
  raycasting.c         → render_3d_view() : boucle sur toutes les colonnes
  dda.c                → cast_ray() : DDA pour une colonne
  projection.c         → calculate_wall_height(), calculate_wall_bounds()
  projection_utils.c   → Helpers calcul texture coords
  draw.c               → draw_textured_wall_column()
  ```

- **Flow de rendu** :
  ```c
  render_3d_view(app):
    for (x = 0; x < screen_width; x++)
      1. calculate_ray_dir(app, x, &ray_dir)
      2. cast_ray(app, ray_dir, &hit)        // DDA
      3. lineHeight = calculate_wall_height(hit.perp_dist, screen_h)
      4. calculate_wall_bounds(lineHeight, screen_h, &drawStart, &drawEnd)
      5. draw_textured_wall_column(app, params, &hit)
  ```

- **Fonction clé** : `draw_textured_wall_column()` (draw.c)
  ```c
  1. Sélectionner texture selon hit.wall_face (N/S/E/W)
  2. Calculer texX = get_texture_coord_x(hit.wall_x, texture)
  3. Pour chaque pixel y de drawStart à drawEnd :
       - Calculer texY = (y - drawStart) * texture.h / lineHeight
       - Récupérer couleur = get_texture_pixel(texture, texX, texY)
       - img_put_pixel(frame, x, y, couleur)
  ```

### 🏗️ Architecture & Choix
- **Pourquoi calculer `lineHeight` avant de dessiner ?**
  * Détermine combien de pixels occuper verticalement
  * Plus simple que de recalculer pour chaque pixel

- **Pourquoi clamper `drawStart/drawEnd` ?**
  * Éviter de dessiner hors de l'écran (out-of-bounds)
  * Si mur très proche, `lineHeight` peut dépasser screen_h

### 🛡️ Gestion Erreurs/Mémoire
- **Division par zéro** :
  * Si `perp_dist` est très petit → `lineHeight` devient énorme
  * Protection : `if (perp_dist < 0.1) perp_dist = 0.1;` (optionnel)

- **Accès texture out-of-bounds** :
  * `texX` et `texY` doivent être dans `[0, texture.w-1]` et `[0, texture.h-1]`
  * Protection dans `get_texture_coord_x()` et calcul `texY`

### 🎯 Questions d'Évaluation
- ❓ "Comment calcules-tu la hauteur d'un mur à l'écran ?"
- ❓ "Pourquoi utilise-t-on la distance perpendiculaire et pas euclidienne ?"
- ❓ "C'est quoi `drawStart` et `drawEnd` ?"
- ❓ "Explique-moi comment tu dessines une colonne de mur texturé."
- ❓ "Montre-moi la boucle principale de `render_3d_view()`."

---

## 6️⃣ TEXTURES

### 🧠 Concepts de Haut Niveau
- **Texture = image 2D** (ex: 64x64 pixels)
- **Mapping texture → mur** :
  * **Coordonnée X de texture (`texX`)** : quelle colonne de la texture ?
    - Déterminée par `wall_x` : où exactement le rayon a touché le mur (0.0 à 1.0)
    - `texX = (int)(wall_x * texture_width)`
  * **Coordonnée Y de texture (`texY`)** : quelle ligne ?
    - Dépend de la hauteur à l'écran et de la position du pixel
    - Échantillonnage vertical linéaire

- **Calcul de `wall_x`** :
  * Si collision verticale (side == 0) :
    ```c
    wall_x = player.pos.y + perp_dist * ray_dir.y;
    wall_x -= floor(wall_x);  // Partie fractionnaire
    ```
  * Si collision horizontale (side == 1) :
    ```c
    wall_x = player.pos.x + perp_dist * ray_dir.x;
    wall_x -= floor(wall_x);
    ```

### 💻 Implémentation Concrète
- **Structure `t_textures`** (cub3d.h:118-129) :
  ```c
  typedef struct s_textures {
      t_img  north, south, west, east;
      char   *north_path, *south_path, *west_path, *east_path;
      bool   loaded;
  } t_textures;
  ```

- **Chargement** : `render/textures.c:load_textures()`
  ```c
  1. mlx_xpm_file_to_image() pour chaque texture
  2. mlx_get_data_addr() pour récupérer le buffer de pixels
  3. Stocker dans app->tex
  ```

- **Sélection texture** : `texture_utils.c:get_wall_texture()`
  ```c
  switch (hit->wall_face) {
      case FACE_NORTH: return &app->tex.north;
      case FACE_SOUTH: return &app->tex.south;
      case FACE_EAST:  return &app->tex.east;
      case FACE_WEST:  return &app->tex.west;
  }
  ```

- **Échantillonnage** : `texture_utils.c:get_texture_pixel()`
  ```c
  int offset = texY * texture.line_len + texX * (texture.bpp / 8);
  return *(int *)(texture.addr + offset);
  ```

### 🏗️ Architecture & Choix
- **Pourquoi stocker 4 textures séparées ?**
  * Chaque face de mur (N/S/E/W) peut avoir une texture différente
  * Permet plus de variété visuelle
  * Requis par le sujet

- **Pourquoi `loaded` dans `t_textures` ?**
  * Flag pour savoir si les textures ont été chargées avec succès
  * Si échec de chargement → fallback sur couleurs unies

### 🛡️ Gestion Erreurs/Mémoire
- **Erreurs de chargement** :
  * `mlx_xpm_file_to_image()` retourne NULL si échec
  * → message d'erreur + cleanup des textures déjà chargées
  * Séquence de cleanup : `texture_cleanup.c`
    ```c
    cleanup_north_texture()
    cleanup_north_south_textures()
    cleanup_north_south_west_textures()
    // Nettoie progressivement selon combien ont été chargées
    ```

- **Libération** : `free_textures()` (texture_cleanup.c)
  ```c
  if (tex.north.ptr) mlx_destroy_image(mlx, tex.north.ptr);
  if (tex.south.ptr) mlx_destroy_image(mlx, tex.south.ptr);
  // etc.
  ```

### 🎯 Questions d'Évaluation
- ❓ "Comment calcules-tu `wall_x` ?"
- ❓ "C'est quoi `texX` et `texY` ?"
- ❓ "Comment sélectionnes-tu la bonne texture pour un mur ?"
- ❓ "Montre-moi comment tu échantillonnes un pixel de texture."
- ❓ "Que se passe-t-il si le chargement d'une texture échoue ?"

---

## 7️⃣ INPUTS & MOUVEMENT

### 🧠 Concepts de Haut Niveau
- **Système d'input basé sur flags** :
  * `KeyPress` → `keys.w = true`
  * `KeyRelease` → `keys.w = false`
  * Boucle de jeu lit les flags et applique les mouvements

- **3 types de mouvements** :
  1. **Avant/Arrière** : déplacement dans la direction `dir`
     ```c
     new_pos = pos + dir * move_speed * direction
     ```
  2. **Strafe (gauche/droite)** : déplacement perpendiculaire à `dir`
     ```c
     strafe_dir = perpendicular(dir)  // Rotation 90°
     new_pos = pos + strafe_dir * move_speed * direction
     ```
  3. **Rotation** : faire tourner `dir` et `plane` d'un angle
     ```c
     Matrice de rotation 2D :
     [ cos(θ)  -sin(θ) ]
     [ sin(θ)   cos(θ) ]
     ```

### 💻 Implémentation Concrète
- **Structure `t_keys`** (cub3d.h:167-175) :
  ```c
  typedef struct s_keys {
      bool w, a, s, d;
      bool left, right;
  } t_keys;
  ```

- **Hooks clavier** (main.c:46-47) :
  ```c
  mlx_hook(win, 2, 1L<<0, key_press, &app);    // KeyPress event
  mlx_hook(win, 3, 1L<<1, key_release, &app);  // KeyRelease event
  ```

- **Gestion événements** : `input/keys.c`
  ```c
  int key_press(int keycode, t_app *app) {
      if (keycode == KEY_W) app->keys.w = true;
      if (keycode == KEY_A) app->keys.a = true;
      // etc.
  }
  ```

- **Application mouvements** : `input/movement.c`
  ```c
  update_player_movement(app):
    if (keys.w) move_player_forward_backward(app, 1);
    if (keys.s) move_player_forward_backward(app, -1);
    if (keys.a) move_player_strafe(app, -1);
    if (keys.d) move_player_strafe(app, 1);
    if (keys.left) rotate_player(app, -1);
    if (keys.right) rotate_player(app, 1);
  ```

- **Collision** :
  ```c
  move_player(app, move_x, move_y):
    new_x = player.pos.x + move_x;
    new_y = player.pos.y + move_y;
    if (is_valid_position(app, new_x, new_y))
      player.pos.x = new_x;
      player.pos.y = new_y;

  is_valid_position(app, x, y):
    map_x = (int)x;
    map_y = (int)y;
    return map.grid[map_y][map_x] != '1';
  ```

- **Rotation** : `movement.c:rotate_player()`
  ```c
  double angle = rot_speed * direction;
  double cos_a = cos(angle);
  double sin_a = sin(angle);

  // Rotation de dir
  t_vec2 old_dir = player.dir;
  player.dir.x = old_dir.x * cos_a - old_dir.y * sin_a;
  player.dir.y = old_dir.x * sin_a + old_dir.y * cos_a;

  // Rotation de plane (même angle)
  t_vec2 old_plane = player.plane;
  player.plane.x = old_plane.x * cos_a - old_plane.y * sin_a;
  player.plane.y = old_plane.x * sin_a + old_plane.y * cos_a;
  ```

### 🏗️ Architecture & Choix
- **Pourquoi flags au lieu d'actions directes ?**
  * Permet plusieurs touches simultanées (W+D = diagonale)
  * Découple événements (KeyPress/Release) de la logique de jeu (loop)

- **Pourquoi rotation de `dir` ET `plane` ?**
  * `dir` → direction centrale change
  * `plane` doit rester ⟂ à `dir` et maintenir le FOV
  * Même angle pour les deux → cohérence

### 🛡️ Gestion Erreurs/Mémoire
- **Collision grid-based simple** :
  * Pro : rapide, pas de malloc
  * Con : collision "carrée" (pas de sliding le long des murs)

- **Protection out-of-bounds** :
  * Vérifier `map_x` et `map_y` dans les limites avant accès `map.grid`

### 🎯 Questions d'Évaluation
- ❓ "Explique-moi ton système de gestion des touches."
- ❓ "Comment fais-tu pour avancer/reculer ?"
- ❓ "C'est quoi le strafe et comment tu le calcules ?"
- ❓ "Montre-moi comment tu fais tourner le joueur."
- ❓ "Comment gères-tu les collisions ?"
- ❓ "Pourquoi tourner à la fois `dir` et `plane` ?"

---

## 8️⃣ ARCHITECTURE GLOBALE & MLX

### 🧠 Concepts de Haut Niveau
- **MiniLibX** : bibliothèque graphique simple pour 42
  * Abstraction de X11 (Linux) ou AppKit (macOS)
  * Fenêtres, images, événements

- **Double buffering** :
  * Problème : dessiner directement à l'écran → flickering
  * Solution : dessiner dans un buffer offscreen → afficher en 1 fois

- **Boucle de jeu** :
  ```
  Initialisation → Enregistrement hooks → mlx_loop() (infini)
    ↓
  Chaque frame :
    1. Traiter inputs
    2. Mettre à jour état (mouvement)
    3. Rendu complet
    4. Affichage
  ```

### 💻 Implémentation Concrète
- **Séquence d'init** : `main.c + init.c`
  ```c
  main():
    1. parsing(&app, argc, argv)        // Parse .cub
    2. app_init(&app, win_w, win_h)
       → mlx_init()
       → load_textures()
       → mlx_new_window()
       → mlx_new_image() (frame offscreen)
    3. mlx_loop_hook(mlx, app_loop, &app)  // Fonction appelée chaque frame
    4. mlx_hook(win, 2, ..., key_press)    // KeyPress
    5. mlx_hook(win, 3, ..., key_release)  // KeyRelease
    6. mlx_hook(win, 17, ..., close_window) // Fermeture fenêtre (croix)
    7. mlx_loop(mlx)                       // Boucle infinie
    8. app_destroy(&app, 0)                // Cleanup
  ```

- **Boucle de jeu** : `loop.c:app_loop()`
  ```c
  int app_loop(t_app *app) {
      update_player_movement(app);        // Applique touches
      fill_background(app, 0x000000);     // Efface frame
      render_background(app);             // Plafond/sol
      render_3d_view(app);                // Murs texturés (raycasting)
      render_minimap(app);                // Overlay minimap
      mlx_put_image_to_window(app->mlx, app->win, app->frame.ptr, 0, 0);
      return (0);
  }
  ```

- **Destruction** : `init.c:app_destroy()`
  ```c
  1. free_textures(app)
  2. gnl_free(NULL)
  3. mlx_destroy_image(mlx, frame.ptr)
  4. mlx_destroy_window(mlx, win)
  5. mlx_destroy_display(mlx)
  6. free(mlx)
  7. free_map(app)
  ```

### 🏗️ Architecture & Choix
- **Pourquoi `mlx_loop_hook` au lieu de boucle while ?**
  * MLX gère la synchronisation (vsync implicite)
  * Intégration avec le gestionnaire d'événements

- **Pourquoi `app_loop` retourne `int` ?**
  * Signature imposée par MLX
  * Retourner non-zéro stopperait la boucle (pas utilisé ici)

### 🛡️ Gestion Erreurs/Mémoire
- **Échec d'init** :
  * Chaque étape peut échouer → cleanup des ressources partielles
  * Exemple : si `mlx_new_window()` échoue mais `mlx_init()` a réussi
    → il faut quand même `free(mlx)`

- **Tests Valgrind** :
  * ✅ Tous cas d'erreur : 0 bytes leaked
  * ⚠️ Cas nominaux avec `timeout` : "still reachable" (attendu car SIGTERM)

### 🎯 Questions d'Évaluation
- ❓ "Explique-moi la boucle de jeu complète."
- ❓ "C'est quoi le double buffering et pourquoi tu l'utilises ?"
- ❓ "Que fait `mlx_loop_hook` ?"
- ❓ "Comment gères-tu la fermeture propre du programme ?"
- ❓ "Montre-moi la séquence de destruction des ressources."

---

## 🎯 STRATÉGIE DE RÉVISION

### Phase 1 : Lecture Active (2-3 jours)
- Lire ce plan en entier
- Pour chaque section :
  * Lire le code correspondant
  * Annoter avec des commentaires en français
  * Noter les parties floues dans HUGO.md

### Phase 2 : Explication à Voix Haute (2-3 jours)
- Pour chaque module, expliquer à voix haute comme à un évaluateur
- S'enregistrer (optionnel) pour identifier les hésitations
- Se poser les questions d'évaluation et y répondre sans notes

### Phase 3 : Sessions Socratiques avec l'IA (3-4 jours)
- Demander à l'IA de te poser des questions sur chaque module
- L'IA utilise la méthode socratique pour te guider
- Mise à jour de HUGO.md après chaque session

### Phase 4 : Simulation d'Évaluation (1 jour)
- Mock evaluation avec l'IA ou un pair
- Timer 15 minutes par section
- Feedback sur clarté et précision

---

## 📌 RESSOURCES UTILES

### Commandes de Debug Utiles
```bash
# Lancer avec Valgrind (détection fuites)
env DISPLAY=:0 valgrind --leak-check=full --show-leak-kinds=all \
  --track-origins=yes --log-file=/tmp/valgrind.log ./cub3D maps/sample.cub

# Voir les logs Valgrind
cat /tmp/valgrind.log

# Build
make re

# Test rapide
./cub3D maps/sample.cub
```

### Commandes Git (Workflow)
```bash
# Créer une branche feature
git checkout consolidation && git pull
git checkout -b hugo/learning

# Commit
git add HUGO.md LEARNING_PLAN.md
git commit -m "docs(learning): add learning plan and tracking"

# Push
git push -u origin hugo/learning
```

---

## 🏆 CHECKLIST FINALE AVANT ÉVALUATION

- [ ] Je peux expliquer le flow général du programme (main → loop → destroy)
- [ ] Je comprends toutes les structures de données (`t_app`, `t_player`, `t_ray_hit`, etc.)
- [ ] Je peux expliquer le parsing ligne par ligne
- [ ] Je maîtrise les vecteurs `dir` et `plane`
- [ ] Je peux dessiner l'algorithme DDA au tableau
- [ ] Je comprends la projection 3D et le calcul de hauteur
- [ ] Je peux expliquer le mapping de textures (`wall_x`, `texX`, `texY`)
- [ ] Je maîtrise le système de mouvement et rotation
- [ ] Je comprends la gestion d'erreurs et mémoire
- [ ] J'ai testé le programme avec Valgrind (0 leaks)
- [ ] Je peux naviguer rapidement dans mon code (connaître les numéros de ligne clés)

---

**Prêt à commencer ton apprentissage ? Par quel module veux-tu démarrer ?**
