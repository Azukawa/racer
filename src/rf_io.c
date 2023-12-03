/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rf_io.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 22:00:15 by eniini            #+#    #+#             */
/*   Updated: 2021/08/12 22:01:45 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
*	I/O management functions for the resource file handling.
*/

#include "racer.h"
#include <unistd.h>
#include <fcntl.h>

/*
*	Returns the updated file descriptor unless write() call fails.
*/
static int	rf_init_header(int fd)
{
	static unsigned char	rfheader[RF_HEADERSIZE] = {
		'R', 'F',
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0};

	if (write(fd, rfheader, RF_HEADERSIZE) != RF_HEADERSIZE)
		return (FALSE);
	return (fd);
}

/*
*	Opens the designated resource file in a desired mode.
*	Valid modes are:
*	[r] - read only
*	[w] - write only
*	[m] - read & write.
*	Returns file descriptor on success, -1 on failure.
*/
int	rf_open_resourcefile(char openmode, char *filename)
{
	int	fd;

	fd = -1;
	if (openmode == 'r')
		fd = open(filename, O_RDONLY);
	else if (openmode == 'w')
	{
		fd = open(filename, O_WRONLY);
		if (fd == -1)
		{
			fd = open(filename, O_WRONLY | O_CREAT | O_EXCL,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			if (fd != -1)
				return (rf_init_header(fd));
		}
	}
	else if (openmode == 'm')
		fd = open(filename, O_RDWR);
	if (fd >= 0)
		return (fd);
	return (-1);
}

/*
*	TODO: this probably needs to free all of the linked lists etc on memory
*/
void	rf_close_fd(t_rf *rf)
{
	if (close(rf->fd) == -1)
		ft_getout(strerror(errno));
}
