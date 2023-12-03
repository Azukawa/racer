/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rf_lump.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 17:02:39 by eniini            #+#    #+#             */
/*   Updated: 2021/08/13 11:47:51 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "racer.h"
#include <unistd.h>

void	rf_free_lumplist(t_rf_lump *head)
{
	t_rf_lump	*temp;

	while (head != NULL)
	{
		temp = head;
		head = head->next;
		temp->next = NULL;
		free(temp);
	}
	head = NULL;
}

/*
*	Creates a new lump struct into linked list stored in [rf] until all
*	content modification is finished. After that, updated list of lumps is
*	written to the end of the resource file.
*	Returns FALSE on error.
*/
t_bool	rf_create_lumpinfo(t_rf *rf, off_t	bytesize, char type)
{
	t_rf_lump	*newlump;

	newlump = (t_rf_lump *)malloc(sizeof(t_rf_lump));
	if (!newlump)
		return (FALSE);
	rf->lumpcount++;
	newlump->bytesize = bytesize;
	newlump->id = rf->lumpcount;
	newlump->offset = rf->lumpdata_offset;
	newlump->type = type;
	newlump->next = NULL;
	if (rf->lumplist)
	{
		newlump->next = rf->lumplist;
		rf->lumplist = newlump;
	}
	else
		rf->lumplist = newlump;
	ft_printf("Loaded asset [%hd] to offset [%lu]\n", rf->lumplist->id, \
	rf->lumpdata_offset);
	rf->lumpdata_offset += bytesize;
	return (TRUE);
}

/*
*	Iterates through all of the lumps within [rf->lumplist] and writes the
*	data in segments after the last actual lump within the given resource file.
*/
void	rf_write_lumplist(t_rf *rf)
{
	unsigned char	lumpinfo[RF_LUMPINFOSIZE];
	t_rf_lump		*next;
	t_rf_lump		*head;
	off_t			lumpcount;

	lumpcount = 0;
	ft_bzero(lumpinfo, RF_LUMPINFOSIZE);
	if (!(lseek(rf->fd, rf->lumpdata_offset, SEEK_SET)))
		ft_getout("failed to lseek into lumplist position");
	if (!rf->lumplist)
		ft_getout("failed to find a linked list to write");
	head = rf->lumplist;
	while (rf->lumplist)
	{
		next = rf->lumplist->next;
		set_charray_value(lumpinfo, rf->lumplist->offset, sizeof(off_t));
		set_charray_value(&lumpinfo[8], rf->lumplist->bytesize, sizeof(off_t));
		set_charray_value(&lumpinfo[16], rf->lumplist->id, sizeof(short));
		set_charray_value(&lumpinfo[18], rf->lumplist->type, sizeof(char));
		if (write(rf->fd, lumpinfo, RF_LUMPINFOSIZE) != RF_LUMPINFOSIZE)
			ft_getout("failed to write lumpinfo into fd");
		rf->lumplist = next;
		lumpcount++;
	}
	if (lumpcount != rf->lumpcount)
		ft_getout("new and existing lumpcounts do not match!");
	rf->lumplist = head;
}
