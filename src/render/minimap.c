#include "cub3d.h"
#include "libft.h"

void draw_minimap_tile(t_app *app, int map_x, int map_y, int color)
{
    int screen_x = MINIMAP_OFFSET_X + map_x * MINIMAP_SCALE;
    int screen_y = MINIMAP_OFFSET_Y + map_y * MINIMAP_SCALE;

    // Dessiner la case
    draw_rect(app, screen_x, screen_y, MINIMAP_SCALE, MINIMAP_SCALE, color);

    // Dessiner la bordure
    draw_rect(app, screen_x, screen_y, MINIMAP_SCALE, 1, COLOR_BORDER);  // Top
    draw_rect(app, screen_x, screen_y, 1, MINIMAP_SCALE, COLOR_BORDER);  // Left
}

void draw_player_on_minimap(t_app *app)
{
    int player_screen_x = MINIMAP_OFFSET_X + (int)(app->player.pos.x * MINIMAP_SCALE) - 3;
    int player_screen_y = MINIMAP_OFFSET_Y + (int)(app->player.pos.y * MINIMAP_SCALE) - 3;

    // Dessiner le joueur comme un carré rouge 6x6
    draw_rect(app, player_screen_x, player_screen_y, 6, 6, COLOR_PLAYER);

    // Dessiner la direction (petite ligne)
    int dir_end_x = player_screen_x + 3 + (int)(app->player.dir.x * 10);
    int dir_end_y = player_screen_y + 3 + (int)(app->player.dir.y * 10);

    // Simple ligne pour la direction (approximative)
    draw_rect(app, dir_end_x, dir_end_y, 2, 2, 0x00FF00);  // Vert pour la direction
}

void render_minimap(t_app *app)
{
    int x, y;

    // Dessiner un cadre autour de la minimap
    draw_rect(app, MINIMAP_OFFSET_X - 2, MINIMAP_OFFSET_Y - 2,
              app->map.width * MINIMAP_SCALE + 4,
              app->map.height * MINIMAP_SCALE + 4, 0xFFFFFF);

    y = 0;
    while (y < app->map.height)
    {
        x = 0;
        int line_len = app->map.grid[y] ? ft_strlen(app->map.grid[y]) : 0;

        while (x < line_len)
        {
            char tile = app->map.grid[y][x];
            int color;

            // Debug: couleurs très distinctes
            if (tile == '1')
                color = 0xFFFFFF;      // Blanc - murs
            else if (tile == '0')
                color = 0x404040;      // Gris - sol
            else if (tile == 'N' || tile == 'S' || tile == 'E' || tile == 'W')
                color = 0x0000FF;      // Bleu - position initiale joueur
            else if (tile == ' ')
                color = 0x000000;      // Noir - vide
            else
                color = 0xFF00FF;      // Magenta - erreur

            draw_minimap_tile(app, x, y, color);
            x++;
        }
        y++;
    }

    draw_player_on_minimap(app);
}
