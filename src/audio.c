#include "racer.h"

void	init_audio(t_audio *audio)
{
	ft_bzero(audio, sizeof(t_audio));
	audio->sound = NULL;
	if( SDL_Init(SDL_INIT_AUDIO) < 0 )
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
	audio->sound = Mix_LoadWAV("resources/turning-pages-02.wav");

}

void audio_cleanup(t_audio *audio)
{
	Mix_FreeChunk(audio->sound);
	audio->sound = NULL;
}

float		ft_lerp_1d(float start, float end, float t)
{
	return (start + t * (end - start));

}
float	my_sin(int time, int amp, int freq, int phase)
{
	float ret = 0;
	float wat = 0;

	//wat = 2 * 3.1415 * freq;
	wat = 2 * 3.1415;
	//ret = amp * sin(wat * time + phase);
	ret = amp * sin((wat * time) / 44.1);
	ret = (ret + 1) / 2;
	printf("[%f]", ret);
	return (ret);
}

void	audios(t_audio *audio)
{
	int i = 0;
	if (audio->sound_trigger == TRUE)
	{
		Mix_PlayChannel(-1, audio->sound, 0);
		while(i < (int)audio->sound->alen)
		{
//			printf("(%u)", audio->sound->abuf[i]);
//			audio->sound->abuf[i] = ft_lerp_1d(0, 255, my_sin(i % 441, 1, 1000, 0));
			audio->sound->abuf[i] = ft_lerp_1d(0, 255, my_sin(i % 441, 1, 1000, 0));
//			audio->sound->abuf[i] = rand();
			i++;
		}
		printf("\n");
		printf("allocated\t= %i\n", audio->sound->allocated);
		printf("alen\t\t= %d\n", audio->sound->alen);
		printf("volume\t\t= %d\n\n", audio->sound->volume);

		audio->sound->volume = audio->sound->volume / 2;
//		audio->sound->alen = audio->sound->alen / 2;
		
		audio->sound_trigger = FALSE;
	}

}

