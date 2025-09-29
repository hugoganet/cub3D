#include "cub3d.h"
#include <math.h>

void	calculate_ray_dir(t_app *app, int x, t_vec2 *ray_dir)
{
	double	camera_x;

	camera_x = 2 * x / (double)app->win_w - 1;
	ray_dir->x = app->player.dir.x + app->player.plane.x * camera_x;
	ray_dir->y = app->player.dir.y + app->player.plane.y * camera_x;
}

int	get_wall_side(int step_x, int step_y, int side)
{
	if (side == 0)
	{
		if (step_x > 0)
			return (FACE_WEST);
		else
			return (FACE_EAST);
	}
	else
	{
		if (step_y > 0)
			return (FACE_NORTH);
		else
			return (FACE_SOUTH);
	}
}

static void	render_solid_colors(t_app *app)
{
	int			x;
	t_vec2		ray_dir;
	t_ray_hit	hit;
	int			wall_height;
	int			draw_coords[2];

	printf("⚠️ Textures not loaded, using solid colors\n");
	x = 0;
	while (x < app->win_w)
	{
		calculate_ray_dir(app, x, &ray_dir);
		if (cast_ray(app, ray_dir, &hit))
		{
			wall_height = (int)calculate_wall_height(hit.perp_dist, app->win_h);
			calculate_wall_bounds(wall_height, app->win_h, &draw_coords[0],
				&draw_coords[1]);
			draw_wall_column(app, x, draw_coords[0], draw_coords[1],
				get_wall_color(hit.wall_face));
		}
		x++;
	}
}

static void	render_textured_walls(t_app *app)
{
	int			x;
	t_vec2		ray_dir;
	t_ray_hit	hit;
	int			wall_height;
	int			draw_coords[2];

	x = 0;
	while (x < app->win_w)
	{
		calculate_ray_dir(app, x, &ray_dir);
		if (cast_ray(app, ray_dir, &hit))
		{
			wall_height = (int)calculate_wall_height(hit.perp_dist, app->win_h);
			calculate_wall_bounds(wall_height, app->win_h, &draw_coords[0],
				&draw_coords[1]);
			draw_textured_wall_column(app, x, draw_coords[0], draw_coords[1],
				&hit);
		}
		x++;
	}
}

void	render_3d_view(t_app *app)
{
	if (!app->tex.loaded)
		render_solid_colors(app);
	else
		render_textured_walls(app);
}
