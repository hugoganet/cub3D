/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncrivell <ncrivell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:18:41 by ncrivell          #+#    #+#             */
/*   Updated: 2025/09/29 13:22:29 by ncrivell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

/**
 * @brief Extrait le chemin de fichier depuis une ligne de configuration.
 *
 * Parcourt la ligne à partir du premier token (ex: "NO", "SO", ...) puis
 * saute les espaces et copie la séquence de caractères jusqu'au prochain
 * espace ou '\n' dans une nouvelle allocation.
 *
 * @param line Ligne d'entrée (ex: "NO ./path/to/texture.xpm\n").
 * @return char* Chaîne allouée contenant le chemin (NULL si allocation échoue).
 *               L'appelant est responsable de free().
 */
char	*extract_path(char *line)
{
	char	*path;
	int		i;
	int		j;
	int		start;

	i = 0;
	while (line[i] && line[i] != ' ')
		i++;
	while (line[i] && line[i] == ' ')
		i++;
	start = i;
	while (line[i] && line[i] != '\n' && line[i] != ' ')
		i++;
	path = malloc(i - start + 1);
	if (!path)
		return (NULL);
	j = 0;
	while (start < i)
		path[j++] = line[start++];
	path[j] = '\0';
	return (path);
}

static int	set_texture_path(char **dest, char *path)
{
	if (*dest)
		return (-1);
	*dest = ft_strdup(path);
	return (0);
}

static int	assign_texture(t_app *app, char *line, char *path)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		return (set_texture_path(&app->tex.north_path, path));
	else if (ft_strncmp(line, "SO ", 3) == 0)
		return (set_texture_path(&app->tex.south_path, path));
	else if (ft_strncmp(line, "WE ", 3) == 0)
		return (set_texture_path(&app->tex.west_path, path));
	else if (ft_strncmp(line, "EA ", 3) == 0)
		return (set_texture_path(&app->tex.east_path, path));
	return (0);
}

int	parse_texture_line(t_app *app, char *line)
{
	char	*path;
	int		result;

	path = extract_path(line);
	if (!path)
		error_exit(app, "Memory allocation failed");
	result = assign_texture(app, line, path);
	free(path);
	if (result != 0)
		return (-1);
	return (0);
}
