# Parcours d'Apprentissage cub3D - Hugo

## 🎯 Focus Actuel

Niveau 2 - Parsing & Validation (prochain)

## ✅ Concepts Maîtrisés

### 🏗️ Niveau 1 : Fondations - MAÎTRISÉ ✅

**Date** : 2025-10-01

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

## 🔄 Zones Nécessitant un Renforcement

### Session 1 - Niveau 1 (2025-10-01)

- ⚠️ **Confusion initiale sur `mlx_loop_hook`** : Hugo pensait qu'elle était appelée une seule fois (boucle principale), alors qu'elle est appelée ~60 FPS en continu. **Résolu** après explication.
- ⚠️ **Oubli de la map dans les données parsées** : Lors de la liste des éléments remplis par `parsing()`, Hugo a oublié la grille de map (a listé textures, couleurs, position/direction joueur). **Résolu** immédiatement quand pointé vers `t_map`.
- ✅ **Bonne intuition** : A correctement identifié que `app_init()` nécessite le pointeur `mlx` pour charger les ressources.

## 💡 Métaphores & Analogies Personnalisées

### Session 1 - Niveau 1 (2025-10-01)

**Aucune métaphore explicite utilisée** - Apprentissage par questions/réponses directes sur le code.

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

### Parser (0/5) : Pas encore évalué

- [ ] Peut expliquer les étapes de validation
- [ ] Peut décrire la gestion des erreurs
- [ ] Peut expliquer la vérification de fermeture de map

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

**Résultat** : Niveau 1 maîtrisé - Hugo peut expliquer le flow complet

---

## 🎓 Objectif Final

Connaître le programme **parfaitement** et être capable d'expliquer chaque ligne de code comme si je l'avais écrit moi-même lors de l'évaluation par les pairs.
