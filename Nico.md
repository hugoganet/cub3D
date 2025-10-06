# Nico's cub3D Learning Journey

## Current Focus
Niveau 2️⃣ : Parsing & Validation - Maîtriser l'entrée du programme : lecture, validation, gestion d'erreurs

## Mastered Concepts
✅ Flow général du programme (parsing → init MLX → boucle de jeu → cleanup)
✅ Principe du raycasting (1 rayon = 1 colonne, distance ∝ hauteur inverse)
✅ Mathématiques caméra (vecteurs dir/plane, formule camera_x, ray_direction)
✅ Structures principales (t_app, t_player, t_vec2)

## Areas Needing Reinforcement
(À identifier selon le plan d'apprentissage)

## Custom Metaphors & Analogies Used
- 🔦 Lampe de poche : Un rayon éclaire une direction, mais l'œil voit un champ de vision → besoin de multiples rayons
- 📐 Éventail de rayons : 1024 rayons = 1024 colonnes, mais ils pointent dans des directions différentes
- 🎯 Rayon central + espacement régulier : dir = centre, puis répartition homogène gauche/droite
- 📺 Écran de cinéma : dir = centre écran, plane = largeur écran (perpendiculaire à dir)
- 🎛️ Coefficient magique : Transformer colonne (0→1023) en coefficient (-1→+1) pour ray_direction
- 🏰 Map fermée = Bordures (première/dernière ligne + début/fin lignes) + vérification voisins du joueur

## Evaluation Readiness
(Will assess readiness per EVALUATION.md sections)

## Session Notes
- First session: Creating learning journal and establishing baseline understanding
- Évaluation initiale : Nico comprend le flow général (parsing → MLX → jeu) mais doit approfondir l'initialisation MLX et la boucle de jeu
- Progression : Identifie la boucle MLX, le chargement de textures, et le concept de raycasting "adaptant les pixels"
- EXCELLENT : Explique parfaitement le principe du raycasting (illusion 3D, rayons depuis joueur, 1 rayon = 1 colonne, distance ∝ hauteur inverse)
- ✅ BREAKTHROUGH : Découverte autonome de la formule magique camera_x = 2*x/width-1 et ray_dir = dir + plane*camera_x
- ✅ MAÎTRISÉ : Vecteurs dir/plane, champ de vision, calcul direction des rayons
- Parsing : Comprend la logique de validation map fermée (bordures + voisins du joueur)
- Découverte : Son implémentation est plus sophistiquée que l'approche basique début/fin de ligne
- ✅ DDA revu : Comprend init_dda_vars, boucle DDA, check_wall_hit, correction de distance perpendiculaire, calcul de wall_x et détermination des faces
- Prochaine étape : Projection 3D & rendu (hauteur du mur → draw bounds → textures)
