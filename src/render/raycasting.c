#include "cub3d.h"
#include <math.h>

void calculate_ray_dir(t_app *app, int x, t_vec2 *ray_dir)
{
    // Calculate camera X position (from -1 to 1)
    double camera_x = 2 * x / (double)app->win_w - 1;

    // Calculate ray direction
    ray_dir->x = app->player.dir.x + app->player.plane.x * camera_x;
    ray_dir->y = app->player.dir.y + app->player.plane.y * camera_x;
}

int get_wall_side(int step_x, int step_y, int side)
{
    if (side == 0)
    {
        if (step_x > 0)
            return FACE_WEST;
        else
            return FACE_EAST;
    }
    else
    {
        if (step_y > 0)
            return FACE_NORTH;
        else
            return FACE_SOUTH;
    }
}

void render_3d_view(t_app *app)
{
    int x;
    t_vec2 ray_dir;
    t_ray_hit hit;

    // Check if textures are loaded
    if (!app->tex.loaded)
    {
        printf("⚠️ Textures not loaded, using solid colors\n");
        // Fallback to solid color rendering
        x = 0;
        while (x < app->win_w)
        {
            calculate_ray_dir(app, x, &ray_dir);
            if (cast_ray(app, ray_dir, &hit))
            {
                int wall_height = (int)calculate_wall_height(hit.perp_dist, app->win_h);
                int draw_start, draw_end;
                calculate_wall_bounds(wall_height, app->win_h, &draw_start, &draw_end);
                int color = get_wall_color(hit.wall_face);
                draw_wall_column(app, x, draw_start, draw_end, color);
            }
            x++;
        }
        return;
    }

    // Textured rendering
    x = 0;
    while (x < app->win_w)
    {
        // Calculate ray direction for this column
        calculate_ray_dir(app, x, &ray_dir);

        // Cast ray and get hit information
        if (cast_ray(app, ray_dir, &hit))
        {
            // Calculate wall height on screen
            int wall_height = (int)calculate_wall_height(hit.perp_dist, app->win_h);

            // Calculate top and bottom of wall strip
            int draw_start, draw_end;
            calculate_wall_bounds(wall_height, app->win_h, &draw_start, &draw_end);

            // Draw the textured vertical wall strip
            draw_textured_wall_column(app, x, draw_start, draw_end, &hit);
        }
        x++;
    }
}