/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clamp_d.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 18:05:05 by eniini            #+#    #+#             */
/*   Updated: 2021/04/20 14:10:21 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	ft_clamp_d(double value, double min, double max)
{
	if (value < min)
		value = min;
	if (value > max)
		value = max;
	return (value);
}
