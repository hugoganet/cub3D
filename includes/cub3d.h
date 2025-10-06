/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncrivell <ncrivell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 12:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/06 16:53:04 by ncrivell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdbool.h>
# include <stddef.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include "libft.h"
# include "get_next_line.h"

# define DEFAULT_WIN_WIDTH 1024
# define DEFAULT_WIN_HEIGHT 768

# define MINIMAP_SCALE 10
# define MINIMAP_OFFSET_X 25
# define MINIMAP_OFFSET_Y 25

# define MINIMAP_RAY_COUNT 20
# define MINIMAP_RAY_LENGTH 50

# define COLOR_WALL 0xFFFFFF
# define COLOR_FLOOR 0x404040
# define COLOR_PLAYER 0xFF0000
# define COLOR_BORDER 0x808080
# define COLOR_RAY 0x00FF00

# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_ESC 65307
# define KEY_LEFT 65361
# define KEY_RIGHT 65363

# define MOVE_SPEED 0.03
# define ROT_SPEED 0.03

/* ========================================================================== */
/*                            STRUCTURES                                     */
/* ========================================================================== */

// Basic data types
/**
 * @brief Vecteur 2D avec composantes en virgule flottante.
 *
 * Structure représentant un point ou un vecteur dans l'espace 2D.
 * Utilisée pour les positions, directions et vitesses.
 */
typedef struct s_vec2
{
	double	x;
	double	y;
}	t_vec2;

/**
 * @brief Structure représentant une couleur RGB.
 *
 * Stocke les composantes rouge, verte et bleue d'une couleur.
 * Valeurs comprises entre 0 et 255.
 */
typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

// Graphics structures
/**
 * @brief Structure d'image MiniLibX.
 *
 * Contient toutes les informations nécessaires pour manipuler une image
 * créée avec la MiniLibX, incluant les données de pixels et les
 * métadonnées de format.
 */
typedef struct s_img
{
	void	*ptr;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		w;
	int		h;
}	t_img;

/**
 * @brief Structure contenant toutes les textures murales.
 *
 * Stocke les images de texture pour chaque direction (Nord, Sud, Est,
 * Ouest) ainsi que leurs chemins d'accès et l'état de chargement.
 */
typedef struct s_textures
{
	t_img	north;
	t_img	south;
	t_img	west;
	t_img	east;
	char	*north_path;
	char	*south_path;
	char	*west_path;
	char	*east_path;
}	t_textures;

// Game world structures
/**
 * @brief Structure représentant la carte du jeu.
 *
 * Contient la grille de caractères représentant la carte avec ses
 * dimensions. La grille utilise '0' pour les espaces vides, '1' pour
 * les murs.
 */
typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
}	t_map;

/**
 * @brief Structure du joueur dans le monde 3D.
 *
 * Définit la position, direction de vue et plan de caméra du joueur
 * pour le système de raycasting. Le plan de caméra détermine le
 * champ de vision.
 */
typedef struct s_player
{
	t_vec2	pos;
	t_vec2	dir;
	t_vec2	plane;
}	t_player;

typedef struct s_keys
{
	bool	w;
	bool	a;
	bool	s;
	bool	d;
	bool	left;
	bool	right;
}	t_keys;

// Raycasting structures
/**
 * @brief Variables temporaires pour l'algorithme DDA.
 *
 * Structure contenant toutes les variables nécessaires au calcul
 * DDA (Digital Differential Analyzer) pour le raycasting.
 */
typedef struct s_ray_vars
{
	int		map_x;
	int		map_y;
	double	delta_dist_x;
	double	delta_dist_y;
	int		step_x;
	int		step_y;
	double	side_dist_x;
	double	side_dist_y;
	int		side;
}	t_ray_vars;

/**
 * @brief Résultat d'une collision de rayon avec un mur.
 *
 * Contient toutes les informations nécessaires pour le rendu d'une
 * colonne de mur : distance, face touchée, coordonnées de texture.
 */
typedef struct s_ray_hit
{
	double	perp_dist;
	int		side;
	int		wall_face;
	double	wall_x;
	int		map_x;
	int		map_y;
}	t_ray_hit;

/**
 * @brief Énumération des faces de murs pour différents ombrages.
 *
 * Définit les constantes pour identifier quelle face d'un mur a été
 * touchée par un rayon, permettant d'appliquer différentes textures
 * ou ombrages selon l'orientation.
 */
enum e_wall_face
{
	FACE_NORTH = 0,
	FACE_SOUTH = 1,
	FACE_EAST = 2,
	FACE_WEST = 3
};

// Parser helper
/**
 * @brief Compteurs pour l'analyse du fichier .cub.
 *
 * Structure d'aide pour suivre l'état du parsing : nombre d'éléments
 * trouvés et position dans le fichier.
 */
typedef struct s_parse_counters
{
	int	texture_count;
	int	color_count;
	int	map_started;
	int	map_line_index;
}	t_parse_counters;

/**
 * @brief Structure principale de l'application cub3D.
 *
 * Contient tous les éléments nécessaires au fonctionnement du moteur :
 * données MLX, graphiques, monde de jeu, entrées utilisateur et
 * paramètres de jeu.
 */
typedef struct s_app
{
	void		*mlx;
	void		*win;
	int			win_w;
	int			win_h;
	t_img		frame;
	t_textures	tex;
	t_map		map;
	t_player	player;
	t_color		floor;
	t_color		ceil;
	t_keys		keys;
	double		move_speed;
	double		rot_speed;
}	t_app;

/* ========================================================================== */
/*                          FUNCTION PROTOTYPES                              */
/* ========================================================================== */

// ============================================================================
//                              CORE FUNCTIONS
// ============================================================================

// Initialization & Cleanup
int		app_init(t_app *app, int w, int h);
void	app_destroy(t_app *app);
void	init_defaults(t_app *app);

// Main Loop
int		app_loop(t_app *app);
int		close_window(t_app *app);

// ============================================================================
//                              PARSING
// ============================================================================

// Main Parsing Entry Points

int		parsing(t_app *app, int argc, char **argv);
int		parse_input(int argc, char **argv);
int		parse_cub_file(t_app *app, const char *path);
int		parse_single_line(t_app *app, char *line, t_parse_counters *counters);

// Line Handlers

int		handle_texture_line(t_app *app, char *line,
			t_parse_counters *counters);
int		handle_color_line(t_app *app, char *line, t_parse_counters *counters);
int		handle_map_line(t_app *app, char *line, t_parse_counters *counters);

// Line Type Checkers

int		is_empty_line(char *line);
int		is_map_line(char *line);
int		is_color_line(char *line);
int		is_texture_line(char *line);

// Texture Parsing

int		parse_texture_line(t_app *app, char *line);
char	*extract_path(char *line);

// Color Parsing

int		parse_color_line(t_app *app, char *line);
int		parse_rgb_values(const char *rgb_str, t_color *color);
char	*extract_rgb_string(char *line);

// Map Parsing

int		add_map_line(t_app *app, char *line, int line_index);
char	**resize_map_grid(char **old_grid, int needed);
char	*dup_map_line(char *line, int len);
int		get_trimmed_len(char *line);

// Map Validation

int		validate_parsing_completion(t_parse_counters *counters);
int		validate_map(t_app *app);
int		check_valid_chars(t_app *app);
int		check_map_closed(t_app *app);

// Player Detection & Setup

int		find_player(t_app *app);
void	process_cell_for_player(t_app *app, int i, int j, int *player_count);
void	orientation_north_or_south(t_app *app, char c);
void	orientation_east_or_west(t_app *app, char c);

// Map Utilities

int		is_wall_or_void(t_app *app, int x, int y);
int		is_at_map_edge(t_app *app, int x, int y);
int		has_open_neighbor(t_app *app, int x, int y);
int		is_valid_neighbor(t_app *app, int x, int y);

// ============================================================================
//                              INPUT HANDLING
// ============================================================================

// Keyboard Events

int		key_press(int keycode, t_app *app);
int		key_release(int keycode, t_app *app);

// Player Movement

void	update_player_movement(t_app *app);
void	move_player(t_app *app, double move_x, double move_y);
void	rotate_player(t_app *app, int direction);
bool	is_valid_position(t_app *app, double x, double y);

// ============================================================================
//                              RAYCASTING
// ============================================================================

// Ray Casting Core

int		cast_ray(t_app *app, t_vec2 ray_dir, t_ray_hit *hit);
void	calculate_ray_dir(t_app *app, int x, t_vec2 *ray_dir);
int		get_wall_side(int step_x, int step_y, int side);

// DDA Utilities

void	init_delta_dist(t_vec2 ray_dir, t_ray_vars *vars);
void	init_step_x(t_app *app, t_vec2 ray_dir, t_ray_vars *vars);
void	init_step_y(t_app *app, t_vec2 ray_dir, t_ray_vars *vars);
void	init_dda_vars(t_app *app, t_vec2 ray_dir, t_ray_vars *vars);
int		check_wall_hit(t_app *app, t_ray_vars *vars);
void	fill_hit_info(t_app *app, t_vec2 ray_dir, t_ray_vars *vars,
			t_ray_hit *hit);

// 3D Rendering

void	render_3d_view(t_app *app);
void	calculate_wall_bounds(int height, int screen_h, int *draw_start,
			int *draw_end);
double	calculate_wall_height(double perp_dist, int screen_h);

// Wall Drawing

void	draw_wall_column(t_app *app, int *params, int color);
void	draw_textured_wall_column(t_app *app, int *params, t_ray_hit *hit);
int		get_wall_color(int wall_face);

// ============================================================================
//                              RENDERING
// ============================================================================

// Frame Rendering

void	render_frame(t_app *app);

// Background Rendering

void	render_background(t_app *app);
void	draw_ceiling(t_app *app);
void	draw_floor(t_app *app);

// Texture Management

int		load_textures(t_app *app);
int		load_single_texture(t_app *app, char *path, t_img *texture);
t_img	*get_wall_texture(t_app *app, int side, t_vec2 ray_dir);
int		get_texture_pixel(t_img *texture, int x, int y);
int		get_texture_coord_x(double wall_x, t_img *texture, int wall_face);

// Drawing Primitives

void	img_put_pixel(t_img *img, int x, int y, int color);
void	draw_rect(t_app *app, int *params, int color);
void	draw_line(t_app *app, int *params, int color);

// ============================================================================
//                              UTILITIES
// ============================================================================

// Error Handling

int		error_msg(const char *msg);

// Memory Management

void	free_map(t_app *app);
void	free_textures(t_app *app);
void	free_split(char **arr);
void	*gnl_free(void *p);

// Color Utilities

int		color_to_int(t_color color);
int		rgb_to_int(t_color c);

#endif
