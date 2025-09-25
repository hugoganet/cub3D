#include "cub3d.h"
#include "libft.h"


// Fixed: Changed return type from void to int for proper error handling
// This allows caller to manage memory cleanup before error_exit
int parse_single_line(t_app *app, char *line, t_parse_counters *counters)
{
    // printf("DEBUG: Processing line: '%s' (len=%d)\n", line, (int)ft_strlen(line));

    // Parser les textures
    if (is_texture_line(line) && !counters->map_started)
    {
        parse_texture_line(app, line);
        counters->texture_count++;
    }
    // Parser les couleurs
    else if (is_color_line(line) && !counters->map_started)
    {
        parse_color_line(app, line);
        counters->color_count++;
    }
    // Parser la map
    else if (is_map_line(line))
    {
        // printf("DEBUG: Map line detected: '%s'\n", line);
        if (!counters->map_started)
        {
            counters->map_started = 1;
            init_map(app);
        }
        // printf("Map line: %s", line);
        add_map_line(app, line, counters->map_line_index);
        counters->map_line_index++;
    }
    else
    {
        printf("Unknown line format: %s", line);
        return (-1); // Return error instead of calling error_exit directly
    }
    return (0); // Success
}

int is_empty_line(char *line)
{
	return (ft_strlen(line) == 0 || line[0] == '\n');
}




/**
 * @brief Parse un fichier .cub et charge toutes les données de configuration
 *
 * Cette fonction lit et analyse un fichier .cub ligne par ligne pour extraire :
 * - Les chemins des 4 textures de murs (NO, SO, WE, EA)
 * - Les couleurs du sol (F) et du plafond (C) en format RGB
 * - La carte 2D avec les murs (1), espaces (0) et position du joueur (N/S/E/W)
 *
 * La fonction suit un ordre strict : textures et couleurs d'abord, puis la carte.
 * Elle valide que tous les éléments requis sont présents et bien formatés.
 *
 * @param app Pointeur vers la structure principale de l'application
 * @param path Chemin vers le fichier .cub à parser
 *
 * @return 0 en cas de succès
 *
 * @note En cas d'erreur, appelle error_exit() qui termine le programme
 * @note Alloue dynamiquement la mémoire pour stocker la carte 2D
 * @note Affiche des messages de debug pendant le parsing
 *
 * @see parse_texture_line() pour le parsing des textures
 * @see parse_color_line() pour le parsing des couleurs
 * @see init_map() et add_map_line() pour la gestion de la carte
 */

int parse_cub_file(t_app *app, const char *path)
{
	int fd;
	char *line;
	t_parse_counters counters = {0};
	// Ouvrir le fichier
	fd = open(path, O_RDONLY);
	if (fd < 0)
		error_exit(app, "Cannot open file");

   // Lire le fichier ligne par ligne
	while ((line = get_next_line(fd)) != NULL)
	{
		if (is_empty_line(line))
	    {
        	free(line);
        	continue;  // Ignorer complètement les lignes vides
    	}
		// Parse line and check for errors
		if (parse_single_line(app, line, &counters) != 0)
		{
			// Fixed: memory leak in error path - ensure line is freed before exit
			// Memory management principle: every malloc needs corresponding free
			free(line);          // Free current line returned by get_next_line
			gnl_free(NULL);      // Clean up get_next_line static buffer
			close(fd);           // Close file descriptor to avoid resource leak
			error_exit(app, "Invalid file format");
		}
		free(line);
	}
    gnl_free(NULL);
	close(fd);

	// Vérifier que tous les éléments sont présents
	if (counters.texture_count != 4)
		error_exit(app, "Missing texture definitions (need NO, SO, WE, EA)");

	if (counters.color_count != 2)
		error_exit(app, "Missing color definitions (need F and C)");

	if (!counters.map_started)
		error_exit(app, "No map found in file");

	return (0);
}
