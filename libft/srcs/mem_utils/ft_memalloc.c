/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/15 20:26:56 by eniini            #+#    #+#             */
/*   Updated: 2021/07/28 17:31:51 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	Allocates [size] bytes of new memory initialized to 0's and returns it.
*/

void	*ft_memalloc(size_t size)
{
	void	*mem;

	mem = (void *)malloc(sizeof(mem) * size);
	if (!mem)
		return (NULL);
	ft_bzero(mem, sizeof(mem) * size);
	return (mem);
}
