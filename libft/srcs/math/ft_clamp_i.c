/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clamp_i.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 18:00:28 by eniini            #+#    #+#             */
/*   Updated: 2021/04/20 14:07:11 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_clamp_i(int value, int min, int max)
{
	if (value < min)
		value = min;
	if (value > max)
		value = max;
	return (value);
}
