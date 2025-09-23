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

### Environnement de développement

- **macOS (machine hôte)**:
  - MiniLibX OpenGL (frameworks AppKit/OpenGL)
  - Pixels: ARGB32; penser à mettre alpha = 0xFF pour voir les couleurs (fait dans `rgb_to_int`)
  - Chemin du projet: `/Users/hugoganet/Code/42/cub3D`

- **Linux VM (machine virtuelle)**:
  - IP: `192.168.64.5`
  - Utilisateur: `hganet`
  - Chemin du projet: `/home/hganet/cub3D`
  - MiniLibX X11 (libXext, libX11, libm, zlib)
  - Makefile adapté: `-lmlx -lXext -lX11 -lm -lz` et `-I/usr/include` / `-L/usr/lib`
  - Valgrind disponible pour détecter les fuites mémoire

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

- **MLX_INIT TROUBLESHOOTING** (CRITIQUE):
  - Si `mlx_init failed`: TOUJOURS vérifier `DISPLAY` en premier
  - Test rapide: `env DISPLAY=:0 ./cub3D maps/sample.cub`
  - Diagnostic: créer test minimal avec `mlx_init()` seul
  - Root cause: MLX nécessite connexion X11 valide pour initialiser
  - **Solution permanente**: Alias configuré dans ~/.bashrc
    - Usage: `cub3d maps/sample.cub` (DISPLAY automatiquement défini)
    - Si l'alias ne marche pas: `source ~/.bashrc` ou nouvelle session SSH

Conseil: isoler toute la gestion MLX (création image, `mlx_get_data_addr`, destruction) dans des fonctions dédiées pour limiter les différences entre plateformes.

## 6) État actuel et prochaines étapes

### ✅ Implémenté
1. **Boot minimal**: fenêtre, boucle MLX, entrées basiques
2. **Parser complet**:
   - Parse headers (textures NO/SO/WE/EA, couleurs F/C)
   - Parse et validation map avec flood-fill
   - Position et direction joueur (N/S/E/W → dir/plane)
   - Gestion erreurs avec cleanup mémoire
3. **Validation robuste**: caractères autorisés, map fermée, spawn unique
4. **Minimap fonctionnelle**: affichage temps réel, position joueur, murs/sols
5. **Architecture modulaire**: parser/, render/, input/, utils/ opérationnels
6. **Cross-platform**: switch macOS OpenGL → Linux X11, Makefile adapté
7. **Ray visualization sur minimap**: 20 rayons verts étalés sur 60° FOV (-30° à +30°), DDA pour collision murs
8. **🎯 RAYCASTING 3D COMPLET**:
   - DDA algorithm étendu avec `t_ray_hit` structure
   - Projection 3D: calcul hauteurs murs, perspective correcte
   - Camera plane initialisé pour 60° FOV
   - 1024 rayons (1 par colonne écran) avec couleurs debug par face (N/S/E/W)
   - Rendu temps réel: background → 3D walls → minimap overlay

### 🔄 À faire
1. **Texturage murs**: chargement XPM, wallX/texX, sampling vertical (interface prête pour Nico)
2. **Mouvement avec collision**: grid-based collision detection
3. **Tests Valgrind**: validation mémoire sur parsing/cleanup
4. **Intégration avec branche Nicolas**: merger système background + textures

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

- **MiniLibX**: Linux X11 (intégrée directement, plus de submodule)
- **Libft**: intégrée directement (dossier `libft/`) avec ft_printf, get_next_line
- **Build**: Makefile Linux compatible, link `-lmlx -lXext -lX11 -lm -lz`
- **Parser**: modules complets pour .cub (colors.c, parse_tex.c, validate_map.c, etc.)
- **Structures**: `t_app`, `t_player`, `t_map`, `t_ray_hit` définies et utilisées
- **Maps test**: `sample.cub` (valide), `error.cub` (test erreurs)

### 📁 Fichiers Raycasting 3D (Hugo)
- **`src/render/raycasting.c`**: boucle principale 1024 colonnes, calcul directions rayons
- **`src/render/projection.c`**: maths 3D (hauteurs murs, bounds écran, rendu colonnes)
- **`src/render/dda.c`**: algorithme DDA étendu avec `t_ray_hit` pour infos complètes
- **`includes/cub3d.h`**: structures `t_ray_hit`, enums faces murs, prototypes fonctions
- **`src/parser/validate_map.c`**: 🐛 **FIX CRITIQUE** - initialisation camera plane par orientation
- **`src/loop.c`**: intégration `render_3d_view()` entre background et minimap
- **`Makefile`**: ajout nouveaux fichiers sources

---

Ce plan permet de livrer rapidement un rendu minimal et d'itérer en sécurité, tout en garantissant la portabilité macOS ↔ Linux et la compatibilité avec Valgrind dès que possible.
