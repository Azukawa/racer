#ifndef RESOURCEFILE_H
# define RESOURCEFILE_H

# include <stdint.h>

# define RF_HEADERSIZE 18
# define RF_LUMPCOUNT_POSITION 2
# define RF_LUMPLIST_OFFSET 10
# define RF_LUMPINFOSIZE 32

# define RF_TYPE_TGA 01

typedef struct s_rf_lump {
	off_t				offset;
	off_t				bytesize;
	struct s_rf_lump	*next;
	short				id;
	char				type;
}						t_rf_lump;

typedef struct s_rf {
	off_t		lumpcount;
	off_t		lumpdata_offset;
	t_rf_lump	*lumplist;
	int			fd;
}				t_rf;

int		rf_open_resourcefile(char openmode, char *filename);
void	rf_close_fd(t_rf *rf);

t_bool	rf_create_tga_lump(t_rf *rf, const char *asset);
t_bool	rf_load_tga_into_rf(t_rf *rf, const char *filepath);

off_t	get_charray_value(unsigned char *p, unsigned int s);
void	set_charray_value(unsigned char *p, off_t value, unsigned int size);

t_bool	rf_create_lumpinfo(t_rf *rf, off_t	bytesize, char type);
void	rf_free_lumplist(t_rf_lump *head);
void	rf_write_lumplist(t_rf *rf);

#endif
