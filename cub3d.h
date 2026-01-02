/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:00:38 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/12/26 17:08:10 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include "./include/minilibx/include/MLX42/MLX42.h"
# include "./libft/libft.h"
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define WIN_WIDTH 1280
# define WIN_HEIGHT 720
# define CELL_SIZE 12
# define CELL_COUNT 10
# define CELL_COUNT_HALF 5
# define PLAYER_SIZE 3
# define PLAYER_HALF 1
# define PLAYER_BUFFER 0.2f
# define WIN_TITLE "Cub3D"
# define ROT_SPEED 3.0f
# define MOVE_SPEED 1.5f
# define MOUSE_SENSITIVITY 0.001f
# define MAPFG 0xFFFFFFFF
# define MAPBG 0x333333FF
# define GUN_FRAMES 15

typedef struct s_v2f
{
	float			x;
	float			y;
}					t_v2f;

typedef struct s_v2i
{
	int32_t			x;
	int32_t			y;
}					t_v2i;

typedef struct s_player
{
	t_v2f			position;
	t_v2f			dir;
	t_v2f			plane;
}					t_player;

typedef struct s_textures
{
	char			*north;
	char			*south;
	char			*west;
	char			*east;
}					t_textures;

typedef struct s_colors
{
	int				floor_r;
	int				floor_g;
	int				floor_b;
	int				ceiling_r;
	int				ceiling_g;
	int				ceiling_b;
}					t_colors;

typedef struct s_map
{
	char			**grid;
	int				width;
	int				height;
	int				line_count;
	int				player_count;
	int				map_line_index;
	int				elements_count;
	mlx_texture_t	*tex_north;
	mlx_texture_t	*tex_south;
	mlx_texture_t	*tex_west;
	mlx_texture_t	*tex_east;
	t_textures		textures;
	t_colors		colors;
	mlx_image_t		*img;
	t_v2i			position;
	t_v2i			size;
	t_player		player;
}					t_map;

typedef struct s_ray
{
	t_v2f			dir;
	t_v2f			length;
	t_v2f			step_size;
	t_v2f			step_dir;
	t_v2f			hit_pos;
	uint32_t		side;
	float			distance;
}					t_ray;

typedef struct s_wall_stripe
{
	int32_t			height;
	int32_t			start;
	int32_t			end;
	t_ray			ray;
}					t_wall_stripe;

typedef struct s_wall_texture
{
	mlx_texture_t	*tex;
	float			wallx;
	float			tex_step;
	float			tex_pos;
	int32_t			texx;
}					t_wall_texture;

typedef struct s_gun
{
	mlx_image_t		*frames[GUN_FRAMES];
	int				curr_frame;
	bool			is_active;
	double			timer;
}					t_gun;

typedef struct s_game
{
	mlx_t			*mlx;
	mlx_image_t		*canvas;
	t_map			*map;
	mlx_texture_t	*tex;
	double			last_mouse;
	t_gun			gun;
}					t_game;

int					init_game(char *map_file);
t_map				*read_map(char *map_file);

// Map parsing functions
t_map				*init_map(void);
int					is_valid_element(char *line);
int					validate_map_line(char *line);
int					process_line(char *line, t_map *map, int fd);
int					validate_final_map(t_map *map);
// int     allocate_map_grid(t_map *map);
int					store_map_line(char *line, t_map *map);
int					load_map_data(char *map_file, t_map *map);
int					is_valid_map_char(char c);
int					check_first_last_row(t_map *map, int row);
int					check_side_walls(t_map *map);
int					is_valid_neighbor(t_map *map, int row, int col);
int					validate_walkable_spaces(t_map *map);
int					check_map_surrounded_by_walls(t_map *map);

// Texture and color loading functions
char				*extract_path(char *line);
int					parse_color(char *line, int *r, int *g, int *b);
int					load_texture(char *line, t_map *map);

// Player initialization functions
void				set_player_direction(t_player *player, char direction);
int					find_player_position(t_map *map, t_player *player);
int					init_player(t_map *map, t_player *player);

// Memory management functions
void				free_textures(t_map *map);
void				free_map_grid(t_map *map);
void				free_game(void);
void				free_map(void);
void				free_player(void);

// Map utility functions
void				fix_zero_space_to_zero(t_map *map);

// Mini map
int32_t				clamp(int32_t value, int32_t min, int32_t max);
int					minimap_setup(t_game *game);
void				minimap_render(t_game *game);
void				render_player(t_map *scene, uint32_t color);
int					init_gun(t_game *game);
void				update_gun(t_game *game);

// Engine
void				render_game(t_game *game);
void				draw_line(mlx_image_t *grid, t_v2f start, t_v2f end,
						uint32_t color);
void				draw_fov(t_game *game, t_map *scene);
bool				ft_at_wall(t_map *map, float offsetx, float offsety);
mlx_texture_t		*ft_get_texture(t_map *scene, uint32_t side, t_v2f step);
uint32_t			ft_darken_color(uint32_t color);
uint32_t			ft_get_pixel_color(uint8_t r, uint8_t g, uint8_t b,
						uint8_t a);
int32_t				ft_get_lineheight(float distance);
t_ray				ft_init_ray(t_map *scene, float angle);
t_wall_stripe		ft_compute_wall_stripe(t_map *scene, float camerax);

// Vector functions
t_v2i				veci_new(int32_t x, int32_t y);
t_v2i				veci_zero(void);
t_v2i				veci_from(t_v2i that);
t_v2i				veci_fromf(t_v2f that);
void				veci_print(char *label, t_v2i vec);
t_v2i				veci_scale(t_v2i vec, float factor);
t_v2i				veci_add(t_v2i vec, t_v2i that);
t_v2i				veci_sub(t_v2i vec, t_v2i that);
t_v2i				veci_div(t_v2i vec, t_v2i that);
t_v2i				veci_mul(t_v2i vec, t_v2i that);
t_v2f				vecf_rot(t_v2f vec, float angle);

t_v2f				vecf_new(float x, float y);
t_v2f				vecf_zero(void);
t_v2f				vecf_from(t_v2f that);
void				vecf_print(char *label, t_v2f vec);
t_v2f				vecf_scale(t_v2f vec, float factor);
t_v2f				vecf_scale(t_v2f vec, float factor);
t_v2f				vecf_add(t_v2f vec, t_v2f that);
t_v2f				vecf_sub(t_v2f vec, t_v2f that);
t_v2f				vecf_div(t_v2f vec, t_v2f that);
t_v2f				vecf_mul(t_v2f vec, t_v2f that);

// Window and game functions
int					init_window(t_game *game);
int					close_window(t_game *game);
int					handle_keypress(int keycode, t_game *game);
void				render_frame(t_game *game);
int					game_loop(t_game *game);
void				move_player_in_dir(t_game *game, t_v2f dir);
void				rotate_player(t_game *game, float rot_speed);
void				rotate_player_mouse(t_game *game, float rot_speed);

#endif
