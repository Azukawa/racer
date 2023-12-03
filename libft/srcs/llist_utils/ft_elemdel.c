/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_elemdel.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 01:08:54 by eniini            #+#    #+#             */
/*   Updated: 2021/02/19 15:56:26 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	Frees all pointed content in a link.
*/

void	ft_elemdel(void *content, size_t size)
{
	if (content != NULL)
	{
		(void)size;
		free(content);
	}
}
