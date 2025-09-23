#ifndef CUB3D_H
#define CUB3D_H

/* ========================================================================== */
/*                               INCLUDES                                    */
/* ========================================================================== */

#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "libft.h"
#include <math.h>

/* ========================================================================== */
/*                             CONSTANTS                                     */
/* ========================================================================== */

// Window settings
#define DEFAULT_WIN_WIDTH	1024
#define DEFAULT_WIN_HEIGHT	768

// Minimap settings
#define MINIMAP_SCALE		15
#define MINIMAP_OFFSET_X	50
#define MINIMAP_OFFSET_Y	50

// Ray visualization settings
#define MINIMAP_RAY_COUNT	20
#define MINIMAP_RAY_LENGTH	50

// Colors
#define COLOR_WALL			0xFFFFFF
#define COLOR_FLOOR			0x404040
#define COLOR_PLAYER		0xFF0000
#define COLOR_BORDER		0x808080
#define COLOR_RAY			0x00FF00

// Key codes (adjust according to your system)
#define KEY_W				119
#define KEY_A				97
#define KEY_S				115
#define KEY_D				100
#define KEY_ESC				65307
#define KEY_LEFT			65361
#define KEY_RIGHT			65363

/* ========================================================================== */
/*                            STRUCTURES                                     */
/* ========================================================================== */

// Basic data types
typedef struct	s_vec2
{
	double	x;
	double	y;
}	t_vec2;

typedef struct	s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

// Graphics structures
typedef struct	s_img
{
	void	*ptr;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		w;
	int		h;
}	t_img;

typedef struct	s_textures
{
	t_img	north;
	t_img	south;
	t_img	west;
	t_img	east;

	char	*north_path;
	char	*south_path;
	char	*west_path;
	char	*east_path;

	bool	loaded;
}	t_textures;

// Game world structures
typedef struct	s_map
{
	char	**grid;
	int		width;
	int		height;
}	t_map;

typedef struct	s_player
{
	t_vec2	pos;
	t_vec2	dir;
	t_vec2	plane;
}	t_player;

// Input handling
typedef struct	s_keys
{
	bool	w;
	bool	a;
	bool	s;
	bool	d;
	bool	left;
	bool	right;
}	t_keys;

// Parser helper
typedef struct	s_parse_counters
{
	int		texture_count;
	int		color_count;
	int		map_started;
	int		map_line_index;
}	t_parse_counters;

// Main application structure
typedef struct	s_app
{
	// MLX data
	void		*mlx;
	void		*win;
	int			win_w;
	int			win_h;

	// Graphics
	t_img		frame;
	t_textures	tex;
	t_textures	textures;

	// Game world
	t_map		map;
	t_player	player;
	t_color		floor;
	t_color		ceil;

	// Input
	t_keys		keys;

	// Game settings
	double		move_speed;
	double		rot_speed;
} t_app;

/* ========================================================================== */
/*                          FUNCTION PROTOTYPES                              */
/* ========================================================================== */

// ============================================================================
//                              CORE FUNCTIONS
// ============================================================================

// Application lifecycle
int		app_init(t_app *app, int w, int h);
void	app_destroy(t_app *app, int code);
int		app_loop(t_app *app);
void	init_defaults(t_app *app);

// Error handling
void	error_exit(t_app *app, const char *msg);

// ============================================================================
//                              PARSING
// ============================================================================

// Main parsing functions
int		parsing(t_app *app, int argc, char **argv);
int		parse_input(t_app *app, int argc, char **argv);
int		parse_cub_file(t_app *app, const char *path);
void	parse_single_line(t_app *app, char *line, t_parse_counters *counters);

// Texture parsing
int		parse_texture_line(t_app *app, char *line);

// Color parsing
int		parse_color_line(t_app *app, char *line);
int		parse_rgb_values(const char *rgb_str, t_color *color);
char	*extract_rgb_string(char *line);

// Map parsing
int		init_map(t_app *app);
int		add_map_line(t_app *app, char *line, int line_index);

// Map validation
int		validate_map(t_app *app);
int		find_player(t_app *app);
int		check_valid_chars(t_app *app);
int		check_map_closed(t_app *app);
int		is_wall_or_void(t_app *app, int x, int y);

// Parsing utilities
int		is_map_line(char *line);
int		is_color_line(char *line);
int		is_texture_line(char *line);
int		is_empty_line(char *line);

// ============================================================================
//                              RENDERING
// ============================================================================

// Frame rendering
void	render_frame(t_app *app);
void	fill_background(t_app *app, int color);

// Image manipulation
void	img_put_pixel(t_img *img, int x, int y, int color);
void	draw_rect(t_app *app, int x, int y, int w, int h, int color);

// Minimap
void	render_minimap(t_app *app);
void	draw_minimap_tile(t_app *app, int map_x, int map_y, int color);
void	draw_player_on_minimap(t_app *app);

// Ray visualization
void	render_minimap_rays(t_app *app);
int		cast_minimap_ray(t_app *app, t_vec2 ray_dir, t_vec2 *hit_point);
void	get_ray_direction(t_app *app, int ray_index, int total_rays, t_vec2 *ray_dir);
void	draw_line(t_app *app, int x0, int y0, int x1, int y1, int color);

// Texture management
int		load_textures(t_app *app);
void	free_textures(t_app *app);

int		color_to_int(t_color color);
void	draw_ceiling(t_app *app);
void	draw_floor(t_app *app);
void	render_background(t_app *app);
// ============================================================================
//                              INPUT HANDLING
// ============================================================================

int		key_press(int keycode, t_app *app);
int		key_release(int keycode, t_app *app);
int		close_window(t_app *app);
bool	is_valid_position(t_app *app, double x, double y);
void	move_player_forward_backward(t_app *app, int direction);
void	move_player_strafe(t_app *app, int direction);
void	update_player_movement(t_app *app);
void	rotate_player(t_app *app, int direction);
void	move_player(t_app *app, double move_x, double move_y);

// ============================================================================
//                              UTILITIES
// ============================================================================

// String utilities
void	free_split(char **arr);
void	*gnl_free(void *p);

// Color utilities
int		rgb_to_int(t_color c);

// Get Next Line
char	*get_next_line(int fd);
char	*freebuff(char **principal_buff);
char	*ft_new_buff(char *principal_buff);
char	*ft_create_line(char *principal_buff);
char	*ft_explore_text(char *principal_buff, int fd);

#endif // CUB3D_H
