#include "cub3d.h"
#include "libft.h"

int parse_cub_file(t_app *app, const char *path)
{
	int fd;
	char *line;
	int line_count;
	int texture_count = 0;
	int color_count = 0;
	int map_started = 0;
	int map_line_index = 0;

	// Ouvrir le fichier
	fd = open(path, O_RDONLY);
	if (fd < 0)
		error_exit(app, "Cannot open file");

	// Compter les lignes pour allouer la map
	line_count = count_lines(path);
	if (line_count < 0)
		error_exit(app, "Error reading file");

   // Lire le fichier ligne par ligne
	while ((line = get_next_line(fd)) != NULL)
	{
		// Ignorer les lignes vides
		if (ft_strlen(line) == 0 || line[0] == '\n')
		{
			free(line);
			continue;
		}

		// Parser les textures
		if (is_texture_line(line) && !map_started)
		{
			printf("Found texture: %s", line);
			parse_texture_line(app, line);
			texture_count++;
		}
		// Parser les couleurs
		else if (is_color_line(line) && !map_started)
		{
			printf("Found color: %s", line);
			parse_color_line(app, line);
			color_count++;
		}
		// Parser la map
		else if (is_map_line(line))
		{
			if (!map_started)
			{
				printf("Map section started\n");
				map_started = 1;
				// TODO 3: Initialiser la map ← REMPLACER PAR :
				init_map(app, path);
			}
			printf("Map line: %s", line);
			// TODO 4: Ajouter cette ligne à la map ← REMPLACER PAR :
			add_map_line(app, line, map_line_index);
			map_line_index++;  // ← Incrémenter l'index
		}
		else
		{
			printf("Unknown line format: %s", line);
			free(line);
			close(fd);
			error_exit(app, "Invalid file format");
		}

		free(line);
	}

	close(fd);

	// Vérifier que tous les éléments sont présents
	if (texture_count != 4)
		error_exit(app, "Missing texture definitions (need NO, SO, WE, EA)");

	if (color_count != 2)
		error_exit(app, "Missing color definitions (need F and C)");

	if (!map_started)
		error_exit(app, "No map found in file");

	printf("✓ File parsed successfully!\n");
	printf("✓ Map size: %dx%d\n", app->map.width, app->map.height);
	return (0);
}
