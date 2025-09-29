#include "cub3d.h"
#include "libft.h"

/**
 * @brief Parse les valeurs RGB depuis une chaîne "R,G,B"
 * @param rgb_str Chaîne contenant "R,G,B"
 * @param color Structure t_color à remplir
 * @note On divise la chaîne par les virgules
 * @note On verifie qu'on a exactement 3 valeurs
 * @note On converti en entiers et valider (0-255)
 * @note On stocke les valeurs
 * @return 0 si succès, 1 si erreur
 */
int parse_rgb_values(const char *rgb_str, t_color *color)
{
	char **rgb_parts;
	int r, g, b;

	rgb_parts = ft_split(rgb_str, ',');
	if (!rgb_parts)
		return (1);
	int count = 0;
	while (rgb_parts[count])
		count++;
	if (count != 3)
	{
		free_split(rgb_parts);
		return (1);
	}
	r = ft_atoi(rgb_parts[0]);
	g = ft_atoi(rgb_parts[1]);
	b = ft_atoi(rgb_parts[2]);
	free_split(rgb_parts);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (1);
	color->r = r;
	color->g = g;
	color->b = b;
	return (0);
}

/**
 * @brief Extrait la partie RGB d'une ligne "F R,G,B" ou "C R,G,B"
 * @param line Ligne complète à parser
 * @return Chaîne "R,G,B" ou NULL si erreur
 */
char *extract_rgb_string(char *line)
{
	char *rgb_str;
	int i, j, start;

	// Ignorer le préfixe (F ou C) et les espaces
	i = 0;
	while (line[i] && line[i] != ' ')
		i++;
	while (line[i] && line[i] == ' ')
		i++;

	// Début des valeurs RGB
	start = i;

	// Trouver la fin (ignorer \n)
	while (line[i] && line[i] != '\n')
		i++;

	// Allouer et copier
	rgb_str = malloc(i - start + 1);
	if (!rgb_str)
		return (NULL);

	j = 0;
	while (start < i)
		rgb_str[j++] = line[start++];
	rgb_str[j] = '\0';

	return (rgb_str);
}


/**
 * @brief Parse une ligne de couleur complète (F ou C)
 * @param app Structure principale
 * @param line Ligne à parser ("F R,G,B" ou "C R,G,B")
 * @return 0 si succès, 1 si erreur
 */
int parse_color_line(t_app *app, char *line)
{
	char *rgb_str;
	t_color color;

	// Extraire la partie RGB de la ligne
	rgb_str = extract_rgb_string(line);
	if (!rgb_str)
		return (1); // Fixed: return error instead of calling error_exit directly

	// Parser les valeurs RGB
	if (parse_rgb_values(rgb_str, &color) != 0)
	{
		free(rgb_str); // Fixed: ensure cleanup before returning error
		return (1); // Fixed: return error instead of calling error_exit directly
	}

	// Déterminer si c'est Floor ou Ceiling
	if (ft_strncmp(line, "F ", 2) == 0)
	{
		app->floor = color;
	}
	else if (ft_strncmp(line, "C ", 2) == 0)
	{
		app->ceil = color;
	}
	else
	{
		free(rgb_str); // Fixed: ensure cleanup before returning error
		return (1); // Fixed: return error instead of calling error_exit directly
	}

	free(rgb_str);
	return (0);
}
