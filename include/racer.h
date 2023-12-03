#ifndef DOOM_H
# define DOOM_H

# include <stdlib.h>
# include <unistd.h>
# include <stdint.h> //for datatypes
# include <string.h> //for sterror
# include <errno.h> //for errno macro

# include "../libSDL2/include/SDL2/SDL_mixer.h"
# include "../libSDL2/include/SDL2/SDL.h"

# include "../libft/includes/libft.h"
# include "../libft/includes/ft_gfx.h"

# include "boid.h"
# include "vector.h"
# include "resourcefile.h"

# define WIN_W 800
# define WIN_H 600
# define WIN_NAME "Drive!"

typedef struct s_buffer {
	uint32_t		*pixels;
	uint32_t		w;
	uint32_t		h;
}					t_buffer;

typedef struct s_rend
{
	SDL_Renderer	*rend;
	SDL_Window		*win;
	SDL_Texture		*win_tex;
	void			*win_pixels;
	t_buffer		*win_buffer;
	int				win_pixel_pitch;
	t_bool			run;
}					t_rend;

//this should probably be generic memory unit for all 'images'
//thats why we probably need offset values. Also BPP might be useless since
//every buffer wants the same 32-bit ARGB values.
//(into which all filedata will be stored inside program memory)
typedef struct s_imgdata {
	uint32_t	*data;
	uint32_t	w;
	uint32_t	h;
}				t_imgdata;

//sprite in the context of this program refers to struct that 
//holds a spritesheet and from which you can blit individual sprites from.
typedef struct s_sprite {
	t_imgdata	*img;
	uint32_t	sprite_count;
	uint32_t	sprite_w;
	uint32_t	sprite_h;
	uint32_t	width;
}				t_sprite;

//place to hold all graphics etc. data we need. Also development stuff
typedef struct s_assets {
	t_imgdata	*testimg001;
	t_imgdata	*testimg002;
	t_imgdata	*testimg003;
	t_imgdata	*sprite_tester;
	t_sprite	*sprite;
	t_boid		flock[BOID_COUNT];
}				t_assets;

//datatype for handling buffer/array coordinates
typedef struct s_point {
	uint32_t	x;
	uint32_t	y;
}				t_point;

typedef struct	s_audio{
	Mix_Chunk	*sound;
	t_bool		sound_trigger;
}				t_audio;

typedef struct	s_v3{
	float 	x;
	float	y;
	float	z;
}				t_v3;

typedef struct s_iv3 {
	int			x;
	int			y;
	int			z;
}				t_iv3;

typedef struct s_line {
	t_iv3		start;
	t_iv3		end;
	uint32_t	color;
}				t_line;

typedef	struct	s_keys{
	t_bool	u;
	t_bool	d;
	t_bool	l;
	t_bool	r;
}				t_keys;

typedef struct	s_race{
	
	t_v3	ppos;
	t_v3	pdir;
	t_keys	k;
	float	f_pdir;
	float	p_rot;
}				t_race;

int			blit_img(t_imgdata *img, t_buffer *buf, t_point start);
int			blit_img_scaled(t_imgdata *img, t_buffer *buf, \
t_point offs, float scale);

t_sprite	*create_sprite(t_imgdata *img, uint32_t spr_count, \
t_point spr_unit);
t_bool		blit_sprite(t_sprite *sprite, t_buffer *buf, int i, t_point pos);

void		draw_pixel(uint32_t x, uint32_t y, t_buffer *buf, uint32_t color);
void		draw_line(t_buffer *buf, t_point p0, t_point p1, uint32_t color);
void		draw_circle(t_buffer *buf, t_point p, int r, uint32_t color);
void		draw_filled_circle(t_buffer *buf, t_point p, int r, uint32_t color);
void		draw_square(t_point a, t_point b, t_buffer *buf, int color);

t_imgdata	*load_tga(const char *filepath);
void		add_tga_to_rf(t_rf *rf, const char *filepath);
t_imgdata	*load_tga_from_rf(t_rf *rf, short lump_id);
t_imgdata	*rf_load_tga_lump(t_rf *rf, short lump_id);

void		update_boids(t_boid *flock, t_buffer *buf);
void		init_tests(t_rf *rf, t_assets *assets);
void		cleanup_tests(t_assets *assets);
void		dotests(t_buffer *buf, t_assets *assets, t_audio *audio);

void		init_audio(t_audio *audio);
void		audio_cleanup(t_audio *audio);
void		audios(t_audio *audio);
void		draw_vector_line(t_buffer *buf, t_line line, uint32_t c);

#endif
