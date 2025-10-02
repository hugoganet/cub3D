/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:37 by hugoganet         #+#    #+#             */
/*   Updated: 2025/09/29 13:32:38 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"
#include <math.h>

/**
 * @brief Dessine une tuile de la minimap avec bordures
 *
 * Convertit les coordonnées de map en coordonnées écran (avec mise à
 * l'échelle MINIMAP_SCALE) et dessine un carré coloré avec bordures.
 * Chaque tuile représente une case de la map.
 *
 * @param app Structure principale de l'application
 * @param map_x Coordonnée X dans la grille de map
 * @param map_y Coordonnée Y dans la grille de map
 * @param color Couleur de remplissage de la tuile
 */
void	draw_minimap_tile(t_app *app, int map_x, int map_y, int color)
{
	int	screen_x;
	int	screen_y;
	int	params1[4];
	int	params2[4];
	int	params3[4];

	screen_x = MINIMAP_OFFSET_X + map_x * MINIMAP_SCALE;
	screen_y = MINIMAP_OFFSET_Y + map_y * MINIMAP_SCALE;
	params1[0] = screen_x;
	params1[1] = screen_y;
	params1[2] = MINIMAP_SCALE;
	params1[3] = MINIMAP_SCALE;
	params2[0] = screen_x;
	params2[1] = screen_y;
	params2[2] = MINIMAP_SCALE;
	params2[3] = 1;
	params3[0] = screen_x;
	params3[1] = screen_y;
	params3[2] = 1;
	params3[3] = MINIMAP_SCALE;
	draw_rect(app, params1, color);
	draw_rect(app, params2, COLOR_BORDER);
	draw_rect(app, params3, COLOR_BORDER);
}

/**
 * @brief Dessine le joueur et sa direction sur la minimap
 *
 * Affiche un petit carré représentant la position du joueur et un
 * point vert indiquant sa direction de vue. La position est convertie
 * des coordonnées monde vers les coordonnées minimap.
 *
 * @param app Structure principale de l'application
 */
void	draw_player_on_minimap(t_app *app)
{
	int	player_params[4];
	int	dir_params[4];

	player_params[0] = MINIMAP_OFFSET_X + (int)(app->player.pos.x
			* MINIMAP_SCALE) - 3;
	player_params[1] = MINIMAP_OFFSET_Y + (int)(app->player.pos.y
			* MINIMAP_SCALE) - 3;
	player_params[2] = 6;
	player_params[3] = 6;
	draw_rect(app, player_params, COLOR_PLAYER);
	dir_params[0] = player_params[0] + 3 + (int)(app->player.dir.x * 10);
	dir_params[1] = player_params[1] + 3 + (int)(app->player.dir.y * 10);
	dir_params[2] = 2;
	dir_params[3] = 2;
	draw_rect(app, dir_params, 0x00FF00);
}

/**
 * @brief Rendu complet de la minimap overlay
 *
 * Dessine tous les éléments de la minimap :
 * - Bordure blanche
 * - Grille de tuiles colorées (murs, sol, spawn)
 * - Rayons du champ de vision (vert)
 * - Indicateur du joueur et direction
 *
 * @param app Structure principale de l'application
 */
void	render_minimap(t_app *app)
{
	int		x;
	int		y;
	int		line_len;

	draw_minimap_border(app);
	y = 0;
	while (y < app->map.height)
	{
		x = 0;
		line_len = 0;
		if (app->map.grid[y])
			line_len = ft_strlen(app->map.grid[y]);
		while (x < line_len)
		{
			draw_minimap_tile(app, x, y, get_tile_color(app->map.grid[y][x]));
			x++;
		}
		y++;
	}
	render_minimap_rays(app);
	draw_player_on_minimap(app);
}

/**
 * @brief Calcule la direction d'un rayon pour la minimap
 *
 * Détermine la direction d'un rayon en fonction de son index dans le FOV.
 * Utilise un FOV de 60° (π/3) et interpole l'angle par rapport à la
 * direction du joueur.
 *
 * @param app Structure principale de l'application
 * @param ray_index Index du rayon courant (0 à total_rays-1)
 * @param total_rays Nombre total de rayons à tracer
 * @param ray_dir Pointeur vers le vecteur de direction résultant
 */
void	get_ray_direction(t_app *app, int ray_index, int total_rays,
		t_vec2 *ray_dir)
{
	double	fov_radians;
	double	angle_step;
	double	ray_angle;
	double	player_angle;
	double	final_angle;

	fov_radians = M_PI / 3.0;
	angle_step = fov_radians / (total_rays - 1);
	ray_angle = -fov_radians / 2.0 + ray_index * angle_step;
	player_angle = atan2(app->player.dir.y, app->player.dir.x);
	final_angle = player_angle + ray_angle;
	ray_dir->x = cos(final_angle);
	ray_dir->y = sin(final_angle);
}

/**
 * @brief Dessine les rayons du champ de vision sur la minimap
 *
 * Trace MINIMAP_RAY_COUNT rayons (20 par défaut) du joueur vers les
 * murs visibles. Chaque rayon est calculé via DDA et affiché en vert
 * pour visualiser le FOV.
 *
 * @param app Structure principale de l'application
 */
void	render_minimap_rays(t_app *app)
{
	int		i;
	t_vec2	ray_dir;
	t_vec2	hit_point;

	i = 0;
	while (i < MINIMAP_RAY_COUNT)
	{
		get_ray_direction(app, i, MINIMAP_RAY_COUNT, &ray_dir);
		if (cast_minimap_ray(app, ray_dir, &hit_point))
			draw_minimap_ray(app, hit_point);
		i++;
	}
}
