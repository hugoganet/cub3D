/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncrivell <ncrivell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:02:24 by ncrivell          #+#    #+#             */
/*   Updated: 2025/09/29 13:18:07 by ncrivell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

// map.grid sera alloue dans add_map_line
int	init_map(t_app *app)
{
	app->map.height = 0;
	app->map.width = 0;
	app->map.grid = NULL;
	return (0);
}

int	get_trimmed_len(char *line)
{
	int	len;

	if (!line)
		return (0);
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		len--;
	return (len);
}

char	*dup_map_line(t_app *app, char *line, int len)
{
	char	*map_line;
	int		i;

	map_line = malloc(len + 1);
	if (!map_line)
		error_exit(app, "Memory allocation failed for map line");
	i = 0;
	while (i < len)
	{
		map_line[i] = line[i];
		i++;
	}
	map_line[i] = '\0';
	return (map_line);
}

char	**ensure_grid_capacity(t_app *app, char **old_grid, int needed)
{
	char	**new_grid;

	new_grid = realloc(old_grid, sizeof(char *) * needed);
	if (!new_grid)
		error_exit(app, "Memory reallocation failed for map");
	return (new_grid);
}

/**
 * @brief Ajoute une ligne formatée à la grille de la map.
 *
 * Cette version refactorisée utilise des helpers pour clarifier les étapes :
 * - get_trimmed_len() : calcule la longueur effective sans '\n'
 * - dup_map_line() : duplique la ligne dans une allocation propre
 * - ensure_grid_capacity() : réalloue le tableau app->map.grid si nécessaire
 *
 * La fonction insère la nouvelle ligne à l'index spécifié, termine la grille
 * par NULL, met à jour app->map.height et adapte app->map.width si la ligne
 * est plus large que la largeur courante.
 *
 * En cas d'allocation échouée, les helpers appellent error_exit() (arrêt).
 *
 * @param app Pointeur vers la structure principale contenant app->map.
 * @param line Chaîne à ajouter (peut contenir un '\n' final).
 * @param line_index Index 0-based où insérer la ligne dans la grille.
 * @return int 0 si succès, 1 si line == NULL (erreur non-fatale).
 */
int	add_map_line(t_app *app, char *line, int line_index)
{
	int		len;
	char	*map_line;
	char	**new_grid;

	if (!line)
		return (1);
	len = get_trimmed_len(line);
	map_line = dup_map_line(app, line, len);
	new_grid = ensure_grid_capacity(app, app->map.grid, line_index + 2);
	app->map.grid = new_grid;
	app->map.grid[line_index] = map_line;
	app->map.grid[line_index + 1] = NULL;
	app->map.height = line_index + 1;
	if (len > app->map.width)
		app->map.width = len;
	return (0);
}
