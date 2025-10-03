# Parcours d'Apprentissage cub3D - Hugo

## 🎯 Focus Actuel

Niveau 2 - Parsing & Validation - COMPLÉTÉ ✅ (100%)
Date : 2025-10-03

## ✅ Concepts Maîtrisés

### 🏗️ Niveau 1 : Fondations - MAÎTRISÉ ✅
**Date** : 2025-10-01

### 📄 Niveau 2 : Parsing & Validation - MAÎTRISÉ ✅
**Date** : 2025-10-03 (complété)

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

## 📊 Préparation à l'Évaluation

### Théorie du Raycasting (0/5) : Pas encore évalué

- [ ] Peut expliquer comment fonctionne le raycasting
- [ ] Peut décrire la correction du fish-eye
- [ ] Peut expliquer le calcul de perspective

### Algorithme DDA (0/5) : Pas encore évalué

- [ ] Peut expliquer le processus DDA étape par étape
- [ ] Peut décrire la signification de deltaDistX/Y
- [ ] Peut expliquer le rôle de sideDistX/Y

### Mapping de Textures (0/5) : Pas encore évalué

- [ ] Peut expliquer le calcul de wallX
- [ ] Peut décrire le mapping des coordonnées de texture
- [ ] Peut expliquer l'échantillonnage vertical

### Système de Mouvement (0/5) : Pas encore évalué

- [ ] Peut expliquer le mouvement avant/arrière
- [ ] Peut décrire la mécanique de strafe
- [ ] Peut expliquer la rotation avec les vecteurs dir/plane

### Détection de Collision (0/5) : Pas encore évalué

- [ ] Peut expliquer la collision basée sur la grille
- [ ] Peut décrire l'approche de validation

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

**À reprendre** : Implémenter fix complet dans `parse_tex.c`, continuer validation map

---

## 🎓 Objectif Final

Connaître le programme **parfaitement** et être capable d'expliquer chaque ligne de code comme si je l'avais écrit moi-même lors de l'évaluation par les pairs.
