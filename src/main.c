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

float   v_dot(t_v3 va, t_v3 vb)
{
    return (va.x * vb.x + va.y * vb.y + va.z * vb.z);
}
/*
t_v3	side_friction(t_v3 velocity, t_race *race, float drift)
{
	t_v3	right =  race->pdi
	velocity = 
	return ();
}*/

void	friction(t_v3 *velocity, t_race *race)
{
	float		friction = 0.998;
	
	*velocity = v_mult(*velocity, friction);
	race->p_rot = race->p_rot * friction;
	
}

t_v3	gravity(t_v3 velocity, float force, uint32_t delta)
{

	velocity.y = velocity.y + (force * delta);
	return(velocity);
}

void	move(t_race *race)
{
	static	t_v3	velocity 	= {0,0,0};	
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
	v_rot_z(&temp_vel, race->f_pdir);
	velocity = v3_add(velocity, temp_vel);
	friction(&velocity, race);
	velocity = gravity(velocity, 0.0007, delta);
	race->ppos = v3_add(race->ppos, velocity);
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