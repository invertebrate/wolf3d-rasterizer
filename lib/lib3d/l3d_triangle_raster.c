/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_triangle_raster.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 17:02:18 by veilo             #+#    #+#             */
/*   Updated: 2020/10/27 15:32:10 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d_internals.h"

static void		order_corners_y(t_triangle *triangle, t_vertex **vtc,
								t_vec2 *ordered_corners, t_vec2 *points_2d)
{
	size_t	indices[3];
	t_vec2	uvs[3];

	ml_vector2_copy(triangle->uvs[0], uvs[0]);
	ml_vector2_copy(triangle->uvs[1], uvs[1]);
	ml_vector2_copy(triangle->uvs[2], uvs[2]);
	ft_min_double_idx((double[3]){points_2d[0][1],
									points_2d[1][1],
									points_2d[2][1]},
						3, &(indices[0]));
	vtc[0] = triangle->vtc[indices[0]];
	ml_vector2_copy(points_2d[indices[0]], ordered_corners[0]);
	ft_max_double_idx((double[3]){points_2d[0][1],
									points_2d[1][1],
									points_2d[2][1]},
						3, &(indices[2]));
	vtc[2] = triangle->vtc[indices[2]];
	ml_vector2_copy(points_2d[indices[2]], ordered_corners[2]);
	indices[1] = 3 - (indices[0] + indices[2]);
	vtc[1] = triangle->vtc[indices[1]];
	ml_vector2_copy(points_2d[indices[1]], ordered_corners[1]);
}

void			clamp_bary(float *barycoords)
{
	if (barycoords[0] > 1.0)
		barycoords[0] = 1.0;
	else if (barycoords[0] < 0.0)
		barycoords[0] = 0.0;
	if (barycoords[1] > 1.0)
		barycoords[1] = 1.0;
	else if (barycoords[1] < 0.0)
		barycoords[1] = 0.0;
	if (barycoords[2] > 1.0)
		barycoords[2] = 1.0;
	else if (barycoords[2] < 0.0)
		barycoords[2] = 0.0;
}

void			clamp_uv(t_vec2 uv)
{
	if (uv[0] > 1.0)
		uv[0] = 1.0;
	else if (uv[0] < 0.0)
		uv[0] = 0.0;
	if (uv[1] > 1.0)
		uv[1] = 1.0;
	else if (uv[1] < 0.0)
		uv[1] = 0.0;
}

static void		scan_line(uint32_t *buffer, uint32_t *dimensionswh,
							float *limits, t_triangle *triangle)
{
	int			x;
	int			y;
	int			end_x;
	float		baryc[3];
	t_vec2		uv;

	y = floor(limits[2]);
	x = floor(limits[0]);
	end_x = floor(limits[1]);
	while (x < end_x)
	{
		if (x < -(int)dimensionswh[0] / 2 - 1)
			x = -(int)dimensionswh[0] / 2 - 1;
		else if (x > (int)dimensionswh[0] / 2 + 1)
			break ;
		l3d_calculate_barycoords(triangle->points_2d, (t_vec2){x, y}, baryc);
		// clamp_bary(baryc);//!this causes a seam artifact when clipping triangles
		l3d_interpolate_uv(triangle, baryc, uv);
		clamp_uv(uv);
		l3d_pixel_plot(buffer,
					(uint32_t[2]){dimensionswh[0], dimensionswh[1]},
					(int[2]){x + dimensionswh[0] / 2, y + dimensionswh[1] / 2},
					l3d_sample_texture(triangle->material->texture,
										triangle->material->width,
										triangle->material->height,
										uv));
		x++;
	}
}

static void		scan_z_line(uint32_t *zbuffer, uint32_t *dimensionswh,
							float *limits, t_triangle *triangle)
{
	int			x;
	int			y;
	int			end_x;
	float		baryc[3];
	int			z_val;

	y = floor(limits[2]);
	x = floor(limits[0]);
	end_x = floor(limits[1]);
	while (x < end_x)
	{
		if (x < -(int)dimensionswh[0] / 2 - 1)
			x = -(int)dimensionswh[0] / 2 - 1;
		else if (x > (int)dimensionswh[0] / 2 + 1)
			break ;
		l3d_calculate_barycoords(triangle->points_2d, (t_vec2){x, y}, baryc);
		z_val = (int)floor(baryc[0] * triangle->vtc[0]->pos[2] +
			baryc[1] * triangle->vtc[1]->pos[2] +
			baryc[2] * triangle->vtc[2]->pos[2]);
		l3d_pixel_plot(zbuffer, (uint32_t[2]){dimensionswh[0], dimensionswh[1]},
			(int[2]){x + dimensionswh[0] / 2, y + dimensionswh[1] / 2},
			z_val);
		x++;
	}
}

static void		raster_upper(uint32_t *buf, uint32_t *dims,
							t_triangle *triangle, t_raster_data *data)
{
	float	x;
	float	y;
	float	end_x;

	y = data->y1;
	while (y < data->y2)
	{
		if (y < -(int)dims[1] / 2 - 1)
			y = -(int)dims[1] / 2 - 1;
		else if (y > (int)dims[1] / 2 + 1)
			break;
		x = data->x2 + data->slope_ab * (y - data->y2);
		end_x = data->x1 + data->slope_ac * (y - data->y1);
		if (!data->is_zbuffer)
		{
			if (x < end_x)
				scan_line(buf, dims, (float[3]){x, end_x + 1, y}, triangle);
			else
				scan_line(buf, dims, (float[3]){end_x, x + 1, y}, triangle);
		}
		else
		{
			if (x < end_x)
				scan_z_line(buf, dims, (float[3]){x, end_x + 1, y}, triangle);
			else
				scan_z_line(buf, dims, (float[3]){end_x, x + 1, y}, triangle);
		}
		

		y++;
	}
}

static void		raster_lower(uint32_t *buf, uint32_t *dims,
							t_triangle *triangle, t_raster_data *data)
{
	float	x;
	float	y;
	float	end_x;

	y = data->y2;
	while (y < data->y3)
	{
		if (y < -(int)dims[1] / 2 - 1)
			y = -(int)dims[1] / 2 - 1;
		else if (y > (int)dims[1] / 2 + 1)
			break;
		x = data->x2 + data->slope_bc * (y - data->y2);
		end_x = data->x1 + data->slope_ac * (y - data->y1);
		if (!data->is_zbuffer)
		{
			if (x < end_x)
				scan_line(buf, dims, (float[3]){x, end_x + 1, y}, triangle);
			else
				scan_line(buf, dims, (float[3]){end_x, x + 1, y}, triangle);
		}
		else
		{
			if (x < end_x)
				scan_z_line(buf, dims, (float[3]){x, end_x + 1, y}, triangle);
			else
				scan_z_line(buf, dims, (float[3]){end_x, x + 1, y}, triangle);
		}
		y++;
	}
}

void			l3d_triangle_set_zbuffer(uint32_t *buffer, uint32_t *dimensions,
									t_triangle *triangle)
{
	t_raster_data	data;
	t_vec2			ordered_points_2d[3];

	order_corners_y(triangle, triangle->ordered_vtc, ordered_points_2d,
					triangle->points_2d);
	data.x1 = floor(ordered_points_2d[0][0]);
	data.x2 = floor(ordered_points_2d[1][0]);
	data.x3 = floor(ordered_points_2d[2][0]);
	data.y1 = floor(ordered_points_2d[0][1]);
	data.y2 = floor(ordered_points_2d[1][1]);
	data.y3 = floor(ordered_points_2d[2][1]);
	data.slope_bc = (data.x3 - data.x2) / (data.y3 - data.y2);
	data.slope_ac = (data.x3 - data.x1) / (data.y3 - data.y1);
	data.slope_ab = (data.x2 - data.x1) / (data.y2 - data.y1);
	data.is_zbuffer = true;
	raster_upper(buffer, dimensions, triangle, &data);
	raster_lower(buffer, dimensions, triangle, &data);
}

void			l3d_triangle_raster(uint32_t *buffer, uint32_t *dimensions,
									t_triangle *triangle)
{
	t_raster_data	data;
	t_vec2			ordered_points_2d[3];

	order_corners_y(triangle, triangle->ordered_vtc, ordered_points_2d,
					triangle->points_2d);
	data.x1 = floor(ordered_points_2d[0][0]);
	data.x2 = floor(ordered_points_2d[1][0]);
	data.x3 = floor(ordered_points_2d[2][0]);
	data.y1 = floor(ordered_points_2d[0][1]);
	data.y2 = floor(ordered_points_2d[1][1]);
	data.y3 = floor(ordered_points_2d[2][1]);
	data.slope_bc = (data.x3 - data.x2) / (data.y3 - data.y2);
	data.slope_ac = (data.x3 - data.x1) / (data.y3 - data.y1);
	data.slope_ab = (data.x2 - data.x1) / (data.y2 - data.y1);
	data.is_zbuffer = false;
	raster_upper(buffer, dimensions, triangle, &data);
	raster_lower(buffer, dimensions, triangle, &data);
}

/*
**	Calculates the barycentric coordinates for a 2d point
*/

void			l3d_calculate_barycoords(t_vec2 *triangle_points_2d,
										t_vec2 point,
										float *baryc)
{
	float denom;
	float inv_denom;

	denom = ((triangle_points_2d[1][1] - triangle_points_2d[2][1]) *
			(triangle_points_2d[0][0] - triangle_points_2d[2][0]) +
			(triangle_points_2d[2][0] - triangle_points_2d[1][0]) *
			(triangle_points_2d[0][1] - triangle_points_2d[2][1]));

	if (fabs(denom) < L3D_EPSILON)
		denom = (denom / fabs(denom)) * L3D_EPSILON;
	inv_denom = 1 / ((triangle_points_2d[1][1] - triangle_points_2d[2][1]) *
				(triangle_points_2d[0][0] - triangle_points_2d[2][0]) +
				(triangle_points_2d[2][0] - triangle_points_2d[1][0]) *
				(triangle_points_2d[0][1] - triangle_points_2d[2][1]));

	baryc[0] = ((triangle_points_2d[1][1] - triangle_points_2d[2][1]) *
				(point[0] - triangle_points_2d[2][0]) +
				(triangle_points_2d[2][0] - triangle_points_2d[1][0]) *
				(point[1] - triangle_points_2d[2][1])) * inv_denom;
	baryc[1] = ((triangle_points_2d[2][1] - triangle_points_2d[0][1]) *
				(point[0] - triangle_points_2d[2][0]) +
				(triangle_points_2d[0][0] - triangle_points_2d[2][0]) *
				(point[1] - triangle_points_2d[2][1])) * inv_denom;
	baryc[2] = 1 - baryc[0] - baryc[1];
}

/*
**	Interpolates the uv coordinates for a 2d point based on barycentric
**	coordinates
*/

void			l3d_interpolate_uv(t_triangle *triangle, float *baryc,
									t_vec2 uv)
{
	float	az;
	float	bz;
	float	cz;

	az = 1 / triangle->vtc[0]->pos[2];
	bz = 1 / triangle->vtc[1]->pos[2];
	cz = 1 / triangle->vtc[2]->pos[2];
	uv[0] = ((baryc[0] * triangle->uvs[0][0]) * az +
			(baryc[1] * triangle->uvs[1][0]) * bz +
			(baryc[2] * triangle->uvs[2][0]) * cz) /
			((baryc[0] * 1) * az +
			(baryc[1] * 1) * bz +
			(baryc[2] * 1) * cz);
	uv[1] = 1 - ((baryc[0] * triangle->uvs[0][1]) * az +
			(baryc[1] * triangle->uvs[1][1]) * bz +
			(baryc[2] * triangle->uvs[2][1]) * cz) /
			((baryc[0] * 1) * az +
			(baryc[1] * 1) * bz +
			(baryc[2] * 1) * cz);
}

	/*
**	Samples the texture with given uv_coordinates
**	x = (floor(U * (width - 1))); //TODO Check later if the -1 offset is needed
**	y = (floor(V * (height - 1)));
**	index = x + width * y;
*/

uint32_t		l3d_sample_texture(uint32_t * texture_data, int width,
									int height, t_vec2 uv_point)
{
	int	index;

	index = (int)floor((floor(uv_point[0] * (width))) +
							width * (floor(uv_point[1] * (height))));
	if (index >= width * height)
		index = width * height - 1;
	return (texture_data[index]);
}
