/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:54:01 by eniini            #+#    #+#             */
/*   Updated: 2021/08/10 15:01:37 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "racer.h"

/*
*	Initializing bunch of values in separate functions to appease Norminette.
*	Otherwise pixel-accurate line drawing algorithm between 2 point coordinates.
*
*	Initial checks swap point data if needed since drawfunc always traverses
*	from left to right. [flip] checks if we're incrementing through x or y.
*	After that, drawline() plots a line of pixels from [point0] to [point1].
*/

static void	init_errors(t_point p0, t_point p1, int *derror, int *error)
{
	uint32_t	ydiff;

	if (p1.y > p0.y)
		ydiff = p1.y - p0.y;
	else
		ydiff = p0.y - p1.y;
	*derror = ydiff * 2;
	*error = 0;
}

static t_bool	init_points(t_point *p0, t_point *p1)
{
	t_bool		flip;
	uint32_t	xdiff;
	uint32_t	ydiff;

	flip = FALSE;
	if (p0->x > p1->x)
		xdiff = p0->x - p1->x;
	else
		xdiff = p1->x - p0->x;
	if (p0->y > p1->y)
		ydiff = p0->y - p1->y;
	else
		ydiff = p1->y - p0->y;
	if (xdiff < ydiff)
	{
		ft_swap(&p0->x, &p0->y, sizeof(int));
		ft_swap(&p1->x, &p1->y, sizeof(int));
		flip = TRUE;
	}
	if (p0->x > p1->x)
	{
		ft_swap(&p0->x, &p1->x, sizeof(int));
		ft_swap(&p0->y, &p1->y, sizeof(int));
	}
	return (flip);
}

/*
*	Bresenham's line algorithm.
*/
void	draw_line(t_buffer *buf, t_point p0, t_point p1, uint32_t color)
{
	int		derror;
	int		error;
	t_point	crawler;
	t_bool	flip;

	flip = init_points(&p0, &p1);
	crawler = (t_point){p0.x, p0.y};
	init_errors(p0, p1, &derror, &error);
	while (crawler.x <= p1.x)
	{
		if (!flip)
			draw_pixel(crawler.x++, crawler.y, buf, color);
		else
			draw_pixel(crawler.y, crawler.x++, buf, color);
		error += derror;
		if (error > (int)p1.x - (int)p0.x)
		{
			if (p1.y > p0.y)
				crawler.y += 1;
			else
				crawler.y += -1;
			error -= ((int)p1.x - (int)p0.x) * 2;
		}
	}
}
