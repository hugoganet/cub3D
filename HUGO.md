# Parcours d'Apprentissage cub3D - Hugo

## 🎯 Focus Actuel

Niveau 2 - Parsing & Validation (50% complété - à reprendre : correction bug memory leak doublons)

## ✅ Concepts Maîtrisés

### 🏗️ Niveau 1 : Fondations - MAÎTRISÉ ✅
**Date** : 2025-10-01

### 📄 Niveau 2 : Parsing & Validation - EN COURS (50%) ⏳
**Date** : 2025-10-01

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

**🐛 Bug détecté ensemble** :
- ⚠️ Memory leak si doublon texture : `north_path` écrasé sans free du 1er malloc
- 💡 Solution identifiée : vérifier `if (north_path != NULL)` avant assignation dans `parse_texture_line()`
- 📍 Ligne 33 `parse_file.c` : check ajouté `if (texture_count == 4) → error` (détection précoce)
- ⏸️ **À reprendre** : implémenter vérification dans `parse_tex.c` pour chaque texture

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
- ✅ Sans loop : rien ne démarre (hooks jamais appelés)
- ✅ `exit(0)` : fonction libc (stdlib.h) qui **tue le processus entier**, pas une fonction MLX
- ✅ Pourquoi ligne 45 main.c jamais atteinte : `close_window()` appelle `exit(0)` → processus terminé avant retour de `mlx_loop()`

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

## 💡 Métaphores & Analogies Personnalisées

### Sessions 1-2 (2025-10-01)

**Aucune métaphore explicite utilisée** - Apprentissage par Q&A directes sur code.

### Session 3 (2025-10-02)

**Métaphore du Restaurant** :

- `mlx_loop_hook()` = Donner une recette au chef (instructions)
- `mlx_loop()` = Ouvrir le restaurant (exécution)
- Sans loop_hook : restaurant ouvert mais aucun plat préparé (fenêtre noire)
- `exit(0)` = Fermer brutalement le restaurant en éteignant les lumières

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

### Parser (2/5) : En cours

- [x] Peut expliquer les étapes de validation (open, GNL, parse ligne, validate)
- [x] Peut décrire la gestion des erreurs (error_exit, cleanup, compteurs)
- [ ] Peut expliquer la vérification de fermeture de map (pas encore vu)

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
