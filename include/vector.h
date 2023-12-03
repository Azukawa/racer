#ifndef VECTOR_H
# define VECTOR_H

typedef struct s_vector
{
	float	x;
	float	y;
}			t_vector;

t_vector	vector_add(t_vector v1, t_vector v2);
t_vector	vector_sub(t_vector v1, t_vector v2);
t_vector	vector_mul(t_vector v1, float mul);
t_vector	vector_div(t_vector v1, float div);
float		vector_dist(t_vector v1, t_vector v2);
t_vector	vector_normalize(t_vector v);

#endif
