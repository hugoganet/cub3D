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

	// Fixed: initialize texture pointer to NULL for safe cleanup
	texture->ptr = NULL;
	texture->addr = NULL;
	texture->w = 0;
	texture->h = 0;

	texture->ptr = mlx_xpm_file_to_image(app->mlx, path, &width, &height);
	if (!texture->ptr)
	{
		// Fixed: proper Error message format for texture loading failures
		ft_putstr_fd("Error\n", 2);
		ft_putstr_fd("Failed to load texture: ", 2);
		ft_putendl_fd(path, 2);
		return (1);
	}

	texture->addr = mlx_get_data_addr(texture->ptr, &texture->bpp,
								  &texture->line_len, &texture->endian);
	if (!texture->addr)
	{
		// Fixed: proper Error message format and cleanup texture ptr on addr failure
		ft_putstr_fd("Error\n", 2);
		ft_putstr_fd("Failed to get texture data: ", 2);
		ft_putendl_fd(path, 2);
		mlx_destroy_image(app->mlx, texture->ptr);
		texture->ptr = NULL; // Ensures cleanup
		return (1);
	}

	texture->w = width;
	texture->h = height;
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
	// Fixed: load textures with proper error handling and cleanup
	// Charger texture Nord
	if (load_single_texture(app, app->tex.north_path, &app->tex.north))
		return (1);

	// Charger texture Sud
	if (load_single_texture(app, app->tex.south_path, &app->tex.south))
	{
		// Cleanup: destroy previously loaded north texture
		if (app->tex.north.ptr)
		{
			mlx_destroy_image(app->mlx, app->tex.north.ptr);
			app->tex.north.ptr = NULL; // Fixed: prevent double-free
		}
		return (1);
	}

	// Charger texture Ouest
	if (load_single_texture(app, app->tex.west_path, &app->tex.west))
	{
		// Cleanup: destroy previously loaded textures
		if (app->tex.north.ptr)
		{
			mlx_destroy_image(app->mlx, app->tex.north.ptr);
			app->tex.north.ptr = NULL; // Fixed: prevent double-free
		}
		if (app->tex.south.ptr)
		{
			mlx_destroy_image(app->mlx, app->tex.south.ptr);
			app->tex.south.ptr = NULL; // Fixed: prevent double-free
		}
		return (1);
	}

	// Charger texture Est
	if (load_single_texture(app, app->tex.east_path, &app->tex.east))
	{
		// Cleanup: destroy previously loaded textures
		if (app->tex.north.ptr)
		{
			mlx_destroy_image(app->mlx, app->tex.north.ptr);
			app->tex.north.ptr = NULL; // Fixed: prevent double-free
		}
		if (app->tex.south.ptr)
		{
			mlx_destroy_image(app->mlx, app->tex.south.ptr);
			app->tex.south.ptr = NULL; // Fixed: prevent double-free
		}
		if (app->tex.west.ptr)
		{
			mlx_destroy_image(app->mlx, app->tex.west.ptr);
			app->tex.west.ptr = NULL; // Fixed: prevent double-free
		}
		return (1);
	}

	app->tex.loaded = true;  // ← Marquer comme chargé
	return (0);
}

/**
 * @brief Libère la mémoire des textures
 *
 * @param app Structure principale
 */
void free_textures(t_app *app)
{
	// Fixed: safe texture cleanup with double-free protection
	if (app->tex.north.ptr)
	{
		mlx_destroy_image(app->mlx, app->tex.north.ptr);
		app->tex.north.ptr = NULL;
	}
	if (app->tex.south.ptr)
	{
		mlx_destroy_image(app->mlx, app->tex.south.ptr);
		app->tex.south.ptr = NULL;
	}
	if (app->tex.west.ptr)
	{
		mlx_destroy_image(app->mlx, app->tex.west.ptr);
		app->tex.west.ptr = NULL;
	}
	if (app->tex.east.ptr)
	{
		mlx_destroy_image(app->mlx, app->tex.east.ptr);
		app->tex.east.ptr = NULL;
	}
    // AJOUTER : Libérer les chemins de textures
    if (app->tex.north_path)
    {
        free(app->tex.north_path);
        app->tex.north_path = NULL;
    }
    if (app->tex.south_path)
    {
        free(app->tex.south_path);
        app->tex.south_path = NULL;
    }
    if (app->tex.east_path)
    {
        free(app->tex.east_path);
        app->tex.east_path = NULL;
    }
    if (app->tex.west_path)
    {
        free(app->tex.west_path);
        app->tex.west_path = NULL;
    }
	app->tex.loaded = false;  // ← Marquer comme non chargé

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

