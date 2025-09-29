/**
 * @file cub3d.h
 * @brief Fichier d'en-tête principal du projet cub3D.
 *
 * Ce fichier contient toutes les définitions de structures, constantes,
 * énumérations et prototypes de fonctions nécessaires au moteur de
 * raycasting cub3D. Il définit l'API publique du projet.
 */

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
#define DEFAULT_WIN_WIDTH 1024
#define DEFAULT_WIN_HEIGHT 768

// Minimap settings
#define MINIMAP_SCALE 10
#define MINIMAP_OFFSET_X 25
#define MINIMAP_OFFSET_Y 25

// Ray visualization settings
#define MINIMAP_RAY_COUNT 20
#define MINIMAP_RAY_LENGTH 50

// Colors
#define COLOR_WALL 0xFFFFFF
#define COLOR_FLOOR 0x404040
#define COLOR_PLAYER 0xFF0000
#define COLOR_BORDER 0x808080
#define COLOR_RAY 0x00FF00

// Key codes (adjust according to your system)
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_ESC 65307
#define KEY_LEFT 65361
#define KEY_RIGHT 65363

/* ========================================================================== */
/*                            STRUCTURES                                     */
/* ========================================================================== */

// Basic data types
/**
 * @brief Vecteur 2D avec composantes en virgule flottante.
 *
 * Structure représentant un point ou un vecteur dans l'espace 2D.
 * Utilisée pour les positions, directions et vitesses.
 * 
 */
typedef struct s_vec2
{
	double x; /**< Composante X du vecteur */
	double y; /**< Composante Y du vecteur */
} t_vec2;

/**
 * @brief Structure représentant une couleur RGB.
 *
 * Stocke les composantes rouge, verte et bleue d'une couleur.
 * Valeurs comprises entre 0 et 255.
 */
typedef struct s_color
{
	int r; /**< Composante rouge (0-255) */
	int g; /**< Composante verte (0-255) */
	int b; /**< Composante bleue (0-255) */
} t_color;

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
	void *ptr;	  /**< Pointeur vers l'objet image MLX */
	char *addr;	  /**< Adresse des données de pixels */
	int bpp;	  /**< Bits par pixel */
	int line_len; /**< Longueur d'une ligne en octets */
	int endian;	  /**< Ordre des octets (endianness) */
	int w;		  /**< Largeur de l'image */
	int h;		  /**< Hauteur de l'image */
} t_img;

/**
 * @brief Structure contenant toutes les textures murales.
 *
 * Stocke les images de texture pour chaque direction (Nord, Sud, Est,
 * Ouest) ainsi que leurs chemins d'accès et l'état de chargement.
 */
typedef struct s_textures
{
	t_img north; /**< Texture du mur Nord */
	t_img south; /**< Texture du mur Sud */
	t_img west;	 /**< Texture du mur Ouest */
	t_img east;	 /**< Texture du mur Est */

	char *north_path; /**< Chemin vers la texture Nord */
	char *south_path; /**< Chemin vers la texture Sud */
	char *west_path;  /**< Chemin vers la texture Ouest */
	char *east_path;  /**< Chemin vers la texture Est */

	bool loaded; /**< État du chargement des textures */
} t_textures;

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
	char **grid; /**< Grille 2D de la carte */
	int width;	 /**< Largeur de la carte */
	int height;	 /**< Hauteur de la carte */
} t_map;

/**
 * @brief Structure du joueur dans le monde 3D.
 *
 * Définit la position, direction de vue et plan de caméra du joueur
 * pour le système de raycasting. Le plan de caméra détermine le
 * champ de vision.
 */
typedef struct s_player
{
	t_vec2 pos;	  /**< Position du joueur dans la carte */
	t_vec2 dir;	  /**< Vecteur de direction de vue */
	t_vec2 plane; /**< Plan de caméra (perpendiculaire à dir) */
} t_player;

// Input handling
/**
 * @brief État des touches de contrôle du joueur.
 *
 * Structure booléenne pour suivre quelles touches sont actuellement
 * pressées. Permet la gestion de touches multiples simultanées.
 */
typedef struct s_keys
{
	bool w;		/**< Touche W (avancer) */
	bool a;		/**< Touche A (strafer gauche) */
	bool s;		/**< Touche S (reculer) */
	bool d;		/**< Touche D (strafer droite) */
	bool left;	/**< Flèche gauche (tourner gauche) */
	bool right; /**< Flèche droite (tourner droite) */
} t_keys;

// Raycasting structures
/**
 * @brief Résultat d'une collision de rayon avec un mur.
 *
 * Contient toutes les informations nécessaires pour le rendu d'une
 * colonne de mur : distance, face touchée, coordonnées de texture.
 */
typedef struct s_ray_hit
{
	double perp_dist; /**< Distance perpendiculaire au mur */
	int side;		  /**< Côté touché (0=NS, 1=EO) */
	int wall_face;	  /**< Face du mur (Nord/Sud/Est/Ouest) */
	double wall_x;	  /**< Coordonnée X de l'impact sur le mur */
	int map_x;		  /**< Coordonnée X dans la carte */
	int map_y;		  /**< Coordonnée Y dans la carte */
} t_ray_hit;

/**
 * @brief Énumération des faces de murs pour différents ombrages.
 *
 * Définit les constantes pour identifier quelle face d'un mur a été
 * touchée par un rayon, permettant d'appliquer différentes textures
 * ou ombrages selon l'orientation.
 */
enum e_wall_face
{
	FACE_NORTH = 0, /**< Face Nord du mur */
	FACE_SOUTH = 1, /**< Face Sud du mur */
	FACE_EAST = 2,	/**< Face Est du mur */
	FACE_WEST = 3	/**< Face Ouest du mur */
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
	int texture_count;	/**< Nombre de textures trouvées */
	int color_count;	/**< Nombre de couleurs trouvées */
	int map_started;	/**< Indicateur de début de carte */
	int map_line_index; /**< Index de ligne de la carte */
} t_parse_counters;

/**
 * @brief Structure principale de l'application cub3D.
 *
 * Contient tous les éléments nécessaires au fonctionnement du moteur :
 * données MLX, graphiques, monde de jeu, entrées utilisateur et
 * paramètres de jeu.
 */
typedef struct s_app
{
	// MLX data
	void *mlx; /**< Contexte MiniLibX */
	void *win; /**< Fenêtre MLX */
	int win_w; /**< Largeur de la fenêtre */
	int win_h; /**< Hauteur de la fenêtre */

	// Graphics
	t_img frame;	/**< Buffer d'image principal */
	t_textures tex; /**< Textures murales */

	// Game world
	t_map map;		 /**< Carte du jeu */
	t_player player; /**< État du joueur */
	t_color floor;	 /**< Couleur du sol */
	t_color ceil;	 /**< Couleur du plafond */

	// Input
	t_keys keys; /**< État des touches */

	// Game settings
	double move_speed; /**< Vitesse de déplacement */
	double rot_speed;  /**< Vitesse de rotation */
} t_app;

/* ========================================================================== */
/*                          FUNCTION PROTOTYPES                              */
/* ========================================================================== */

// ============================================================================
//                              CORE FUNCTIONS
// ============================================================================

// Application lifecycle
int app_init(t_app *app, int w, int h);
void app_destroy(t_app *app, int code);
int app_loop(t_app *app);
void init_defaults(t_app *app);

// Error handling
void error_exit(t_app *app, const char *msg);

// ============================================================================
//                              PARSING
// ============================================================================

// Main parsing functions
int parsing(t_app *app, int argc, char **argv);
int parse_input(int argc, char **argv);
int parse_cub_file(t_app *app, const char *path);
int parse_single_line(t_app *app, char *line, t_parse_counters *counters);

// Texture parsing
int parse_texture_line(t_app *app, char *line);

// Color parsing
int parse_color_line(t_app *app, char *line);
int parse_rgb_values(const char *rgb_str, t_color *color);
char *extract_rgb_string(char *line);

// Map parsing
int init_map(t_app *app);
int add_map_line(t_app *app, char *line, int line_index);

// Map validation
int validate_map(t_app *app);
int find_player(t_app *app);
int check_valid_chars(t_app *app);
int check_map_closed(t_app *app);
int is_wall_or_void(t_app *app, int x, int y);

// Parsing utilities
int is_map_line(char *line);
int is_color_line(char *line);
int is_texture_line(char *line);
int is_empty_line(char *line);

// ============================================================================
//                              RENDERING
// ============================================================================

// Frame rendering
void render_frame(t_app *app);
void fill_background(t_app *app, int color);

// Image manipulation
void img_put_pixel(t_img *img, int x, int y, int color);
void draw_rect(t_app *app, int x, int y, int w, int h, int color);

// Minimap
void render_minimap(t_app *app);
void draw_minimap_tile(t_app *app, int map_x, int map_y, int color);
void draw_player_on_minimap(t_app *app);

// Ray visualization
void render_minimap_rays(t_app *app);
int cast_minimap_ray(t_app *app, t_vec2 ray_dir, t_vec2 *hit_point);
void get_ray_direction(t_app *app, int ray_index, int total_rays, t_vec2 *ray_dir);
void draw_line(t_app *app, int x0, int y0, int x1, int y1, int color);

// Texture management
int load_textures(t_app *app);
void free_textures(t_app *app);
int get_texture_pixel(t_img *texture, int x, int y);
t_img *get_wall_texture(t_app *app, int side, t_vec2 ray_dir);

int color_to_int(t_color color);
void draw_ceiling(t_app *app);
void draw_floor(t_app *app);
void render_background(t_app *app);
// ============================================================================
//                              INPUT HANDLING
// ============================================================================

int key_press(int keycode, t_app *app);
int key_release(int keycode, t_app *app);
int close_window(t_app *app);
bool is_valid_position(t_app *app, double x, double y);
void move_player_forward_backward(t_app *app, int direction);
void move_player_strafe(t_app *app, int direction);
void update_player_movement(t_app *app);
void rotate_player(t_app *app, int direction);
void move_player(t_app *app, double move_x, double move_y);

// ============================================================================
//                              RAYCASTING
// ============================================================================

// DDA algorithm
int cast_ray(t_app *app, t_vec2 ray_dir, t_ray_hit *hit);
int cast_minimap_ray(t_app *app, t_vec2 ray_dir, t_vec2 *hit_point);

// 3D Raycasting
void render_3d_view(t_app *app);
void calculate_ray_dir(t_app *app, int x, t_vec2 *ray_dir);
int get_wall_side(int step_x, int step_y, int side);

// 3D Projection
double calculate_wall_height(double perp_dist, int screen_h);
void calculate_wall_bounds(int height, int screen_h, int *draw_start, int *draw_end);
void draw_wall_column(t_app *app, int x, int draw_start, int draw_end, int color);
void draw_textured_wall_column(t_app *app, int x, int draw_start, int draw_end, t_ray_hit *hit);
int get_texture_coord_x(double wall_x, t_img *texture);
int get_wall_color(int wall_face);

// ============================================================================
//                              UTILITIES
// ============================================================================

// String utilities
void free_split(char **arr);
void *gnl_free(void *p);

// Color utilities
int rgb_to_int(t_color c);

// Get Next Line
char *get_next_line(int fd);
char *freebuff(char **principal_buff);
char *ft_new_buff(char *principal_buff);
char *ft_create_line(char *principal_buff);
char *ft_explore_text(char *principal_buff, int fd);

void free_map(t_app *app);
#endif // CUB3D_H
