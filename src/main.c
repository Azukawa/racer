#include "racer.h"
# define DEG_TO_RAD 0.01745329251
# define RAD_TO_DEG 57.2957795131
/*
*	Todo:	read up on windowflags in case we could have additional features
*			Q:do we need sethint() and should it be to linear or nearest?
*			same thing with rendermodes, pixelformat etc.
*			A:Currently we don't use any SDL texture transformation funcs so its
*			probably not needed!
*			blending is only needed if we work with multiple texture layers
*			(and if we want to blend them together)
*/
static void	init(t_rend *renderer, t_race *race)
{
	ft_bzero(renderer, sizeof(t_rend));
	ft_bzero(race, sizeof(t_race));
	renderer->win_buffer = (t_buffer *)malloc(sizeof(t_buffer));
	if (!renderer->win_buffer)
		ft_getout("failed to initialize main buffer");
	renderer->win_buffer->w = WIN_W;
	renderer->win_buffer->h = WIN_H;
	renderer->win_buffer->pixels = (uint32_t *)ft_memalloc(WIN_H * WIN_W);
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		ft_getout(SDL_GetError());
	renderer->win = SDL_CreateWindow(WIN_NAME, SDL_WINDOWPOS_UNDEFINED, \
		SDL_WINDOWPOS_UNDEFINED, WIN_W, WIN_H, 0);
	if (!renderer->win)
		ft_getout(SDL_GetError());
	renderer->rend = SDL_CreateRenderer(renderer->win, -1, \
		SDL_RENDERER_ACCELERATED);
	if (!renderer->rend)
		ft_getout(SDL_GetError());
	renderer->win_tex = SDL_CreateTexture(renderer->rend, \
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIN_W, WIN_H);
	if (!renderer->win_tex)
		ft_getout(SDL_GetError());
	renderer->run = TRUE;
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048 ) != 0)
		ft_getout(SDL_GetError());
	
	race->ppos.x = 100;
	race->ppos.y = 200;
}

static void	cleanup(t_rend *renderer)
{
	SDL_DestroyTexture(renderer->win_tex);
	SDL_DestroyRenderer(renderer->rend);
	SDL_DestroyWindow(renderer->win);
	free(renderer->win_buffer->pixels);
	free(renderer->win_buffer);
	Mix_Quit();
	SDL_Quit();
}

t_iv3	v32iv3(t_v3 v)
{
	t_iv3	ret;

	ret.x = v.x;
	ret.y = v.y;
	ret.z = v.z;
	return (ret);
}

t_point	v2p(t_v3 v)
{
	t_point	ret;
	
	ret.x = v.x;
	ret.y = v.y;
	return (ret);
}

/*
*	Note about SDL_LockTexture: void **pixels is 'filled in', meaning that SDL
*	creates its own allocated pixel buffer thats returned to the given pointer.
*	This is why you need to query for the pixel_pitch too since its the only
*	way to know the 1-directional pitch of the created buffer.
*/
void	draw_2_window(t_rend *rend)
{
	if (SDL_LockTexture(rend->win_tex, NULL, \
		&rend->win_pixels, &rend->win_pixel_pitch) < 0)
		ft_getout(SDL_GetError());
	ft_memcpy(rend->win_pixels, rend->win_buffer->pixels, \
	WIN_H * rend->win_pixel_pitch);
	SDL_UnlockTexture(rend->win_tex);
	if (SDL_RenderCopy(rend->rend, rend->win_tex, NULL, NULL) < 0)
		ft_getout(SDL_GetError());
	SDL_RenderPresent(rend->rend);

}


void	keyevent(SDL_Event *e, t_rend *rend, t_race	*race)
{
//	bzero(&race->k, sizeof(t_keys));
	while (SDL_PollEvent(e))
	{
//		const Uint8* key = SDL_GetKeyboardState(NULL);

		if (e->window.event == SDL_WINDOWEVENT_CLOSE || e->key.keysym.sym == SDLK_ESCAPE)
			rend->run = FALSE;
//		if(key[SDL_SCANCODE_UP])
//			race->k.u = 1;
//		if(key[SDL_SCANCODE_DOWN])
//			race->k.d = 1;
//		if(key[SDL_SCANCODE_LEFT])
//			race->k.l = 1;
//		if(key[SDL_SCANCODE_RIGHT])
//			race->k.r = 1;
		if (e->key.keysym.sym == SDLK_UP && e->type == SDL_KEYDOWN)
			race->k.u = 1;
		if (e->key.keysym.sym == SDLK_UP && e->type == SDL_KEYUP)
			race->k.u = 0;
		if (e->key.keysym.sym == SDLK_DOWN && e->type == SDL_KEYDOWN)
			race->k.d = 1;
		if (e->key.keysym.sym == SDLK_DOWN && e->type == SDL_KEYUP)
			race->k.d = 0;
		if (e->key.keysym.sym == SDLK_LEFT && e->type == SDL_KEYDOWN)
			race->k.l = 1;
		if (e->key.keysym.sym == SDLK_LEFT && e->type == SDL_KEYUP)
			race->k.l = 0;
		if (e->key.keysym.sym == SDLK_RIGHT && e->type == SDL_KEYDOWN)
			race->k.r = 1;
		if (e->key.keysym.sym == SDLK_RIGHT && e->type == SDL_KEYUP)
			race->k.r = 0;
	}

}

uint32_t	get_delta()
{
	static uint32_t	tics;
	uint32_t			pretics;

	pretics = tics;
	tics = SDL_GetTicks();
	return((tics - pretics));
}

t_v3	v3_add(t_v3 a, t_v3 b)
{
	a.x = a.x + b.x;
	a.y = a.y + b.y;
	a.z = a.z + b.z;
	return(a);
}

t_v3   v_sub(t_v3 a, t_v3 b)
{
	t_v3   ret;

	ret.x = a.x - b.x;
	ret.y = a.y - b.y;
	ret.z = a.z - b.z;
	return (ret);
}
  
t_v3   v_div(t_v3 vector, float c)
{
	vector.x = vector.x / c;
	vector.y = vector.y / c;
	vector.z = vector.z / c;
	return (vector);
}
  
t_v3   v_mult(t_v3 vector, float c)
{
	vector.x = vector.x * c;
	vector.y = vector.y * c;
	vector.z = vector.z * c;
	return (vector);
}

void	v_rot_x(t_v3 *vec, float rad)
{
	t_v3	prev;

	prev.z = vec->z;
	prev.y = vec->y;
	vec->z = prev.z * cos(rad) - prev.y * sin(rad);
	vec->y = prev.z * sin(rad) + prev.y * cos(rad);
}

void	v_rot_y(t_v3 *vec, float rad)
{
	t_v3	prev;

	prev.x = vec->x;
	prev.z = vec->z;
	vec->x = prev.x * cos(rad) - prev.z * sin(rad);
	vec->z = prev.x * sin(rad) + prev.z * cos(rad);
}

void	v_rot_z(t_v3 *vec, float rad)
{
	t_v3	prev;

	prev.y = vec->y;
	prev.x = vec->x;
	vec->x = prev.x * cos(rad) - prev.y * sin(rad);
	vec->y = prev.x * sin(rad) + prev.y * cos(rad);
}

t_v3	v_rot_xyz(t_v3 vec, t_v3 rot)
{
	t_v3	ret;

	ret = vec;
	v_rot_x(&ret, DEG_TO_RAD * rot.x);
	v_rot_y(&ret, DEG_TO_RAD * rot.y);
	v_rot_z(&ret, DEG_TO_RAD * rot.z);
	return (ret);
}

t_v3	v3_cross(t_v3 va, t_v3 vb)
{
	t_v3	result;

	result.x = va.y * vb.z - va.z * vb.y;
	result.y = va.z * vb.x - va.x * vb.z;
	result.z = va.x * vb.y - va.y * vb.x;
	return (result);
}

float   v3_dot(t_v3 va, t_v3 vb)
{
    return (va.x * vb.x + va.y * vb.y + va.z * vb.z);
}

float   v2_dot(t_v3 va, t_v3 vb)
{
    return (va.x * vb.x + va.y * vb.y);
}

t_v3	rad2vec3(float dir)
{
	t_v3	ret = {0,0,0};
	
	ret.x = cos(dir);
	ret.y = sin(dir);
	return (ret);
}
float	lerp_1d(float start, float end, float t)
{
		return (start + t * (end - start));
}

t_v3	v3_lerp(t_v3 va, t_v3 vb, float t)
{
	t_v3	ret = {0,0,0};
	
	ret.x = lerp_1d(va.x, vb.x, t);
	ret.y = lerp_1d(va.y, vb.y, t);
	ret.z = lerp_1d(va.z, vb.z, t);
	return (ret);
}


/*
*	Multiplying negative values with themselves will always result in positive
*	values, therefore sqrt() call will always success.
*	(Additionally, it does not trip onto possible NaN / Inf values.)
*/
float	v_len(t_v3 v)
{
	return (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
}

t_v3	v_normalize(t_v3 v)
{
	t_v3	ret;
	float		l;

	l = v_len(v);
	ret.x = v.x / l;
	ret.y = v.y / l;
	ret.z = v.z / l;
	return (ret);
}

//This function is supposed to slow down sideways velocity
//and re-direct forward velocity
t_v3	tire_friction(t_v3 velocity, t_race *race, float drift)
{
//	t_v3	right = race->pdir;
//	v_rot_z(&right, 90 * DEG_TO_RAD);
//	t_v3	forvel = v_mult(race->pdir,  v3_dot(velocity, race->pdir));
//	t_v3	rightvel = v_mult(right,  v3_dot(velocity, right));

//	velocity = v_mult(v3_add(forvel, rightvel), drift);
//	velocity = v3_add(forvel, rightvel);
//	velocity = v3_lerp(race->pvel, forvel, 0.99);
	v_rot_z(&race->pvel, race->p_rot);
	return (velocity);
}


void	friction(t_v3 *velocity, t_race *race)
{
//	float		friction = 0.998;
	float		friction = 0.998;
	
	*velocity = v_mult(*velocity, friction);
	race->p_rot = race->p_rot * 0.99;
//	*velocity = tire_friction(*velocity, race, 0.0001);
}

//Add vertical down force to player velocity each tick
//to simulate gravity. Modify gravity strenght with force.
t_v3	gravity(t_v3 velocity, float force, uint32_t delta)
{

	velocity.y = velocity.y + (force * delta);
	return(velocity);
}

float	uusfunktio(t_race *race)
{
	static float max_speed = 1.5;
	t_v3	speed = race->pvel;
//	v_rot_z(&speed, -race->f_pdir);
	speed.y = fabs(speed.y);
	if(speed.y > max_speed)
		return(0);
//	printf("speed = %f/tick\n", speed.y);
	return(1);
}

// Apply forces to player velocity and player rotation
// according to buttons pressed.
// Then rotate velocity to the direction of player and add rotation to player direction.
// Then add friction and gravity to player velocity.
void	move(t_race *race)
{
	t_v3			temp_vel	= {0, 0, 0};
	static float	turn		= 0.00003;
	float			speed		= 0.00185;
	uint32_t		delta		= get_delta();
	
	if(race->k.u)
		temp_vel.y -= speed * delta;
	else if(race->k.d)
		temp_vel.y += speed * delta;
	if(race->k.l)
		race->p_rot -= turn * delta;
	else if(race->k.r)
		race->p_rot += turn * delta;
	race->f_pdir += race->p_rot;
	race->pdir = rad2vec3(race->f_pdir);
	v_rot_z(&temp_vel, race->f_pdir);
//	uusfunktio(race);
	race->pvel = v3_add(race->pvel, temp_vel);
	v_rot_z(&race->pvel, race->p_rot * uusfunktio(race));

//	float dot = v3_dot(v_normalize(race->pvel), v_normalize(race->pdir));
//	dot = fabs(dot);
//	printf("dot = %f\n", dot);
	
	friction(&race->pvel, race);
//	race->pvel = tire_friction(race->pvel, race, 0.0001);
//	race->pvel = gravity(race->pvel, 0.0007, delta);
	race->ppos = v3_add(race->ppos, race->pvel);
}

//Draws graphics.
//So far only "car".
void	graphics(t_rend *rend, t_race	*race)
{
	int		aimline = -30;
	t_v3	temp = {0,aimline,0};
	t_line	aim;

//center circle of car
	draw_circle(rend->win_buffer, v2p(race->ppos), 7, 0xFFFFFF);
//Rotate point in front of player pos and draw a line to it
	v_rot_z(&temp, race->f_pdir);
	temp = v3_add(temp, race->ppos);
	aim.start = v32iv3(race->ppos);
	aim.end = v32iv3(temp);
	draw_vector_line(rend->win_buffer, aim, 0x00FF00);
//This dumbest line of code in the world
//clones and mirrors front arrow behind the center of car
	aim.end.x = (((aim.end.x - race->ppos.x) *  -1) + race->ppos.x);
	aim.end.y = (((aim.end.y - race->ppos.y) *  -1) + race->ppos.y);	
	draw_vector_line(rend->win_buffer, aim, 0xFF0000);
//This draws the turqise line in the direction of player velocity(x 10)	
	aim.end = v32iv3(v3_add(v_mult(race->pvel, 10), race->ppos));
	draw_vector_line(rend->win_buffer, aim, 0x00FFFF);
}

static void	loop(t_rend *rend, SDL_Event *e, t_race	*race)
{
	ft_bzero(rend->win_buffer->pixels, WIN_H * WIN_W * sizeof(uint32_t));
	keyevent(e, rend, race);
	move(race);
	graphics(rend, race);
	draw_2_window(rend);
}

int	main(void)
{
	t_rend		rend;
	SDL_Event	e;
	t_race		race;

	init(&rend, &race);
	while (rend.run)
		loop(&rend, &e, &race);
	cleanup(&rend);
	return (0);
}
