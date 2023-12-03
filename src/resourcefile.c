/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resourcefile.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 22:01:59 by eniini            #+#    #+#             */
/*   Updated: 2021/08/13 10:30:11 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "racer.h"
#include <unistd.h>

/*
*	Updates specified header information with the given data.
*	valid datapositions are either RF_LUMPCOUNT_POSITION or RF_LUMPLIST_OFFSET
*	Returns FALSE if write() call fails or an invalid position is given.
*/
static t_bool	rf_update_header(int fd, unsigned int datapos, off_t newdata)
{
	unsigned char	val[8];

	if (datapos == RF_LUMPCOUNT_POSITION || datapos == RF_LUMPLIST_OFFSET)
	{
		lseek(fd, datapos, SEEK_SET);
		set_charray_value(val, newdata, sizeof(newdata));
		if (write(fd, val, 8) == -1)
			return (FALSE);
		return (TRUE);
	}
	return (FALSE);
}

/*
*	Handles the whole process of adding a .TGA file into
*	the active resource file.
*/
void	add_tga_to_rf(t_rf *rf, const char *asset)
{
	if (!rf->fd)
		ft_getout("no active file descriptor! (while creating the lump)");
	if (!(rf_load_tga_into_rf(rf, asset)))
		ft_getout("failed to load asset!");
	if (!rf_update_header(rf->fd, RF_LUMPCOUNT_POSITION, rf->lumpcount))
		ft_getout("failed to update resourcefile header");
	if (!rf_update_header(rf->fd, RF_LUMPLIST_OFFSET, rf->lumpdata_offset))
		ft_getout("failed to update resourcefile header");
}

/*
*	Handles the whole process of loading a .TGA file from the active resource
*	resource file. Returns an [imgdata] pointer to translated image data
*	or NULL on failure.
*/
t_imgdata	*load_tga_from_rf(t_rf *rf, short lump_id)
{
	int				fd;
	unsigned char	h[RF_HEADERSIZE];
	off_t			l_count;
	off_t			l_totalsize;

	fd = rf_open_resourcefile('r', "DATA");
	if (read(fd, h, RF_HEADERSIZE) != RF_HEADERSIZE)
		ft_getout("Failed to find TGA lump position in RF!");
	l_count = get_charray_value(&h[RF_LUMPCOUNT_POSITION], (sizeof(off_t)));
	l_totalsize = get_charray_value(&h[RF_LUMPLIST_OFFSET], (sizeof(off_t)));
	return (rf_load_tga_lump(rf, lump_id));
}
