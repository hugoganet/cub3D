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

/**
 * @brief Parse une ligne de texture et stocke le chemin dans app->tex.
 *
 * Utilise extract_path() pour récupérer le chemin, duplique la chaîne
 * avec ft_strdup() et la place dans le champ correspondant (north/south/…).
 * En cas d'échec d'allocation, appelle error_exit().
 *
 * @param app  Pointeur vers la structure principale où stocker le chemin.
 * @param line Ligne à parser (doit commencer par "NO ", "SO ", "WE " ou "EA ").
 * @return int 0 en cas de succès (le chemin est dupliqué et stocké).
 */
int	parse_texture_line(t_app *app, char *line)
{
	char	*path;

	path = extract_path(line);
	if (!path)
		error_exit(app, "Memory allocation failed");
	if (ft_strncmp(line, "NO ", 3) == 0)
	{
		app->tex.north_path = ft_strdup(path);
	}
	else if (ft_strncmp(line, "SO ", 3) == 0)
	{
		app->tex.south_path = ft_strdup(path);
	}
	else if (ft_strncmp(line, "WE ", 3) == 0)
	{
		app->tex.west_path = ft_strdup(path);
	}
	else if (ft_strncmp(line, "EA ", 3) == 0)
	{
		app->tex.east_path = ft_strdup(path);
	}
	free(path);
	return (0);
}
