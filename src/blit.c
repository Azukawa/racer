#include "racer.h"

/*
*	Copy [img->data] into [buf->pixels] starting the from given x/y coordinates.
*	Returns -1 on error.
*/
int	blit_img(t_imgdata *img, t_buffer *buf, t_point start)
{
	uint32_t	x;
	uint32_t	y;

	if (!img || !buf)
		return (-1);
	if (start.x > buf->w || start.y > buf->h)
		return (-1);
	y = 0;
	while ((y + start.y) <= buf->h && y < img->h)
	{
		x = 0;
		while ((x + start.x) <= buf->w && x < img->w)
		{
			if (img->data[y * img->w + x] != 0)
			{
				buf->pixels[((y + start.y) * buf->w) + x + start.x] \
				= img->data[y * img->w + x];
			}
			x++;
		}
		y++;
	}
	return (0);
}

/*
*	Subfunction for blit_img_scaled(). Handles copying pixels from one buffer
*	to the other.
*/
static void	copy_loop(t_imgdata *o, t_imgdata *n, float step_x, float step_y)
{
	uint32_t	x;
	uint32_t	y;

	y = 0;
	while (y < n->h)
	{
		x = 0;
		while (x < n->w)
		{
			n->data[y * n->w + x] = \
			o->data[((int)floor(y * step_y) * o->w) + (int)floor(x * step_x)];
			x++;
		}
		y++;
	}
}

/*
*	Copies [img->data] into [buf] starting from the given x/y vector using
*	nearest-neighbor technique. [Scale] must be a positive (>=0.1f) scalar.
*	Returns -1 on error.
*/
int	blit_img_scaled(t_imgdata *img, t_buffer *buf, t_point offs, float scale)
{
	t_imgdata	s_img;

	if (!img || !buf || scale < 0.1f)
		return (-1);
	if (offs.x > buf->w || offs.y > buf->h)
		return (-1);
	s_img.w = (uint32_t)(img->w * scale);
	s_img.h = (uint32_t)(img->h * scale);
	s_img.data = ft_memalloc(s_img.w * s_img.h);
	if (!s_img.data)
		return (-1);
	copy_loop(img, &s_img, img->w / (double)s_img.w, img->h / (double)s_img.h);
	if (blit_img(&s_img, buf, offs) < 0)
	{
		free(s_img.data);
		return (-1);
	}
	free(s_img.data);
	return (0);
}
