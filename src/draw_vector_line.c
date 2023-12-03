#include "racer.h"

/*
*	Initializes outcodes for Cohen-Sutherland-styled clipping of an
*	integer vector into unsigned buffer space.
*	Bitwise OR adds the applicable outcodes together.
*	INSIDE	0	[0000]
*	LEFT	1	[0001]
*	RIGHT	2	[0010]
*	BOTTOM	4	[0100]
*	TOP		8	[1000]
*
*	1001 | 1000 | 1010
*	-----+------+-----
*	0001 | 0000 | 0010
*	-----+------+-----
*	0101 | 0100 | 0110
*/
static int	get_ocode(t_buffer *buf, t_iv3 v)
{
	int	outcode;

	outcode = 0;
	if (v.x < 0)
		outcode |= 1;
	else if (v.x > (int)buf->w)
		outcode |= 2;
	if (v.y < 0)
		outcode |= 4;
	else if (v.y > (int)buf->h)
		outcode |= 8;
	return (outcode);
}

/*
*	Finds the screen-space intersection point between [o] and [e].
*	Depending on outcode, point coordinates are scaled in relation to
*	(distance to screen edge from p0) / (distance to p1 from p0).
*/
static t_iv3	line_clip(t_buffer *buf, t_iv3 v0, t_iv3 v1, int ocode)
{
	t_iv3	rv;

	if (ocode & 8)
	{
		rv.x = v0.x + (v1.x - v0.x) * ((int)buf->h - v0.y) / (v1.y - v0.y);
		rv.y = (int)buf->h;
	}
	else if (ocode & 4)
	{
		rv.x = v0.x + (v1.x - v0.x) * -v0.y / (v1.y - v0.y);
		rv.y = 0;
	}
	else if (ocode & 2)
	{
		rv.y = v0.y + (v1.y - v0.y) * ((int)buf->w - v0.x) / (v1.x - v0.x);
		rv.x = (int)buf->w;
	}
	else if (ocode & 1)
	{
		rv.y = v0.y + (v1.y - v0.y) * -v0.x / (v1.x - v0.x);
		rv.x = 0;
	}
	return (rv);
}

/*
*	Note: Outcodes are stored as a single vector where
*	[ocodes.x] = start,
*	[ocodes.y] = end,
*	[ocodes.z] = out.
*	[ocode.x & ocode.y] checks if both points share an outside region, therefore
*	existing out of bounds.
*	(!(ocode.x | ocode.y)) if both points are in screenspace (outcode 0), exit
*/

/*
*	Handling line clipping and drawing in the given unsigned buffer space [buf]
*	using the Cohen-Sutherland algorithm.
*/
void	draw_vector_line(t_buffer *buf, t_line line, uint32_t c)
{
	t_iv3	ocode;

	ocode = (t_iv3){get_ocode(buf, line.start), get_ocode(buf, line.end), 0};
	while (TRUE)
	{
		if (ocode.x & ocode.y)
			return ;
		if (!(ocode.x | ocode.y))
			return (draw_line(buf, (t_point){line.start.x, line.start.y}, \
				(t_point){line.end.x, line.end.y}, c));
		if (ocode.y > ocode.x)
			ocode.z = ocode.y;
		else
			ocode.z = ocode.x;
		if (ocode.z == ocode.x)
		{
			line.start = line_clip(buf, line.start, line.end, ocode.z);
			ocode.x = get_ocode(buf, line.start);
		}
		else
		{
			line.end = line_clip(buf, line.start, line.end, ocode.z);
			ocode.y = get_ocode(buf, line.end);
		}
	}
}
