#include <unistd.h>
#include <fcntl.h>
#include "racer.h"

/*
*	Reading all of the assumed pixel data fromt the file into the buffer.
*	Each 4 bytes is stored into a single hexadecimal (ARGB) value.
*	Returns TRUE if pixel data loading is successful.
*/
static t_bool	load_data(t_imgdata *img, int fd)
{
	ssize_t		readbytes;
	uint32_t	i;
	char		*rawdata;
	ssize_t		img_size;

	readbytes = 0;
	i = 0;
	img_size = img->w * img->h * 4;
	rawdata = (char *)malloc(img_size);
	img->data = (uint32_t *)malloc(sizeof(uint32_t) * img->w * img->h);
	if (!img->data || !rawdata)
		return (FALSE);
	if (read(fd, rawdata, img_size) != img_size)
		return (FALSE);
	while (readbytes < img_size)
	{
		img->data[i++] = (rawdata[readbytes + 3] & 255) << 24 | \
		(rawdata[readbytes + 2] & 255) << 16 | \
		(rawdata[readbytes + 1] & 255) << 8 | (rawdata[readbytes] & 255);
		readbytes += 4;
	}
	free(rawdata);
	if (close(fd) != 0)
		return (FALSE);
	return (TRUE);
}

/*
*	Naive .TGA loading. Only supports uncompressed truecolor (32-bit) variation
*	If a wrong type of file or a malloc/IO error is encountered,
*	function returns NULL. Otherwise a pointer to [t_img] struct is returned.
*/
t_imgdata 	*load_tga(const char *filepath)
{
	int				fd;
	unsigned char	header[18];
	t_imgdata		*img;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
		return (NULL);
	if (read(fd, header, 18) != 18)
		return (NULL);
	img = malloc(sizeof(t_imgdata));
	if (!img)
		return (NULL);
	img->w = header[0x0C] | header[0x0D] << 8;
	img->h = header[0x0E] | header[0x0F] << 8;
	if (header[0x10] != 32 || header[0x02] != 2)
	{
		free(img);
		return (NULL);
	}
	if (!load_data(img, fd))
		return (NULL);
	return (img);
}

/*
*	Takes in a writeable file descriptor for the resource file.
*	Returns FALSE if [fd] can't be opened, tga file can't be accessed or read
*	or if writing to the resource file fails.
*/
t_bool	rf_load_tga_into_rf(t_rf *rf, const char *filepath)
{
	int				tga_fd;
	char			*buffer;
	off_t			retval;
	unsigned char	header[18];
	long		 	imgbytesize;

	tga_fd = open(filepath, O_RDONLY);
	if (tga_fd <= 0 || rf->fd <= 0)
		return (FALSE);
	if (read(tga_fd, header, 18) != 18)
		return (FALSE);
	imgbytesize = ((header[0x0C] | header[0x0D] << 8) * \
		(header[0x0E] | header[0x0F] << 8) * (header[0x10] / 8)) + 18;
	if (imgbytesize <= 0)
		return (FALSE);
	buffer = malloc(imgbytesize);
	if (!buffer)
		return (FALSE);
	if (lseek(tga_fd, 0, SEEK_SET) == -1)
		return (FALSE);
	if (read(tga_fd, (void *)buffer, (size_t)imgbytesize) != imgbytesize)
		return (FALSE);
	if (close(tga_fd) == -1)
		return (FALSE);
	if (lseek(rf->fd, rf->lumpdata_offset, SEEK_SET) == -1)
		return (FALSE);
	retval = write(rf->fd, (void *)buffer, imgbytesize);
	free(buffer);
	if (retval != imgbytesize)
		return (FALSE);
	if (!rf_create_lumpinfo(rf, retval, RF_TYPE_TGA))
		return (FALSE);
	return (TRUE);
}

/*
*	Loads TGA from a resource file.
*	Assumes that [fd] points at the first byte of the image header data.
*/
t_imgdata	*rf_load_tga_lump(t_rf *rf, short lump_id)
{
	t_rf_lump		*next;
	t_rf_lump		*head;
	unsigned char	header[18];
	t_imgdata		*img;

	head = rf->lumplist;
	next = rf->lumplist->next;
	while (rf->lumplist->id != lump_id)
	{
		if (next == NULL)
			ft_getout("no TGA lump with correct id found");
		rf->lumplist = next;
		next = rf->lumplist->next;
	}
	if (rf->lumplist->type != RF_TYPE_TGA)
		ft_getout("found lump with correct ID but wrong data type");
	if (lseek(rf->fd, rf->lumplist->offset, SEEK_SET) != rf->lumplist->offset)
		ft_getout("failed to position fd correctly to read data");
	if (read(rf->fd, header, 18) == -1)
	{
		ft_getout(strerror(errno));
		return (NULL);
	}
	img = malloc(sizeof(t_imgdata));
	if (!img)
	{
		ft_getout(strerror(errno));
		return (NULL);
	}
	img->w = header[0x0C] | header[0x0D] << 8;
	img->h = header[0x0E] | header[0x0F] << 8;
	if (header[0x10] != 32 || header[0x02] != 2)
	{
		free(img);
		return (NULL);
	}
	if (!load_data(img, rf->fd))
		return (NULL);
	rf->lumplist = head;
	return (img);
}
