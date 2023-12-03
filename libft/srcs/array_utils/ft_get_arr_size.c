/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_arr_size.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/01 15:38:02 by eniini            #+#    #+#             */
/*   Updated: 2021/04/20 12:35:06 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	Get the n of array pointers allocated into given double char pointer.
*/

size_t	ft_get_arr_size(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i] != NULL)
		i++;
	return (i);
}
