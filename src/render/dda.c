#include "cub3d.h"
#include <math.h>

int cast_minimap_ray(t_app *app, t_vec2 ray_dir, t_vec2 *hit_point)
{
    int map_x = (int)app->player.pos.x;
    int map_y = (int)app->player.pos.y;

    double delta_dist_x = (ray_dir.x == 0) ? 1e30 : fabs(1 / ray_dir.x);
    double delta_dist_y = (ray_dir.y == 0) ? 1e30 : fabs(1 / ray_dir.y);

    int step_x;
    int step_y;
    double side_dist_x;
    double side_dist_y;

    if (ray_dir.x < 0)
    {
        step_x = -1;
        side_dist_x = (app->player.pos.x - map_x) * delta_dist_x;
    }
    else
    {
        step_x = 1;
        side_dist_x = (map_x + 1.0 - app->player.pos.x) * delta_dist_x;
    }

    if (ray_dir.y < 0)
    {
        step_y = -1;
        side_dist_y = (app->player.pos.y - map_y) * delta_dist_y;
    }
    else
    {
        step_y = 1;
        side_dist_y = (map_y + 1.0 - app->player.pos.y) * delta_dist_y;
    }

    int hit = 0;
    int side;

    while (hit == 0)
    {
        if (side_dist_x < side_dist_y)
        {
            side_dist_x += delta_dist_x;
            map_x += step_x;
            side = 0;
        }
        else
        {
            side_dist_y += delta_dist_y;
            map_y += step_y;
            side = 1;
        }

        if (map_x < 0 || map_y < 0 || map_x >= app->map.width ||
            map_y >= app->map.height)
            hit = 1;
        else if (app->map.grid[map_y] && map_x < (int)ft_strlen(app->map.grid[map_y]) &&
                 app->map.grid[map_y][map_x] == '1')
            hit = 1;
    }

    double perp_wall_dist;
    if (side == 0)
        perp_wall_dist = (map_x - app->player.pos.x + (1 - step_x) / 2) / ray_dir.x;
    else
        perp_wall_dist = (map_y - app->player.pos.y + (1 - step_y) / 2) / ray_dir.y;

    hit_point->x = app->player.pos.x + perp_wall_dist * ray_dir.x;
    hit_point->y = app->player.pos.y + perp_wall_dist * ray_dir.y;

    return (1);
}
