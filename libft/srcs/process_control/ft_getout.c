/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getout.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/14 17:37:00 by eniini            #+#    #+#             */
/*   Updated: 2021/03/26 14:16:02 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	Prints [str] to stderr, then exits the program
**	with the (EXIT_FAILURE) status.
*/

void	ft_getout(const char *str)
{
	ft_putendl_fd(str, 2);
	exit(EXIT_FAILURE);
}
