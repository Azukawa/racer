#include "racer.h"

/*
*	Called once to initialize boid flock starting positions
*/
void	init_boids_positions(t_boid *flock)
{
	int	i;

	i = 0;
	while (i < BOID_COUNT)
	{
		flock[i].index = i;
		flock[i].position.x = (float)(rand() % WIN_W);
		flock[i].position.y = (float)(rand() % WIN_H);
		flock[i].velocity.x = ((rand() % 100) / 100.f);
		flock[i].velocity.y = ((rand() % 100) / 100.f);
		i++;
	}
}

static void	draw_boids(t_boid *flock, t_buffer *buf)
{
	int	i;

	i = 0;
	while (i < BOID_COUNT)
	{
		draw_pixel((int)flock[i].position.x, (int)flock[i].position.y, buf, \
		0x00FFFFFF);
		draw_circle(buf, \
			(t_point){(int)flock[i].position.x, (int)flock[i].position.y}, 5, \
			0x006666ff);
		i++;
	}
}

/*
*	Rule 1: each individual boid will try to fly towards the 'center of mass'
*	of other nearby boids. For each tick the boid will move 1% towards it.
*/
static t_vector	flock_together(t_boid boid, t_boid *flock, float r)
{
	t_vector	avg;
	int			i;
	int			j;
	float		d;

	avg = (t_vector){0.0f, 0.0f};
	i = 0;
	j = 0;
	while (i < BOID_COUNT)
	{
		d = vector_dist(boid.position, flock[i].position);
		if (i != boid.index && d < r)
		{
			avg = vector_add(avg, flock[i].position);
			j++;
		}
		i++;
	}
	if (j > 0)
	{
		avg = vector_sub(vector_div(avg, j), boid.position);
		avg = vector_mul(vector_normalize(avg), BOID_MAXSPEED);
		avg = vector_normalize(vector_sub(avg, boid.velocity));
	}
	return (avg);
}

/*
*	Rule 2: Each boid tries to avoid collision between each other boid.
*	Returned vector is the sum of the all nearby boid offsets.
*/
static t_vector	separate(t_boid boid, t_boid *flock, float r)
{
	t_vector	avg;
	int			i;
	int			j;
	float		d;

	avg = (t_vector){0.0f, 0.0f};
	i = 0;
	j = 0;
	while (i < BOID_COUNT)
	{
		d = vector_dist(boid.position, flock[i].position);
		if (i != boid.index && (d < r))
		{
			avg = vector_add(avg, vector_div(vector_sub(boid.position, \
					flock[i].position), d * d));
			j++;
		}
		i++;
	}
	if (j > 0)
	{
		avg = vector_mul(vector_normalize(vector_div(avg, j)), BOID_MAXSPEED);
		avg = vector_normalize(vector_sub(avg, boid.velocity));
	}
	return (avg);
}

/*
*	Rule 3: Boids try to match the movement of other near boids. 
*	Calculates the average movement vector of each other boid within given
*	range. Substracting the current velocity from the calculated average gives
*	us the new direction.
*/
static t_vector	align_movement(t_boid boid, t_boid *flock, float r)
{
	t_vector	avg;
	int			i;
	int			j;
	float		d;

	avg = (t_vector){0.0f, 0.0f};
	i = 0;
	j = 0;
	while (i < BOID_COUNT)
	{
		d = vector_dist(boid.position, flock[i].position);
		if (i != boid.index && (d < r))
		{
			avg = vector_add(avg, flock[i].velocity);
			j++;
		}
		i++;
	}
	if (j > 0)
	{
		avg = vector_mul(vector_normalize(vector_div(avg, j)), BOID_MAXSPEED);
		avg = vector_normalize(vector_sub(avg, boid.velocity));
	}
	return (avg);
}

/*
*	Handle simulation in screen space
*/
static void	bound_position(t_boid *boid)
{
	if (boid->position.x < 0)
		boid->position.x = WIN_W;
	if (boid->position.x > WIN_W)
		boid->position.x = 0;
	if (boid->position.y < 0)
		boid->position.y = WIN_H;
	if (boid->position.y > WIN_H)
		boid->position.y = 0;
}

/*
*	Run the three behavior logic functions to create three new vectors,
*	which are then used to calculate each boid's new position
*	(very basic movement, velocity being vector sum on position coordinates)
*/
static void	update_boids_positions(t_boid *flock)
{
	int			i;
	t_vector	v1;
	t_vector	v2;
	t_vector	v3;

	v1 = (t_vector){0.0, 0.0};
	v2 = (t_vector){0.0, 0.0};
	v3 = (t_vector){0.0, 0.0};
	i = 0;
	while (i < BOID_COUNT)
	{
		v1 = flock_together(flock[i], flock, 250.f);
		v2 = separate(flock[i], flock, 100.f);
		v3 = align_movement(flock[i], flock, 100.f);
		flock[i].velocity.x = flock[i].velocity.x + v1.x + v2.x + v3.x;
		flock[i].velocity.y = flock[i].velocity.y + v1.y + v2.y + v3.y;
		flock[i].velocity = vector_mul(vector_normalize(flock[i].velocity), \
		BOID_MAXSPEED);
		flock[i].position = vector_add(flock[i].position, flock[i].velocity);
		bound_position(&flock[i]);
		i++;
	}
}

/*
*	Called every frame to update the boids AI simulation
*/
void	update_boids(t_boid *flock, t_buffer *buf)
{
	draw_boids(flock, buf);
	update_boids_positions(flock);
}
