#include "../cub3d.h"

void	render_cell(t_map *scene, t_v2i pos, uint32_t color)
{
	t_v2i	start;
	t_v2i	end;

	start = veci_new(pos.x, pos.y);
	end = veci_new(pos.x + CELL_SIZE, pos.y + CELL_SIZE);
	while (start.y < end.y && end.y < scene->size.y)
	{
		start.x = pos.x;
		while (start.x < end.x && end.x < scene->size.x)
		{
			if (end.y - start.y == CELL_SIZE || end.x - start.x == CELL_SIZE)
				mlx_put_pixel(scene->img, start.x, start.y, color << 1);
			else
				mlx_put_pixel(scene->img, start.x, start.y, color);
			start.x++;
		}
		start.y++;
	}
}

void	minimap_render(t_game *game)
{
	t_v2i	grid_pos;
	t_v2i	grid_start;
	t_v2i	cell;
	int		line_len;

	grid_pos = veci_fromf(game->map->player.position);
	grid_start = veci_new(grid_pos.x - CELL_COUNT_HALF, grid_pos.y - CELL_COUNT_HALF);
	grid_start.x = clamp(grid_start.x, 0, game->map->width - 1);
	grid_start.y = clamp(grid_start.y, 0, game->map->height - 1);
	cell.y = 0;
	grid_pos.y = grid_start.y;
	while (cell.y < CELL_COUNT && grid_pos.y < game->map->height)
	{
		cell.x = 0;
		grid_pos.x = grid_start.x;
		line_len = ft_strlen(game->map->grid[grid_pos.y]);
		while (cell.x < CELL_COUNT && grid_pos.x < game->map->width)
		{
			if (grid_pos.x < line_len && game->map->grid[grid_pos.y][grid_pos.x] == '0')
				render_cell(game->map, veci_new(cell.x * CELL_SIZE, cell.y * CELL_SIZE), MAPFG);
			else
				render_cell(game->map, veci_new(cell.x * CELL_SIZE, cell.y * CELL_SIZE), MAPBG);
			grid_pos.x++;
			cell.x++;
		}
		grid_pos.y++;
		cell.y++;
	}
	render_player(game->map, 0x0000FFFF);
}
