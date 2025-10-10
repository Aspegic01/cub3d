/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 09:58:35 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/10/10 10:22:23 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	parse_map_file(char *map_file, t_map *map)
{
	int		fd;
	char	*line;
	int		result;

	fd = open(map_file, O_RDONLY);
	if (fd < 0)
		return (ft_putstr_fd("Error\nCannot open file\n", 2), -1);
	line = get_next_line(fd);
	if (!line)
	{
		close(fd);
		return (ft_putstr_fd("Error\nEmpty file\n", 2), -1);
	}
	while (line)
	{
		result = process_line(line, map, fd);
		if (result == -1)
		{
			free(line);
			// close(fd);
			// free(map);
			return (-1);
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}

static int	setup_map_grid(char *map_file, t_map *map)
{
	if (!allocate_map_grid(map))
		return (ft_putstr_fd("Error\nMemory allocation failed\n", 2), -1);
	if (!load_map_data(map_file, map))
	{
		free_map_grid(map);
		return (ft_putstr_fd("Error\nFailed to load map data\n", 2), -1);
	}
	
	// Fix spaces inside the map
	fix_zero_space_to_zero(map);
	
	return (0);
}

static int	validate_map(t_map *map, t_player *player)
{
	if (!check_map_surrounded_by_walls(map))
		return (-1);
	if (init_player(map, player) == -1)
		return (-1);
	return (0);
}

t_map	*read_map(char *map_file)
{
	t_map		*map;
	t_player	player;
	int			result;

	map = init_map();
	if (!map)
		return (NULL);
	if (parse_map_file(map_file, map) == -1)
	{
		// free_textures(map);
		return (NULL);
	}
	result = validate_final_map(map);
	if (result == -1)
		return (NULL);
	if (setup_map_grid(map_file, map) == -1)
	{
		free_textures(map);
		free(map);
		return (NULL);
	}
	if (validate_map(map, &player) == -1)
	{
		free_map_grid(map);
		free_textures(map);
		free(map);
		return (NULL);
	}
	// printf("\nMap loaded successfully!\n");
	// printf("Found all 6 elements: NO, SO, WE, EA, F, C\n");
	// printf("Map dimensions: %d x %d\n", map->width, map->height);
	
	// // Print loaded textures and colors
	// printf("\nLoaded textures:\n");
	// printf("North: %s\n", map->textures.north ? map->textures.north : "NULL");
	// printf("South: %s\n", map->textures.south ? map->textures.south : "NULL");
	// printf("West: %s\n", map->textures.west ? map->textures.west : "NULL");
	// printf("East: %s\n", map->textures.east ? map->textures.east : "NULL");
	
	// printf("\nLoaded colors:\n");
	// printf("Floor: RGB(%d, %d, %d)\n", map->colors.floor_r, map->colors.floor_g, map->colors.floor_b);
	// printf("Ceiling: RGB(%d, %d, %d)\n", map->colors.ceiling_r, map->colors.ceiling_g, map->colors.ceiling_b);
	
	// // Print the loaded map for verification
	// printf("\nLoaded map:\n");
	// for (int i = 0; i < map->height; i++)
	// {
	// 	printf("%s\n", map->grid[i]);
	// }
	
	// Don't free the map here - return it to be used by the window
	return (map);
}

int main(int ac, char *av[])
{
    t_game  game;
    t_map   *map;
    
    if (ac != 2)
        return (ft_putstr_fd("Error\nInvalid number of arguments\n", 2), 1);
    if (strcmp(av[1] + ft_strlen(av[1]) - 4, ".cub") != 0)
        return (ft_putstr_fd("Error\nInvalid file extension\n", 2), 1);
    
    map = read_map(av[1]);
    if (!map)
    {
        // free_textures(map);
        free_map_grid(map);
        free(map);
	// printf("sadasda");
        return (1);
    }
    printf("Map dimensions: %dx%d\n", map->width, map->height);
    
    if (init_window(&game) == -1)
    {
        free_textures(map);
        free_map_grid(map);
        free(map);
        return (1);
    }
    
    mlx_loop(game.mlx);
    free_textures(map);
    free_map_grid(map);
    free(map);
    
    return (0);
}
