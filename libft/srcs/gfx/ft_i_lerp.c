/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_i_lerp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 22:04:33 by eniini            #+#    #+#             */
/*   Updated: 2021/05/14 18:06:49 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_gfx.h"

/*
**	Lineal interpolation calculation that returns an integer.
**	Cast to int is warranted as [p * (b - a)] returns a double due to coercion.
*/

int	ft_i_lerp(int a, int b, double p)
{
	return (a + (int)(p * (b - a)));
}
