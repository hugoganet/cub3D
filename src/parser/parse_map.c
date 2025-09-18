#include "cub3d.h"
#include "libft.h"

int count_map_lines(const char *path)
{
	int fd;
	char *line;
	int map_lines = 0;
	int map_started = 0;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (-1);

	while ((line = get_next_line(fd)) != NULL)
	{
		// Skip empty lines
		if (ft_strlen(line) == 0 || line[0] == '\n')
		{
			free(line);
			continue;
		}

		// Check if we've reached the map section
		if (is_map_line(line))
		{
			map_started = 1;
			map_lines++;
		}
		else if (map_started)
		{
			// If we were in map section and hit non-map line, stop counting
			free(line);
			break;
		}

		free(line);
	}

	close(fd);
	return (map_lines);
}

int init_map(t_app *app, const char *path)
{
	int map_lines;

	// Compter le nombre de lignes de la map
	map_lines = count_map_lines(path);
	if (map_lines <= 0)
		error_exit(app, "No valid map found");

	// Allouer le tableau de pointeurs pour les lignes
	app->map.grid = malloc(sizeof(char *) * (map_lines + 1));
	if (!app->map.grid)
		error_exit(app, "Memory allocation failed for map");

	// Initialiser
	app->map.height = map_lines;
	app->map.width = 0;  // Will be determined when we add lines

	// Initialiser tous les pointeurs à NULL
	for (int i = 0; i <= map_lines; i++)
		app->map.grid[i] = NULL;

	printf("✓ Map initialized: %d lines allocated\n", map_lines);
	return (0);
}

int add_map_line(t_app *app, char *line, int line_index)
{
	int len;
	char *map_line;
	int i, j;

	if (!line)
		return (1);

	// Calculer la longueur sans le \n
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		len--;

	// Allouer et copier la ligne
	map_line = malloc(len + 1);
	if (!map_line)
		error_exit(app, "Memory allocation failed for map line");

	// Copier en ignorant le \n
	j = 0;
	for (i = 0; i < len; i++)
		map_line[j++] = line[i];
	map_line[j] = '\0';

	// Stocker dans la map
	app->map.grid[line_index] = map_line;

	// Mettre à jour la largeur si nécessaire
	if (len > app->map.width)
		app->map.width = len;

	printf("✓ Added map line %d: '%s' (len=%d)\n", line_index, map_line, len);
	return (0);
}
