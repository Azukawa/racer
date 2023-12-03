#include "racer.h"

t_point	px = {WIN_W/2, WIN_H/2};
t_point	p1 = {0, 0};
t_point	p2 = {0, WIN_H};
t_point	p3 = {WIN_W, WIN_H};
t_point	p4 = {WIN_W, 0};
float	counter = 0.0;

static void	drawlinetest(t_buffer *buf)
{
	t_point	mp;
	counter += 0.001;
	mp.x = ft_i_lerp(p1.x, p2.x, counter);
	mp.y = ft_i_lerp(p1.y, p2.y, counter);
	draw_line(buf, px, mp, ft_color_lerp(0x006666ff, 0x00ff6666, counter));
	mp.x = ft_i_lerp(p2.x, p3.x, counter);
	mp.y = ft_i_lerp(p2.y, p3.y, counter);
	draw_line(buf, px, mp, ft_color_lerp(0x00ff6666, 0x00ffff66, counter));
	mp.x = ft_i_lerp(p3.x, p4.x, counter);
	mp.y = ft_i_lerp(p3.y, p4.y, counter);
	draw_line(buf, px, mp, ft_color_lerp(0x00ffff66, 0x0066ffff, counter));
	mp.x = ft_i_lerp(p4.x, p1.x, counter);
	mp.y = ft_i_lerp(p4.y, p1.y, counter);
	draw_line(buf, px, mp, ft_color_lerp(0x0066ffff, 0x006666ff, counter));
	if (counter >= 1.0)
		counter = 0.0f;
}

static void	tga_load_test(t_buffer *buf, t_assets *assets)
{	
	static uint32_t	x;
	static uint32_t	y;
	static float	size = 1;

	blit_img_scaled(assets->testimg001, buf, (t_point){x, y}, size);
	blit_img_scaled(assets->testimg002, buf, (t_point){x + 50, y + 50}, size);
	blit_img_scaled(assets->testimg003, buf, (t_point){x + 100, y + 100}, size);
	if (y < WIN_H)
	{
		if (x < WIN_W)
			x++;
		else
		{
			size -= 0.1f;
			x = 0;
			y += WIN_H / 10;
		}
	}
	else
	{
		size = 1.f;
		y = 0;
	}
}

static void	gridtest(t_buffer *buf)
{
	int gridw = 8;
	int gridcount = 16;
	int squaresize = 100;
	int xorig = 0;
	int	yorig = 400;

	int i = 0;
	while (i < gridcount)
	{
		int x = (i % gridw) * squaresize + xorig;
		int y = (i / gridw) * squaresize + yorig;
		draw_square((t_point){x, y}, (t_point){x + squaresize, y + squaresize},
		buf, ft_color_lerp(0x008b53b2, 0x00ee9f3c, i / (double)gridcount));
		i++;
	}
}

static void sprite_test(t_buffer *buf, t_assets *assets, t_audio *audio)
{
	static int i = 0;
	static int counter = 0;
	blit_img(assets->sprite->img, buf, (t_point){0, 0});
	blit_sprite(assets->sprite, buf, i, (t_point){0, 0});
	if (counter > 100)
	{
		counter = 0;
		i++;
		if (i > 16)
			i = 0;
		audio->sound_trigger = TRUE;
	}
	counter++;
}

void	init_tests(t_rf *rf, t_assets *assets)
{
	//init_boids_positions(assets->flock);
	rf->fd = rf_open_resourcefile('w', "DATA");
	//add_tga_to_rf(rf, "resources/a.tga");
	//add_tga_to_rf(rf, "resources/b.tga");
	//add_tga_to_rf(rf, "resources/c.tga");
	add_tga_to_rf(rf, "resources/ikaruga.tga");
	rf_write_lumplist(rf);
	rf_close_fd(rf);
	/*assets->testimg001 = load_tga_from_rf(rf, 001);
	if (!(assets->testimg001))
		ft_getout("failed to load test image001");
	ft_printf("asset 001 loaded successfully!\n");
	assets->testimg002 = load_tga_from_rf(rf, 002);
	if (!assets->testimg002)
		ft_getout("failed to load test image002");
	ft_printf("asset 002 loaded successfully!\n");
	assets->testimg003 = load_tga_from_rf(rf, 003);
	if (!assets->testimg003)
		ft_getout("failed to load test image003");
	ft_printf("asset 003 loaded successfully!\n");*/
	assets->sprite_tester = load_tga_from_rf(rf, 001);
	if (!assets->sprite_tester)
		ft_getout("failed to load spritesheet TGA");
	ft_printf("asset [ikaruga.tga] loaded successfully!\n");
	rf_free_lumplist(rf->lumplist);
	assets->sprite = create_sprite(assets->sprite_tester, 16, \
		(t_point){200, 200});
	if (!assets->sprite)
		ft_getout("spritesheet creation failed!");
}

void	cleanup_tests(t_assets *assets)
{
	//free(assets->testimg001->data);
	//free(assets->testimg001);
	//free(assets->testimg002->data);
	//free(assets->testimg002);
	//free(assets->testimg003->data);
	//free(assets->testimg003);
	free(assets->sprite->img->data);
	free(assets->sprite->img);
	free(assets->sprite);
}

void	dotests(t_buffer *buf, t_assets *assets, t_audio *audio)
{
	sprite_test(buf, assets, audio);
	//gridtest(buf);
	//drawlinetest(buf);
	//tga_load_test(buf, assets);
	//update_boids(assets->flock, buf);
}
