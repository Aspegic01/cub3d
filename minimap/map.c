#include "../cub3d.h"

static void	renderCell(t_map *scene, t_v2i pos, uint32_t color)
{
	uint32_t drawColor;
	uint32_t x;
	uint32_t y;

	y = 0;
	while (y < CELL_SIZE)
	{
		x = 0;
		while (x < CELL_SIZE)
		{
			drawColor = color;
			if (x == 0 || y == 0 || x == CELL_SIZE - 1 || y == CELL_SIZE - 1)
				drawColor = color << 1;
			mlx_put_pixel(scene->img, pos.x + x, pos.y + y, drawColor);
			x++;
		}
		y++;
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
				renderCell(game->map, veci_new(cell.x * CELL_SIZE, cell.y * CELL_SIZE), MAPFG);
			else
				renderCell(game->map, veci_new(cell.x * CELL_SIZE, cell.y * CELL_SIZE), MAPBG);
			grid_pos.x++;
			cell.x++;
		}
		grid_pos.y++;
		cell.y++;
	}
	render_player(game->map, 0x0000FFFF);
}
