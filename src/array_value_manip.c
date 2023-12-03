/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rf_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 14:53:31 by eniini            #+#    #+#             */
/*   Updated: 2021/08/11 15:44:17 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
*	For the management of large values inside the resource file.
*/

#include "racer.h"

/*
*	Read a value of a size [s] from the position [p] on the array.
*/
off_t	get_charray_value(unsigned char *p, unsigned int s)
{
	unsigned int	i;
	off_t			retval;

	retval = p[0];
	i = 1;
	while (i < s)
	{
		retval |= (p[i] << (i * 8));
		i++;
	}
	return (retval);
}

/*
*	Write [value] into the given position [p] on the array.
*/
void	set_charray_value(unsigned char *p, off_t value, unsigned int size)
{
	unsigned int	i;

	i = 0;
	while (i < size)
	{
		p[i] = (unsigned char)(value >> (i * 8));
		i++;
	}
}
