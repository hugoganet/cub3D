#include "cub3d.h"
#include "libft.h"

int parse_rgb_values(const char *rgb_str, t_color *color)
{
	char **rgb_parts;
	int r, g, b;

	// Diviser la chaîne par les virgules
	rgb_parts = ft_split(rgb_str, ',');
	if (!rgb_parts)
		return (1);

	// Vérifier qu'on a exactement 3 valeurs
	int count = 0;
	while (rgb_parts[count])
		count++;

	if (count != 3)
	{
		free_split(rgb_parts);
		return (1);
	}

	// Convertir en entiers et valider (0-255)
	r = ft_atoi(rgb_parts[0]);
	g = ft_atoi(rgb_parts[1]);
	b = ft_atoi(rgb_parts[2]);

	free_split(rgb_parts);

	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (1);

	// Stocker les valeurs
	color->r = r;
	color->g = g;
	color->b = b;

	return (0);
}

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

int parse_color_line(t_app *app, char *line)
{
	char *rgb_str;
	t_color color;

	// Extraire la partie RGB de la ligne
	rgb_str = extract_rgb_string(line);
	if (!rgb_str)
		error_exit(app, "Memory allocation failed");

	// Parser les valeurs RGB
	if (parse_rgb_values(rgb_str, &color) != 0)
	{
		free(rgb_str);
		error_exit(app, "Invalid RGB values (must be 0-255)");
	}

	// Déterminer si c'est Floor ou Ceiling
	if (ft_strncmp(line, "F ", 2) == 0)
	{
		app->floor = color;
		printf("✓ Floor color: R=%d, G=%d, B=%d\n", color.r, color.g, color.b);
	}
	else if (ft_strncmp(line, "C ", 2) == 0)
	{
		app->ceil = color;
		printf("✓ Ceiling color: R=%d, G=%d, B=%d\n", color.r, color.g, color.b);
	}
	else
	{
		free(rgb_str);
		error_exit(app, "Invalid color line format");
	}

	free(rgb_str);
	return (0);
}
