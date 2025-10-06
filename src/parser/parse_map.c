/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncrivell <ncrivell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:02:24 by ncrivell          #+#    #+#             */
/*   Updated: 2025/10/06 13:09:56 by ncrivell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Calcule la longueur de la chaîne en excluant le caractère '\n' final
 * s'il est présent. Utile pour normaliser les lignes de map avant copie.
 *
 * @param line Chaîne à mesurer (peut être NULL).
 * @return int Longueur de la ligne sans '\n', ou 0 si line est NULL.
 */
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

/**
 * @brief Duplique une ligne de map dans une allocation propre.
 *
 * Alloue un buffer de taille len+1 et copie exactement len caractères
 * depuis line. Termine la chaîne par '\0'. En cas d'échec d'allocation,
 * appelle error_msg() pour arrêter le programme proprement.
 *
 * @param app Pointeur vers la structure principale (pour error_msg).
 * @param line Chaîne source à dupliquer.
 * @param len Nombre de caractères à copier (excluant '\0').
 * @return char* Pointeur vers la nouvelle chaîne allouée.
 *
 */
char	*dup_map_line(char *line, int len)
{
	char	*map_line;
	int		i;

	map_line = ft_calloc(len + 1, sizeof(char));
	if (!map_line)
		return (NULL);
	i = 0;
	while (i < len)
	{
		map_line[i] = line[i];
		i++;
	}
	return (map_line);
}

/**
 * @brief Réalloue la grille de map pour garantir la capacité nécessaire.
 *
 * Alloue un nouveau tableau plus grand, copie les anciennes lignes,
 * puis libère l'ancien tableau. Équivalent à realloc() mais sans l'utiliser.
 *
 * @param old_grid Tableau actuel de pointeurs (peut être NULL au départ).
 * @param needed Nombre d'entrées minimum requises dans le tableau.
 * @return char** Pointeur vers le tableau réalloué.
 *
 */
char	**resize_map_grid(char **old_grid, int needed)
{
	char	**new_grid;
	int		i;

	new_grid = ft_calloc(sizeof(char *), needed);
	if (!new_grid)
	{
		error_msg("Memory allocation failed for map grid");
		return (NULL);
	}
	i = 0;
	if (old_grid)
	{
		while (old_grid[i])
		{
			new_grid[i] = old_grid[i];
			i++;
		}
		free(old_grid);
	}
	return (new_grid);
}

/**
 * @brief Ajoute une ligne formatée à la grille de la map.
 *
 * La fonction insère la nouvelle ligne à l'index spécifié, termine la grille
 * par NULL, met à jour app->map.height et adapte app->map.width si la ligne
 * est plus large que la largeur courante.
 *
 * En cas d'allocation échouée, les helpers appellent error_msg() (arrêt).
 *
 * @param app Pointeur vers la structure principale contenant app->map.
 * @param line Chaîne à ajouter (peut contenir un '\n' final).
 * @param line_index Index 0-based où insérer la ligne dans la grille.
 * @return int 0 si succès, -1 si line == NULL (erreur non-fatale).
 */
int	add_map_line(t_app *app, char *line, int line_index)
{
	int		len;
	char	*map_line;
	char	**new_grid;

	if (!line)
		return (error_msg("Null line passed to add_map_line"));
	len = get_trimmed_len(line);
	map_line = dup_map_line(line, len);
	if (!map_line)
		return (error_msg("Memory allocation failed for map line"));
	new_grid = resize_map_grid(app->map.grid, line_index + 2);
	if (!new_grid)
	{
		free(map_line);
		return (-1);
	}
	app->map.grid = new_grid;
	app->map.grid[line_index] = map_line;
	app->map.grid[line_index + 1] = NULL;
	app->map.height = line_index + 1;
	if (len > app->map.width)
		app->map.width = len;
	return (0);
}
