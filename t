void	draw_fov(t_game *game, t_map *scene, t_player *player, uint32_t color)
{
	(void)color;
	uint32_t x = 0;
	while (x < game->canvas->width)
	{
		float_t camerax = 2.0 * (x / (float_t)game->canvas->width) - 1.0;
		uint32_t hit = 0;
		uint32_t side = 0;
		float_t distance = 0;
		t_v2i map = veci_new(player->position.x, player->position.y);
		t_v2f step = vecf_zero();
		t_v2f ray_dir = vecf_add(player->dir, vecf_scale(player->plane, camerax));
		t_v2f rayLenght1D = vecf_zero();
		t_v2f rayStepSize = vecf_zero();

		if (ray_dir.x == 0.0)
			rayStepSize.x = 1e30;
		else
			rayStepSize.x = fabs(1 / ray_dir.x);
		if (ray_dir.y == 0)
			rayStepSize.y = 1e30;
		else
			rayStepSize.y = fabs(1 / ray_dir.y);

		if (ray_dir.x < 0)
		{
			step.x = -1;
			rayLenght1D.x = (player->position.x - map.x) * rayStepSize.x;
		} else {
			step.x = 1;
			rayLenght1D.x = ((map.x + 1.0) - player->position.x) * rayStepSize.x;
		}
		if (ray_dir.y < 0)
		{
			step.y = -1;
			rayLenght1D.y = (player->position.y - map.y) * rayStepSize.y;
		} else {
			step.y = 1;
			rayLenght1D.y = ((map.y + 1.0) - player->position.y) * rayStepSize.y;
		}
		while (hit == 0)
		{
			if (rayLenght1D.x < rayLenght1D.y)
			{
				map.x += step.x;
				distance = rayLenght1D.x;
				rayLenght1D.x += rayStepSize.x;
				side = 0;
			} else {
				map.y += step.y;
				distance = rayLenght1D.y;
				rayLenght1D.y += rayStepSize.y;
				side = 1;
			}
			if (at_wall(scene, map.x, map.y))
				hit = 1;
		}
		
		// t_v2f end = vecf_add(player->position, vecf_scale(ray_dir, distance));
		// draw_line(game->map->img, vecf_scale(player->position, CELL_SIZE), vecf_scale(end, CELL_SIZE), color);
		int32_t line_height;

		if (distance == 0)
			line_height = (int32_t)game->canvas->height / 1;
		else
			line_height = (int32_t)game->canvas->height / distance;
		int32_t draw_start = -line_height / 2 + game->canvas->height / 2;
		if (draw_start < 0)
			draw_start = 0;
		int32_t draw_end = line_height / 2 + game->canvas->height / 2;
		if (draw_end >= (int32_t)game->canvas->height)
			draw_end = game->canvas->height - 1;
		int32_t y = draw_start;
		/* choose texture based on side and step direction */
		mlx_texture_t *tex = NULL;
		if (side == 0)
		{
			if (step.x < 0)
				tex = scene->tex_west;
			else
				tex = scene->tex_east;
		}
		else
		{
			if (step.y < 0)
				tex = scene->tex_north;
			else
				tex = scene->tex_south;
		}

		if (tex)
		{
			/* compute exact hit position on the wall */

			t_v2f hit_pos = vecf_add(player->position, vecf_scale(ray_dir, distance));
			float wallX;
			if (side == 0) {
				wallX = hit_pos.y;
			} else {
				wallX = hit_pos.x;
			}
			wallX = wallX - floor(wallX);
			int texX = (int)(wallX * (float_t)tex->width);
			if (texX < 0)
				texX = 0;
			if (texX >= (int)tex->width)
				texX = tex->width - 1;

			float texStep = (float)tex->height / (float)line_height;
			float texPos = (draw_start - (float)game->canvas->height / 2 + (float)line_height / 2) * texStep;
			while (y < draw_end)
			{
				int texY = (int)texPos;
				if (texY < 0)
					texY = 0;
				if (texY >= (int)tex->height)
					texY = tex->height - 1;
				int idx = (texY * tex->width + texX) * tex->bytes_per_pixel;
				uint8_t *p = &tex->pixels[idx];
				uint32_t pixcol = ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | (uint32_t)p[3];
				// /* darken for y-side to give a sense of depth */
				if (side == 1)
				{
					uint8_t r = (pixcol >> 24) & 0xFF;
					uint8_t g = (pixcol >> 16) & 0xFF;
					uint8_t b = (pixcol >> 8) & 0xFF;
					r /= 2; g /= 2; b /= 2;
					pixcol = ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | (pixcol & 0xFF);
				}
				mlx_put_pixel(game->canvas, x, y, pixcol);
				texPos += texStep;
				y++;
			}
		}
		else
		{
			int wall_color = color;
			if (side == 1)
				wall_color = 0x0000ff00;
			while (y < draw_end)
				mlx_put_pixel(game->canvas, x, ++y, wall_color);
		}
		x++;
	}
}

