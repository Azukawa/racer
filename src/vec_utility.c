#include "racer.h"

/*
*	Calculates the distance between 2d vectors v1 and v2.
*/
float	vector_dist(t_vector v1, t_vector v2)
{
	return (sqrt(((v1.x - v2.x) * (v1.x - v2.x)) + \
			((v1.y - v2.y) * (v1.y - v2.y))));
}

/*
*	Returns the addition of vectors [v1] and [v2].
*/
t_vector	vector_add(t_vector v1, t_vector v2)
{
	t_vector	v;

	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	return (v);
}

/*
*	Returns the result of vector [v1] substracted by [v2].
*/
t_vector	vector_sub(t_vector v1, t_vector v2)
{
	t_vector	v;

	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	return (v);
}

/*
*	Returns the result of [v1] divided by the scalar [div].
*/
t_vector	vector_div(t_vector v1, float div)
{
	t_vector	v;

	v.x = v1.x / div;
	v.y = v1.y / div;
	return (v);
}

/*
*	Returns the result of [v1] multiplied by the scalar [mul].
*/
t_vector	vector_mul(t_vector v1, float mul)
{
	t_vector	v;

	v.x = v1.x * mul;
	v.y = v1.y * mul;
	return (v);
}

/*
*	Vector is normalized i.e. turned into an unit vector, by dividing each of
*	its components by its magnitude.
*/
t_vector	vector_normalize(t_vector v)
{
	t_vector	ret;
	float		m;

	m = (float)sqrt(v.x * v.x + v.y * v.y);
	ret.x = v.x / m;
	ret.y = v.y / m;
	return (ret);
}
