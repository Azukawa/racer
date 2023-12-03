#ifndef BOID_H
# define BOID_H

# include "vector.h"

# define BOID_COUNT 200
# define BOID_MAXSPEED 1.2f

//testing simple AI system
typedef struct s_boid
{
	t_vector	position;
	t_vector	velocity;
	int			index;
}				t_boid;

void	init_boids_positions(t_boid *flock);

#endif