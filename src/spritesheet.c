#include "racer.h"

/*
*	[spr_count] is the number of sprites generated from the given image.
*	[spr_unit] is the length and width of each generated sprite.
*	Creates a struct that stores all the necessary data to blit individual
*	sprites from the given spritesprite image.
*/
t_sprite	*create_sprite(t_imgdata *img, uint32_t spr_count, t_point spr_unit)
{
	t_sprite	*sprite;
	uint32_t	y;
	uint32_t	x;

	sprite = (t_sprite *)malloc(sizeof(t_sprite));
	if (!sprite || !img)
		return (NULL);
	sprite->img = img;
	sprite->width = 0;
	sprite->sprite_count = spr_count;
	y = 0;
	while (spr_unit.y * y <= img->h)
	{
		x = 0;
		while (spr_unit.x * x <= img->w)
			x++;
		if (x > sprite->width)
			sprite->width = x;
		y++;
	}
	sprite->sprite_w = img->w / sprite->width;
	sprite->sprite_h = img->h / y;
	img = NULL;
	return (sprite);
}

t_bool	blit_sprite(t_sprite *sprite, t_buffer *buf, int frame, t_point start)
{
	uint32_t	x;
	uint32_t	y;

	if (!sprite || !sprite->img || !sprite->img->data || !buf)
		return (FALSE);
	if (start.x > buf->w || start.y > buf->h)
		return (FALSE);
	y = 0;
	while ((y + start.y) <= buf->h && y < sprite->sprite_h)
	{
		x = 0;
		while ((x + start.y) <= buf->w && x < sprite->sprite_w)
		{
			buf->pixels[((y + start.y) * buf->w) + x + start.x] = \
			sprite->img->data[(((frame / sprite->width) * \
					sprite->sprite_h + y) * sprite->img->w) + \
			((frame % sprite->width) * sprite->sprite_w + x)];
			x++;
		}
		y++;
	}
	return (TRUE);
}
