/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_player.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncrivell <ncrivell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:47:11 by ncrivell          #+#    #+#             */
/*   Updated: 2025/09/29 15:58:21 by ncrivell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

/**
 * @brief Traite une cellule pour détecter et initialiser le joueur.
 *
 * Si la cellule (i,j) contient un caractère de joueur (N/S/E/W) :
 * - incrémente le compteur de joueurs (*player_count),
 * - positionne app->player.pos au centre de la cellule,
 * - définit l'orientation du joueur via set_player_orientation().
 * Si la cellule ne contient pas un caractère de joueur, ne fait rien.
 *
 * @param app Pointeur vers la structure principale contenant la map et le player.
 * @param i   Indice de ligne (y) dans la grille.
 * @param j   Indice de colonne (x) dans la grille.
 * @param player_count Pointeur vers le compteur de joueurs trouvé jusqu'ici.
 */
void	process_cell_for_player(t_app *app, int i, int j, int *player_count)
{
	char	c;

	c = app->map.grid[i][j];
	if (!is_player_char(c))
		return ;
	(*player_count)++;
	app->player.pos.x = (double)j + 0.5;
	app->player.pos.y = (double)i + 0.5;
	set_player_orientation(app, c);
}

int	is_player_char(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

void	orientation_north_or_south(t_app *app, char c)
{
	if (c == 'N')
	{
		app->player.dir.x = 0;
		app->player.dir.y = -1;
		app->player.plane.x = 0.66;
		app->player.plane.y = 0;
	}
	else if (c == 'S')
	{
		app->player.dir.x = 0;
		app->player.dir.y = 1;
		app->player.plane.x = -0.66;
		app->player.plane.y = 0;
	}
}

void	orientation_east_or_west(t_app *app, char c)
{
	if (c == 'E')
	{
		app->player.dir.x = 1;
		app->player.dir.y = 0;
		app->player.plane.x = 0;
		app->player.plane.y = 0.66;
	}
	else if (c == 'W')
	{
		app->player.dir.x = -1;
		app->player.dir.y = 0;
		app->player.plane.x = 0;
		app->player.plane.y = -0.66;
	}
}

void	set_player_orientation(t_app *app, char c)
{
	if (c == 'N' || c == 'S')
		orientation_north_or_south(app, c);
	if (c == 'E' || c == 'W')
		orientation_east_or_west(app, c);
}
