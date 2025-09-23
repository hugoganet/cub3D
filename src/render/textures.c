#include "cub3d.h"
#include <mlx.h>

/**
 * @file textures.c
 * @brief Système de chargement et application des textures murales
 *
 * Ce fichier gère le chargement des textures XPM et leur application
 * sur les murs lors du rendu 3D. Chaque orientation (N/S/E/W) a sa texture.
 */


/**
 * @brief Charge une texture XPM individual
 *
 * @param app Structure principale
 * @param path Chemin vers le fichier XPM
 * @param texture Structure t_img à remplir
 * @return 0 si succès, 1 si erreur
 */
int load_single_texture(t_app *app, char *path, t_img *texture)
{
	int width, height;

	printf("🖼️ Loading texture: %s\n", path);

	texture->ptr = mlx_xpm_file_to_image(app->mlx, path, &width, &height);
	if (!texture->ptr)
	{
		printf("❌ Failed to load texture: %s\n", path);
		return (1);
	}

	texture->addr = mlx_get_data_addr(texture->ptr, &texture->bpp,
								  &texture->line_len, &texture->endian);
	if (!texture->addr)
	{
		printf("❌ Failed to get texture data: %s\n", path);
		mlx_destroy_image(app->mlx, texture->ptr);
		return (1);
	}

	texture->w = width;
	texture->h = height;

	printf("✅ Texture loaded: %dx%d pixels\n", width, height);
	return (0);
}





/**
 * @brief Charge toutes les textures de murs
 *
 * @param app Structure principale contenant les chemins de textures
 * @return 0 si succès, 1 si erreur
 */
int load_textures(t_app *app)
{
	printf("🎨 Loading wall textures...\n");

	// Charger texture Nord
	if (load_single_texture(app, app->tex.north_path, &app->tex.north))  // ← tex.north_path
		return (1);

	// Charger texture Sud
	if (load_single_texture(app, app->tex.south_path, &app->tex.south))  // ← tex.south_path
		return (1);

	// Charger texture Ouest
	if (load_single_texture(app, app->tex.west_path, &app->tex.west))    // ← tex.west_path
		return (1);

	// Charger texture Est
	if (load_single_texture(app, app->tex.east_path, &app->tex.east))    // ← tex.east_path
		return (1);

	app->tex.loaded = true;  // ← Marquer comme chargé
	printf("🎉 All textures loaded successfully!\n");
	return (0);
}

/**
 * @brief Libère la mémoire des textures
 *
 * @param app Structure principale
 */
void free_textures(t_app *app)
{
	if (app->tex.north.ptr)      // ← tex au lieu de textures
		mlx_destroy_image(app->mlx, app->tex.north.ptr);
	if (app->tex.south.ptr)
		mlx_destroy_image(app->mlx, app->tex.south.ptr);
	if (app->tex.west.ptr)
		mlx_destroy_image(app->mlx, app->tex.west.ptr);
	if (app->tex.east.ptr)
		mlx_destroy_image(app->mlx, app->tex.east.ptr);

	app->tex.loaded = false;  // ← Marquer comme non chargé
	printf("🗑️ Textures freed\n");
}

/**
 * @brief Récupère un pixel d'une texture à des coordonnées données
 *
 * @param texture Texture source
 * @param x Coordonnée X (0 à texture->width-1)
 * @param y Coordonnée Y (0 à texture->height-1)
 * @return Couleur du pixel au format int RGB
 */
int get_texture_pixel(t_img *texture, int x, int y)
{
	char *pixel;
	int color;

	// Vérifier les limites
	if (x < 0 || x >= texture->w || y < 0 || y >= texture->h)  // ← w et h
		return (0x000000);

	// Calculer l'adresse du pixel
	pixel = texture->addr + (y * texture->line_len + x * (texture->bpp / 8));  // ← addr

	// Extraire la couleur
	color = *(int*)pixel;

	return (color & 0xFFFFFF);
}

/**
 * @brief Sélectionne la texture appropriée selon l'orientation du mur
 *
 * @param app Structure principale
 * @param side Orientation du mur (0=N/S, 1=E/W)
 * @param ray_dir Direction du rayon pour déterminer N/S ou E/W
 * @return Pointeur vers la texture appropriée
 */
t_img *get_wall_texture(t_app *app, int side, t_vec2 ray_dir)
{
    if (side == 0) // Mur vertical (N/S)
    {
        if (ray_dir.x > 0)
            return (&app->tex.east);   // ← tex.east
        else
            return (&app->tex.west);   // ← tex.west
    }
    else // Mur horizontal (E/W)
    {
        if (ray_dir.y > 0)
            return (&app->tex.south);  // ← tex.south
        else
            return (&app->tex.north);  // ← tex.north
    }
}

