/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:19 by hugoganet         #+#    #+#             */
/*   Updated: 2025/09/29 13:32:20 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdlib.h>

/**
 * @brief Libère un tableau de chaînes alloué dynamiquement.
 *
 * Parcourt le tableau de pointeurs et libère chaque chaîne individuelle,
 * puis libère le tableau lui-même. Gère correctement le cas NULL (aucune
 * opération). Utilisé principalement pour les résultats de ft_split().
 *
 * @param arr Tableau de chaînes terminé par NULL à libérer.
 *
 */
void	free_split(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

/**
 * @brief Helper pour nettoyer les buffers de get_next_line.
 *
 * Utilitaire qui combine la libération d'un pointeur et le nettoyage des
 * buffers internes de get_next_line. Si p est NULL, appelle get_next_line(-1)
 * pour forcer le vidage des buffers. Sinon, libère simplement p.
 *
 * @param p Pointeur à libérer, ou NULL pour forcer le nettoyage GNL.
 * @return void* Retourne toujours NULL (commodité pour assignation).
 *
 */
void	*gnl_free(void *p)
{
	if (!p)
		get_next_line(-1);
	else
		free(p);
	return (NULL);
}
