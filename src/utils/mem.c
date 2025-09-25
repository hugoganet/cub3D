#include "cub3d.h"
#include <stdlib.h>

void free_split(char **arr)
{
	int i = 0;
	if (!arr)
		return;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void *gnl_free(void *p)
{
	// Clean up get_next_line static buffer when p is NULL
	if (!p)
		get_next_line(-1);  // Trigger static buffer cleanup
	else
		free(p);
	return (NULL);
}
