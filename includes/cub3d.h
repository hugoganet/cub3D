#ifndef CUB3D_H
#define CUB3D_H

#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

// Forward declarations for MiniLibX types to avoid including mlx.h in headers
typedef struct s_mlx s_mlx;
typedef struct s_img s_img;

typedef struct s_vec2
{
	double x;
	double y;
} t_vec2;

typedef struct s_color
{
	int r;
	int g;
	int b;
} t_color;

typedef struct s_img
{
	void *ptr;
	char *addr;
	int bpp;
	int line_len;
	int endian;
	int w;
	int h;
} t_img;

typedef struct s_textures
{
	t_img north;
	t_img south;
	t_img west;
	t_img east;
	// chemins des fichiers
	char *north_path;
	char *south_path;
	char *west_path;
	char *east_path;
	bool loaded;
} t_textures;

typedef struct s_map
{
	char **grid;
	int width;
	int height;
} t_map;

typedef struct s_player
{
	t_vec2 pos;
	t_vec2 dir;
	t_vec2 plane;
} t_player;

typedef struct s_keys
{
	bool w;
	bool a;
	bool s;
	bool d;
	bool left;
	bool right;
} t_keys;

typedef struct s_app
{
	void *mlx;
	void *win;
	int win_w;
	int win_h;

	t_img frame;
	t_textures tex;

	t_map map;
	t_player player;
	t_color floor;
	t_color ceil;

	t_keys keys;

	double move_speed;
	double rot_speed;
} t_app;

// Entry points
int app_init(t_app *app, int w, int h);
void app_destroy(t_app *app, int code);
int app_loop(t_app *app);

// Input
int key_press(int keycode, t_app *app);
int key_release(int keycode, t_app *app);
int close_window(t_app *app);

// Rendering
void render_frame(t_app *app);
void img_put_pixel(t_img *img, int x, int y, int color);

// Textures
int load_textures(t_app *app);
void free_textures(t_app *app);

// Parser
void init_defaults(t_app *app);
int parse_texture_line(t_app *app, char *line);
int parse_cub_file(t_app *app, const char *path);
int parsing(t_app *app, int argc, char **argv);
int parse_input(t_app *app, int argc, char **argv);
int count_lines(const char *filename);
int is_map_line(char *line);
int is_color_line(char *line);
int is_texture_line(char *line);
int parse_rgb_values(const char *rgb_str, t_color *color);
char *extract_rgb_string(char *line);
int parse_color_line(t_app *app, char *line);
int count_map_lines(const char *path);
int init_map(t_app *app, const char *path);
int add_map_line(t_app *app, char *line, int line_index);


// GNL
char *get_next_line(int fd);
char	*freebuff(char **principal_buff);
char	*ft_new_buff(char *principal_buff);
char	*ft_create_line(char *principal_buff);
char	*ft_explore_text(char *principal_buff, int fd);
// Utils
void error_exit(t_app *app, const char *msg);
void free_split(char **arr);
void *gnl_free(void *p);
int rgb_to_int(t_color c);

#endif // CUB3D_H
