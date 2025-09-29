#include "cub3d.h"
#include <math.h>

static void	init_dda_vars(t_app *app, t_vec2 ray_dir, t_ray_vars *vars)
{
	vars->map_x = (int)app->player.pos.x;
	vars->map_y = (int)app->player.pos.y;
	if (ray_dir.x == 0)
		vars->delta_dist_x = 1e30;
	else
		vars->delta_dist_x = fabs(1 / ray_dir.x);
	if (ray_dir.y == 0)
		vars->delta_dist_y = 1e30;
	else
		vars->delta_dist_y = fabs(1 / ray_dir.y);
	if (ray_dir.x < 0)
	{
		vars->step_x = -1;
		vars->side_dist_x = (app->player.pos.x - vars->map_x)
			* vars->delta_dist_x;
	}
	else
	{
		vars->step_x = 1;
		vars->side_dist_x = (vars->map_x + 1.0 - app->player.pos.x)
			* vars->delta_dist_x;
	}
}

static void	init_dda_vars_y(t_app *app, t_vec2 ray_dir, t_ray_vars *vars)
{
	if (ray_dir.y < 0)
	{
		vars->step_y = -1;
		vars->side_dist_y = (app->player.pos.y - vars->map_y)
			* vars->delta_dist_y;
	}
	else
	{
		vars->step_y = 1;
		vars->side_dist_y = (vars->map_y + 1.0 - app->player.pos.y)
			* vars->delta_dist_y;
	}
}

static int	check_wall_hit(t_app *app, t_ray_vars *vars)
{
	if (vars->map_x < 0 || vars->map_y < 0 || vars->map_x >= app->map.width
		|| vars->map_y >= app->map.height)
		return (1);
	if (app->map.grid[vars->map_y]
		&& vars->map_x < (int)ft_strlen(app->map.grid[vars->map_y])
		&& app->map.grid[vars->map_y][vars->map_x] == '1')
		return (1);
	return (0);
}

static void	fill_hit_info(t_app *app, t_vec2 ray_dir, t_ray_vars *vars,
		t_ray_hit *hit)
{
	if (vars->side == 0)
		hit->perp_dist = (vars->map_x - app->player.pos.x
				+ (1 - vars->step_x) / 2) / ray_dir.x;
	else
		hit->perp_dist = (vars->map_y - app->player.pos.y
				+ (1 - vars->step_y) / 2) / ray_dir.y;
	hit->side = vars->side;
	hit->map_x = vars->map_x;
	hit->map_y = vars->map_y;
	if (vars->side == 0 && vars->step_x > 0)
		hit->wall_face = FACE_WEST;
	else if (vars->side == 0)
		hit->wall_face = FACE_EAST;
	else if (vars->step_y > 0)
		hit->wall_face = FACE_NORTH;
	else
		hit->wall_face = FACE_SOUTH;
	if (vars->side == 0)
		hit->wall_x = app->player.pos.y + hit->perp_dist * ray_dir.y;
	else
		hit->wall_x = app->player.pos.x + hit->perp_dist * ray_dir.x;
	hit->wall_x -= floor(hit->wall_x);
}

int	cast_ray(t_app *app, t_vec2 ray_dir, t_ray_hit *hit)
{
	t_ray_vars	vars;
	int			wall_hit;

	init_dda_vars(app, ray_dir, &vars);
	init_dda_vars_y(app, ray_dir, &vars);
	wall_hit = 0;
	while (wall_hit == 0)
	{
		if (vars.side_dist_x < vars.side_dist_y)
		{
			vars.side_dist_x += vars.delta_dist_x;
			vars.map_x += vars.step_x;
			vars.side = 0;
		}
		else
		{
			vars.side_dist_y += vars.delta_dist_y;
			vars.map_y += vars.step_y;
			vars.side = 1;
		}
		wall_hit = check_wall_hit(app, &vars);
	}
	fill_hit_info(app, ray_dir, &vars, hit);
	return (1);
}
