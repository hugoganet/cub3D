/**
 * @file background.c
 * @brief Rendu du sol et du plafond avec les couleurs parsées
 *
 * Ce fichier gère le rendu de l'arrière-plan de la vue 3D :
 * - Plafond (moitié haute) avec la couleur C
 * - Sol (moitié basse) avec la couleur F
 */

#include "cub3d.h"

/**
 * @brief Convertit une couleur t_color en entier RGB pour MLX
 *
 * @param color Structure couleur avec composantes r, g, b
 * @return Entier RGB au format 0xRRGGBB pour MLX
 */
int color_to_int(t_color color)
{
    return ((color.r << 16) | (color.g << 8) | color.b);
}

/**
 * @brief Dessine le plafond (moitié haute de l'écran)
 *
 * @param app Structure principale contenant la couleur du plafond
 */
void draw_ceiling(t_app *app)
{
    int ceiling_color = color_to_int(app->ceil);
    int y = 0;
    int half_height = DEFAULT_WIN_HEIGHT / 2;

    while (y < half_height)
    {
        draw_rect(app, 0, y, DEFAULT_WIN_WIDTH, 1, ceiling_color);
        y++;
    }

    // printf("🔵 Ceiling drawn with color RGB(%d,%d,%d)\n",
    //        app->ceil.r, app->ceil.g, app->ceil.b);
}

/**
 * @brief Dessine le sol (moitié basse de l'écran)
 *
 * @param app Structure principale contenant la couleur du sol
 */
void draw_floor(t_app *app)
{
    int floor_color = color_to_int(app->floor);
    int y = DEFAULT_WIN_HEIGHT / 2;

    while (y < DEFAULT_WIN_HEIGHT)
    {
        draw_rect(app, 0, y, DEFAULT_WIN_WIDTH, 1, floor_color);
        y++;
    }

    // printf("🟤 Floor drawn with color RGB(%d,%d,%d)\n",
    //        app->floor.r, app->floor.g, app->floor.b);
}

/**
 * @brief Dessine le background complet (sol + plafond)
 *
 * Cette fonction est appelée avant de dessiner les murs 3D
 * pour établir l'arrière-plan de la scène.
 *
 * @param app Structure principale
 */
void render_background(t_app *app)
{
    draw_ceiling(app);
    draw_floor(app);

    // printf("🎨 Background rendered successfully\n");
}
