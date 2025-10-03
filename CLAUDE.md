# cub3D – Stratégie d'implémentation

Ce document décrit l'approche retenue pour le projet cub3D, les choix structurants, et l'état actuel de l'implémentation. Le projet fonctionne actuellement sur **Linux VM uniquement** (MiniLibX X11). L'objectif est d'obtenir un socle propre, testable et maintenable, avec un rendu texturé complet.

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

## 5) Environnement de développement

### Configuration actuelle: Linux VM uniquement

- **Linux VM (environnement de développement)**:
  - IP: `192.168.64.5`
  - Utilisateur: `hganet`
  - Chemin du projet: `/home/hganet/cub3D`
  - MiniLibX X11 (libXext, libX11, libm, zlib)
  - Makefile: `-lmlx -lXext -lX11 -lm -lz`
  - Valgrind disponible pour détecter les fuites mémoire

- **macOS (machine hôte pour Git)**:
  - Chemin du projet: `/Users/hugoganet/Code/42/cub3D`
  - Utilisé uniquement pour les opérations Git (commits, push, PR)
  - Synchronisation bidirectionnelle via Mutagen

### Synchronisation et connectivité

- **SSH**:
  - Clé SSH: `~/.ssh/vm_dev` (doit être chargée dans l'agent SSH: `ssh-add ~/.ssh/vm_dev`)
  - Connexion: `ssh hganet@192.168.64.5`

- **Mutagen**:
  - Synchronisation bidirectionnelle entre macOS et Linux VM
  - Session: `cub3d-sync` synchronise `/Users/hugoganet/Code/42/cub3D` ↔ `/home/hganet/cub3D`
  - Fichiers ignorés: `.git/` (les dossiers git ne sont pas synchronisés)
  - Configuration globale: `~/.mutagen/.mutagen.yml` (ignore automatiquement `*.o`, `*.a`, `.DS_Store`, etc.)
  - Commandes utiles:
    - `mutagen sync list cub3d-sync` - vérifier l'état de la synchronisation
    - `mutagen sync flush cub3d-sync` - forcer une synchronisation
    - `mutagen sync terminate cub3d-sync` - arrêter la session

- **Important pour le debug**:
  - Si la synchronisation ne fonctionne pas, vérifier:
    1. SSH agent a la clé: `ssh-add -l` (sinon: `ssh-add ~/.ssh/vm_dev`)
    2. Connexion SSH fonctionne: `ssh hganet@192.168.64.5 'echo OK'`
    3. Mutagen est connecté: `mutagen sync list cub3d-sync | grep Connected`
  - Les modifications de fichiers sont synchronisées automatiquement dans les deux sens
  - Le dossier `.claude/` est synchronisé pour maintenir les configurations cohérentes

- **IMPORTANT - Git et commits**:
  - **Git commits doivent être faits depuis la machine hôte (macOS)**, pas depuis la VM Linux
  - Raison: Les clés SSH et l'accès au repository GitHub sont configurés sur macOS
  - Workflow recommandé:
    1. Développer et tester sur la VM Linux (Valgrind, build, run)
    2. Les changements se synchronisent automatiquement vers macOS via Mutagen
    3. Faire les commits, PR et push depuis macOS
  - Si l'IA tente de commit depuis la VM, rappeler cette contrainte

- **MLX_INIT - Configuration DISPLAY**:
  - **Configuration permanente appliquée**: `export DISPLAY=:0` configuré dans ~/.bashrc
    - Configuration placée **au début du fichier** (ligne 6, avant le guard d'interactivité)
    - Garantit que DISPLAY est exporté même pour shells non-interactifs (nécessaire pour Node.js/scripts)
    - L'alias `cub3d` reste disponible pour lancer rapidement le programme
    - Testeurs externes (npm, Node.js exec) héritent automatiquement de DISPLAY
    - DISPLAY est automatiquement configuré pour toutes les nouvelles sessions SSH
  - **Lancement du programme**: `./cub3D maps/sample.cub` (pas besoin de `env DISPLAY=:0`)
  - **Troubleshooting**: Si `mlx_init failed` dans une session existante, exécuter `source ~/.bashrc`


## 6) État actuel et prochaines étapes

### ✅ Implémenté (Fonctionnalités complètes)
1. **Initialisation MLX** (Linux X11): fenêtre, boucle d'événements, image offscreen
2. **Parser complet**:
   - Parse headers (textures NO/SO/WE/EA, couleurs F/C)
   - Parse et validation map avec vérification de fermeture
   - Position et direction joueur (N/S/E/W → vecteurs dir/plane)
   - Gestion erreurs avec cleanup mémoire complet
3. **Validation robuste**: caractères autorisés, map fermée, spawn unique
4. **Architecture modulaire**: parser/ (11 fichiers), render/ (14 fichiers), input/ (2 fichiers), utils/ (3 fichiers)
5. **Système d'entrées**: gestion WASD, flèches, ESC avec flags booléens
6. **Mouvement du joueur avec collision**:
   - Forward/backward (W/S), strafe (A/D), rotation (flèches)
   - Collision detection grid-based (`is_valid_position`)
   - Paramètres: `move_speed` 0.03, `rot_speed` 0.03
7. **Moteur de raycasting 3D**:
   - Algorithme DDA avec structure `t_ray_hit` complète
   - 1024 rayons par frame (un par colonne d'écran)
   - Calcul de distance perpendiculaire (correction fish-eye)
   - Détection de face de mur (N/S/E/W)
8. **Projection 3D et rendu**:
   - Calcul hauteurs de murs avec perspective correcte
   - Camera plane pour FOV de 60° (magnitude plane ≈ 0.66)
   - Projection perspective-correct
9. **Chargement et rendu de textures**:
   - Chargement de 4 textures XPM (NO/SO/WE/EA)
   - Mapping texture par face de mur
   - Échantillonnage vertical avec calcul wallX/texX
   - Mode fallback avec couleurs solides si textures non chargées
10. **Rendu de fond**: plafond et sol avec couleurs unies (F/C du .cub)
11. **Minimap overlay**:
    - Vue 2D top-down en temps réel
    - Position du joueur, murs/sols
    - Visualisation de 20 rayons (lignes vertes)
12. **Gestion mémoire**: cleanup complet des textures, map, MLX à la sortie

### 🔄 Améliorations possibles
1. **Collision detection**: Ajouter une marge/hitbox autour du joueur (actuellement grid-based strict)
2. **Conformité Norminette**: Vérification finale de tous les fichiers

### ✅ Tests Valgrind - Validation Mémoire Complète

**Suite de tests complète** : 59 maps (19 valides + 38 invalides + 2 root)

#### Structure des tests
```
maps/
├── sample.cub, complex.cub        # Maps originales
├── valid/                         # 19 maps valides
│   ├── simple_square.cub, minimal_3x3.cub
│   ├── player_north/south/east/west.cub
│   ├── irregular_shape.cub, spaces_outside_walls.cub
│   └── rgb_min/max_values.cub, elements_unordered.cub
└── invalid/                       # 38 maps invalides
    ├── missing_*_texture.cub, missing_*_color.cub
    ├── rgb_negative/over_255/non_numeric.cub
    ├── no_player.cub, multiple_players.cub
    ├── map_not_closed_*.cub, player_on_edge_*.cub
    └── space_in_playable_area.cub, hole_in_wall.cub
```

#### Script de test automatisé
```bash
./test_maps.sh  # Valgrind complet sur toutes les maps
```

**Flags Valgrind** : `--leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=mlx.supp`
**Validation stricte** : Détecte definitely lost, indirectly lost, **et still reachable**
**Zéro tolérance** : Aucune fuite acceptée (exit si total_leaks > 0)

#### Validation map closure
**Fix récent** : Correction logique `check_map_closed()` (ligne 43)
- Avant : `has_open_neighbor(app, j, i) && is_at_map_edge(app, j, i)` ❌
- Après : `has_open_neighbor(app, j, i) || is_at_map_edge(app, j, i)` ✅
- Détecte maintenant : joueur sur bord **OU** espace adjacent à zone jouable

### ❌ Non implémenté (Bonus)
1. **Texturage sol/plafond** (floor/ceiling casting)
2. **Sprites** (ennemis, objets)
3. **Portes animées**
4. **Vue à la souris**
5. **Toggle minimap**

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

## 8) Workflow Git pour collaboration 42

### Structure des branches

Le projet utilise un workflow Git structuré pour faciliter la collaboration entre les membres de l'équipe:

```
main                    # Branche principale (production)
  └── consolidation     # Branche d'intégration
        ├── hugo/*      # Branches de features Hugo
        └── nicolas/*   # Branches de features Nicolas
```

### Règles de nommage des branches

- **Format**: `<prenom>/<nom-feature>`
- **Exemples**:
  - `hugo/raycasting`
  - `hugo/textures`
  - `nicolas/minimap`
  - `nicolas/collision`

### Processus de développement

1. **Création de feature branch**:
   ```bash
   git checkout consolidation
   git pull origin consolidation
   git checkout -b hugo/ma-feature
   ```

2. **Développement**:
   - Travailler sur sa branche feature
   - Commits réguliers avec messages Commitizen
   - Push vers origin: `git push -u origin hugo/ma-feature`

3. **Pull Request vers consolidation**:
   - Quand la feature est terminée, créer une PR de `hugo/ma-feature` → `consolidation`
   - Titre PR: `feat(scope): description` (style Commitizen)
   - Description: liste des changements principaux
   - Assigner le coéquipier pour review

4. **Review et merge dans consolidation**:
   - Le coéquipier review la PR
   - Discussion si nécessaire
   - Merge dans `consolidation` après validation

5. **Intégration dans main**:
   - Quand les deux features sont merged dans `consolidation`
   - Tester l'intégration complète
   - Créer une PR de `consolidation` → `main`
   - Les deux coéquipiers approuvent avant merge

### Commandes Git essentielles pour l'IA

- **Créer une nouvelle branche feature**:
  ```bash
  git checkout consolidation && git pull
  git checkout -b <nom>/<feature>
  ```

- **Créer une Pull Request**:
  ```bash
  gh pr create --base consolidation --title "type(scope): description"
  ```

- **Lister les PR ouvertes**:
  ```bash
  gh pr list
  ```

- **Voir le statut d'une PR**:
  ```bash
  gh pr view <numero>
  ```

### Instructions pour l'IA

1. **TOUJOURS** créer des branches avec le format `<prenom>/<feature>`
2. **JAMAIS** pusher directement sur `main` ou `consolidation`
3. **TOUJOURS** créer une PR pour merger dans `consolidation`
4. **NE PAS** merger de `consolidation` vers `main` sans validation explicite
5. **UTILISER** les commandes `gh` pour gérer les PR programmatiquement
6. **RESPECTER** le style Commitizen pour tous les commits et PR

### Résolution de conflits

En cas de conflit lors du merge dans `consolidation`:
1. Pull la dernière version de `consolidation`
2. Merger `consolidation` dans la feature branch
3. Résoudre les conflits localement
4. Push et mettre à jour la PR

## 9) Configuration actuelle

- **MiniLibX**: Linux X11 (intégrée directement dans `minilibx-linux/`, pas de submodule)
- **Libft**: intégrée directement (dossier `libft/`) avec ft_printf, get_next_line
- **Build**: Makefile Linux, flags: `-lmlx -lXext -lX11 -lm -lz`
- **Structures principales**: `t_app`, `t_player`, `t_map`, `t_textures`, `t_ray_hit`, `t_keys`
- **Maps test**:
  - `maps/sample.cub` (map valide avec spawn joueur)
  - `maps/error.cub` (tests de cas d'erreur)
  - `maps/complex.cub` (map plus complexe)

### 📁 Architecture des fichiers

**Parsing (11 fichiers)**:
- `parse_input.c`, `parse_file.c`, `parse_tex.c`, `parse_color.c`, `parse_map.c`
- `validate_chars.c`, `validate_player.c`, `check_map_closed.c`
- `find_player.c`, `map_neighbors.c`, `parsing_utils.c`

**Rendu (14 fichiers)**:
- **Raycasting**: `raycasting.c`, `dda.c`, `ray_utils.c`
- **Projection 3D**: `projection.c`, `projection_utils.c`
- **Textures**: `textures.c`, `texture_utils.c`, `texture_cleanup.c`
- **Background**: `background.c`
- **Minimap**: `minimap.c`, `minimap_utils.c`
- **Dessin**: `draw.c`, `draw_utils.c`

**Entrées (2 fichiers)**: `keys.c`, `movement.c`

**Utils (3 fichiers)**: `errors.c`, `mem.c`, `math.c`

**Core (3 fichiers)**: `main.c`, `init.c`, `loop.c`

### 🎯 Pipeline de rendu complet

La boucle de jeu (`loop.c:app_loop`) exécute:
1. `update_player_movement()` - Traite les entrées et déplace le joueur
2. `fill_background()` - Nettoie le buffer de frame
3. `render_background()` - Dessine plafond et sol (couleurs unies)
4. `render_3d_view()` - Raycasting + projection 3D (murs texturés)
5. `render_minimap()` - Overlay 2D avec position et rayons
6. `mlx_put_image_to_window()` - Affiche la frame complète

---

Le projet est **entièrement fonctionnel** avec toutes les fonctionnalités mandatory implémentées. Le moteur de raycasting est performant et le code est modulaire, maintenable et conforme à la Norme 42.
