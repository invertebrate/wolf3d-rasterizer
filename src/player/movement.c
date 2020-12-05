/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 17:16:02 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/12/05 17:20:33 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		player_rotate(t_wolf3d *app)
{
	t_mat4	rotation_y;
	t_mat4	rotation_x;
	t_mat4	rotation;

	ml_matrix4_rotation_x(ml_rad(app->player.rot_y), rotation_y);
	ml_matrix4_rotation_y(ml_rad(app->player.rot_x), rotation_x);
	ml_matrix4_mul(rotation_x, rotation_y, rotation);
	ml_matrix4_mul_vec3(rotation, (t_vec3){0, 0, -1}, app->player.forward);
	ml_matrix4_mul_vec3(rotation, (t_vec3){1, 0, 0}, app->player.sideways);
	ml_matrix4_inverse(rotation, app->player.inv_rotation);
}

void			player_rotate_vertical(t_wolf3d *app, float angle)
{
	app->player.rot_y += app->player.rot_speed * angle;
	if (app->player.rot_y > 90.0)
		app->player.rot_y = 90.0;
	if (app->player.rot_y < -90.0)
		app->player.rot_y = -90.0;
	player_rotate(app);
}

void			player_rotate_horizontal(t_wolf3d *app, float angle)
{
	app->player.rot_x += app->player.rot_speed * angle;
	player_rotate(app);
}

/*
** / 2.0 because models are positioned with unitsize * 2
*/

void			pos_to_grid_pos(t_vec3 pos, t_vec2 grid_pos, float unit_size)
{
	grid_pos[0] = -(pos[2] / unit_size / 2.0) + 0.5;
	grid_pos[1] = (pos[0] / unit_size / 2.0) + 0.5;
}

void			player_move(t_wolf3d *app, t_move dir, float speed)
{
	t_vec3		add;
	t_vec3		forward;
	t_vec3		sideways;
	t_mat4		rotation_x;

	app->player.is_moving = true;
	ft_memset(add, 0, sizeof(t_vec3));
	ml_matrix4_rotation_y(ml_rad(app->player.rot_x), rotation_x);
	ml_matrix4_mul_vec3(rotation_x, (t_vec3){0, 0, -1}, forward);
	ml_matrix4_mul_vec3(rotation_x, (t_vec3){1, 0, 0}, sideways);
	if (dir == move_forward)
		ml_vector3_mul(forward, speed, add);
	else if (dir == move_backward)
		ml_vector3_mul(forward, -speed, add);
	else if (dir == move_strafe_left)
		ml_vector3_mul(sideways, -speed, add);
	else if (dir == move_strafe_right)
		ml_vector3_mul(sideways, speed, add);
	collision_limit_player(app, add);
	ml_vector3_add(app->player.pos, add, app->player.pos);
	ml_matrix4_translation(app->player.pos[0],
		app->player.pos[1], app->player.pos[2], app->player.translation);
	ml_matrix4_inverse(app->player.translation, app->player.inv_translation);
	pos_to_grid_pos(app->player.pos, app->player.grid_pos, app->unit_size);
	player_update_aabb(&app->player);
}
