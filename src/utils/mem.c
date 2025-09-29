#include "cub3d.h"
#include <stdlib.h>

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

void	*gnl_free(void *p)
{
	if (!p)
		get_next_line(-1);
	else
		free(p);
	return (NULL);
}
