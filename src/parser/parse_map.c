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

    printf("🔍 Counting map lines...\n");

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("  COUNT: line='%s' (len=%d)\n", line, (int)ft_strlen(line));

        // Skip empty lines
        if (ft_strlen(line) == 0 || line[0] == '\n')
        {
            printf("  → SKIPPING empty line\n");
            free(line);
            continue;
        }

        // Check if we've reached the map section
        if (is_map_line(line))
        {
            map_started = 1;
            map_lines++;
            printf("  → MAP LINE %d: '%s'\n", map_lines, line);
        }
        else if (map_started)
        {
            printf("  → END of map section\n");
            free(line);
            break;
        }
        else
        {
            printf("  → NON-MAP line\n");
        }

        free(line);
    }

    close(fd);
    printf("✓ Total map lines counted: %d\n", map_lines);
    return (map_lines);
}

int init_map(t_app *app)
{
    // PAS de count_map_lines() ! On va allouer dynamiquement

    // Commencer avec une taille raisonnable
    app->map.height = 0;
    app->map.width = 0;
    app->map.grid = NULL; // Sera alloué dans add_map_line

    printf("✓ Map initialized: dynamic allocation\n");
    return (0);
}

int add_map_line(t_app *app, char *line, int line_index)
{
    int len;
    char *map_line;
    char **new_grid;
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

    // Réallouer le tableau si nécessaire
    new_grid = realloc(app->map.grid, sizeof(char *) * (line_index + 2));
    if (!new_grid)
        error_exit(app, "Memory reallocation failed for map");

    app->map.grid = new_grid;
    app->map.grid[line_index] = map_line;
    app->map.grid[line_index + 1] = NULL; // Terminer avec NULL

    // Mettre à jour les dimensions
    app->map.height = line_index + 1;
    if (len > app->map.width)
        app->map.width = len;

    printf("✓ Added map line %d: '%s' (len=%d)\n", line_index, map_line, len);
    return (0);
}
