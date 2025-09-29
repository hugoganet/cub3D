#include "cub3d.h"

int	cast_minimap_ray(t_app *app, t_vec2 ray_dir, t_vec2 *hit_point)
{
	t_ray_hit	hit;

	if (!cast_ray(app, ray_dir, &hit))
		return (0);
	hit_point->x = app->player.pos.x + hit.perp_dist * ray_dir.x;
	hit_point->y = app->player.pos.y + hit.perp_dist * ray_dir.y;
	return (1);
}
