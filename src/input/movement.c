#include "cub3d.h"

// Fonction pour vérifier si une position est valide (pas dans un mur)
bool is_valid_position(t_app *app, double x, double y)
{
	int map_x = (int)x;
	int map_y = (int)y;

	// Vérifier les limites de la map
	if (map_x < 0 || map_y < 0 || map_y >= app->map.height)
		return (false);

	// Vérifier que la ligne existe
	if (!app->map.grid[map_y])
		return (false);

	// Vérifier la largeur de cette ligne
	if (map_x >= (int)ft_strlen(app->map.grid[map_y]))
		return (false);

	// Vérifier que ce n'est pas un mur
	char tile = app->map.grid[map_y][map_x];
	if (tile == '1')
		return (false);

	return (true);  // Position valide
}



/**
 * @brief Fait tourner le joueur sur lui-même selon l'angle spécifié
 *
 * Cette fonction applique une rotation matricielle 2D au vecteur de direction
 * du joueur et à son plan de caméra. Elle utilise les formules de rotation :
 * - new_x = x * cos(θ) - y * sin(θ)
 * - new_y = x * sin(θ) + y * cos(θ)
 * L'angle de rotation est calculé en multipliant rot_speed par la direction.
 *
 * @param app Pointeur vers la structure principale de l'application
 * @param direction Sens de rotation (-1 = gauche/antihoraire, 1 = droite/horaire)
 *
 * @note Modifie à la fois player.dir (direction de vue) et player.plane (plan caméra)
 * @note L'angle de rotation dépend de app->rot_speed défini dans init_defaults()
 * @note Nécessite #include <math.h> pour cos() et sin()
 */
void rotate_player(t_app *app, int direction)
{
	// direction: -1 = gauche, 1 = droite
	double angle = app->rot_speed * direction;

	// Sauvegarder les anciennes valeurs
	double old_dir_x = app->player.dir.x;
	double old_plane_x = app->player.plane.x;

	// Rotation de la direction
	app->player.dir.x = app->player.dir.x * cos(angle) - app->player.dir.y * sin(angle);
	app->player.dir.y = old_dir_x * sin(angle) + app->player.dir.y * cos(angle);

	// Rotation du plan de caméra
	app->player.plane.x = app->player.plane.x * cos(angle) - app->player.plane.y * sin(angle);
	app->player.plane.y = old_plane_x * sin(angle) + app->player.plane.y * cos(angle);

}

/**
 * @brief Déplace le joueur selon un vecteur de mouvement donné
 *
 * Fonction unifiée qui applique un déplacement en vérifiant les collisions
 * séparément sur les axes X et Y. Cela permet le "wall sliding" : si le joueur
 * ne peut pas bouger dans une direction, il peut encore bouger dans l'autre.
 *
 * @param app Pointeur vers la structure principale de l'application
 * @param move_x Composante X du vecteur de mouvement (normalisée)
 * @param move_y Composante Y du vecteur de mouvement (normalisée)
 *
 * @note Le vecteur de mouvement est multiplié par app->move_speed
 * @note Les collisions sont vérifiées avec is_valid_position()
 */
void move_player(t_app *app, double move_x, double move_y)
{
	double new_x = app->player.pos.x + move_x * app->move_speed;
	double new_y = app->player.pos.y + move_y * app->move_speed;

	// Vérifier collision X séparément
	if (is_valid_position(app, new_x, app->player.pos.y))
		app->player.pos.x = new_x;

	// Vérifier collision Y séparément
	if (is_valid_position(app, app->player.pos.x, new_y))
		app->player.pos.y = new_y;
}

/**
 * @brief Traite tous les mouvements du joueur selon l'état des touches pressées
 *
 * Fonction principale de gestion des déplacements qui :
 * 1. Calcule le vecteur de mouvement résultant en combinant toutes les touches actives
 * 2. Applique les déplacements linéaires (WASD) si nécessaire
 * 3. Applique les rotations (flèches ←→) si nécessaire
 *
 * Les mouvements diagonaux sont supportés (ex: W+D = avant+droite simultanés).
 * Cette version unifiée remplace les appels séparés aux fonctions de mouvement.
 *
 * @param app Pointeur vers la structure principale de l'application
 *
 * @note Appelée à chaque frame dans la boucle de jeu (app_loop)
 * @note Utilise move_player() pour les déplacements et rotate_player() pour les rotations
 * @note Les touches sont lues depuis app->keys (état persistant press/release)
 */
void update_player_movement(t_app *app)
{
	// Calculer les vecteurs de déplacement
	double move_x = 0;
	double move_y = 0;

	// Avant/Arrière
	if (app->keys.w)
	{
		move_x += app->player.dir.x;
		move_y += app->player.dir.y;
	}
	if (app->keys.s)
	{
		move_x -= app->player.dir.x;
		move_y -= app->player.dir.y;
	}

	// Gauche/Droite (strafe)
	if (app->keys.a)
	{
		move_x += app->player.dir.y;
		move_y -= app->player.dir.x;
	}
	if (app->keys.d)
	{
		move_x -= app->player.dir.y;
		move_y += app->player.dir.x;
	}

	// Appliquer le mouvement
	if (move_x != 0 || move_y != 0)
		move_player(app, move_x, move_y);

	// NOUVEAU: Rotation avec les flèches
	if (app->keys.left)
		rotate_player(app, -1);  // Tourner à gauche
	if (app->keys.right)
		rotate_player(app, 1);   // Tourner à droite
}
