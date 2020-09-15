/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_round.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/18 23:26:36 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/11 17:48:46 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double		ft_ceil(double num)
{
	long long int		int_part;
	double				fraction;
	t_float_dissector	fd;

	if (num == 0)
		return (num);
	fd.f = num;
	int_part = (long long int)num;
	fraction = num - int_part;
	return (fd.b.sign ? int_part : int_part + 1);
}

double		ft_abs_ceil(double num)
{
	t_float_dissector fd;

	if (num == 0)
		return (num);
	fd.f = num;
	return((fd.b.sign ? -1 : 1) * ft_ceil((ft_abs(num))));
}

double		ft_floor(double num)
{
	t_float_dissector	fd;

	fd.f = num;
	return (!fd.b.sign ? (long long int)num : (long long int)num - 1);
}

int			ft_round(double num)
{ 
	return (num < 0 ? num - 0.5 : num + 0.5); 
}
