/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 09:58:35 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/10/17 11:23:23 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "include/minilibx/include/MLX42/MLX42.h"

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
		return (close(fd), ft_putstr_fd("Error\nEmpty file\n", 2), -1);
	while (line)
	{
		result = process_line(line, map, fd);
		if (result == -1)
			return (free(line), close(fd), -1);
		free(line);
		line = get_next_line(fd);
	}
	return (close(fd), 0);
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
		return (NULL);
	result = validate_final_map(map);
	if (result == -1)
		return (NULL);
	if (setup_map_grid(map_file, map) == -1)
	{
		free_map_grid(map);
		free_textures(map);
		return (free(map), NULL);
	}
	if (validate_map(map, &player) == -1)
	{
		free_map_grid(map);
		free_textures(map);
		return (free(map), NULL);
	}
	return (map);
}

int	main(int ac, char *av[])
{
	t_game	game;
	t_map	*map;

	if (ac != 2)
		return (ft_putstr_fd("Error\nInvalid number of arguments\n", 2), 1);
	if (strcmp(av[1] + ft_strlen(av[1]) - 4, ".cub") != 0)
		return (ft_putstr_fd("Error\nInvalid file extension\n", 2), 1);
	map = read_map(av[1]);
	if (!map)
		return (1);
	game.map = map;
	if (init_window(&game) == -1)
	{
		free_textures(map);
		free_map_grid(map);
		free(map);
		return (1);
	}
	mlx_loop(game.mlx);
	mlx_terminate(game.mlx);
	free_textures(map);
	free_map_grid(map);
	free(map);
	return (0);
}
