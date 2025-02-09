#include "gifdec.h"

#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#if ANDROID
#include <unistd.h>
#endif

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))

typedef struct Entry {
	uint16_t length;
	uint16_t prefix;
	uint8_t  suffix;
} Entry;

typedef struct Table {
	int32_t bulk;
	int32_t nentries;
	Entry *entries;
} Table;

static uint16_t read_num(IResource* fd) {
	uint8_t bytes[2];

	fd->Read(bytes, 2); // readed(fd, bytes, 2);;
	return bytes[0] + (((uint16_t)bytes[1]) << 8);
}

gd_GIF* gd_open_gif(IResource* fd) {//const char *fname) {
	//SDL_RWops* fd;
	uint8_t sigver[3];
	uint16_t width, height, depth;
	uint8_t fdsz, bgidx, aspect;
	int32_t gct_sz;
	gd_GIF* gif = NULL;


	#if MSVC
	//fd = SDL_RWFromFile(fname, "rb");
	#else
	//fd = SDL_RWFromFile(fname, "r");
	#endif

	if (!fd)
		printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");


	if (fd == NULL) return NULL;
	/* Header */
	fd->Read(sigver, 3); // readed(fd, sigver, 3);;
	if (memcmp(sigver, "GIF", 3) != 0) {
		fprintf(stderr, "invalid signature\n");
		goto fail;
	}
	/* Version */
	fd->Read(sigver, 3); // readed(fd, sigver, 3);;
	if (memcmp(sigver, "89a", 3) != 0) {
		fprintf(stderr, "invalid version\n");
		//goto fail;
	}
	/* Width x Height */
	width = read_num(fd);
	height = read_num(fd);
	/* FDSZ */
	fdsz = 0;
	fd->Read(&fdsz, 1 * 1); // readed(fd, &fdsz, 1);;
	/* Presence of GCT */
	if (!(fdsz & 0x80)) {
		fprintf(stderr, "no global color table\n");
		goto fail;
	}
	/* Color Space's Depth */
	depth = ((fdsz >> 4) & 7) + 1;
	/* Ignore Sort Flag. */
	/* GCT Size */
	gct_sz = 1 << ((fdsz & 0x07) + 1);
	/* Background Color Index */
	fd->Read(&bgidx, 1 * 1); // readed(fd, &bgidx, 1);;
	/* Aspect Ratio */
	fd->Read(&aspect, 1 * 1); // readed(fd, &aspect, 1);;
	/* Create gd_GIF Structure. */
	gif = (gd_GIF*)calloc(1, sizeof(*gif) + 5 * width * height);
	if (!gif) goto fail;
	gif->fd = fd;
	gif->width = width;
	gif->height = height;
	gif->depth = depth;
	/* Read GCT */
	gif->gct.size = gct_sz;
	fd->Read(gif->gct.colors, 3 * gif->gct.size * 1); // readed(fd, gif->gct.colors, 3 * gif->gct.size);;
	gif->palette = &gif->gct;
	gif->bgindex = bgidx;
	gif->canvas = (uint8_t *)&gif[1];
	gif->frame = &gif->canvas[3 * width * height];
	if (gif->bgindex) {
		memset(gif->frame, gif->bgindex, gif->width * gif->height);
    }
	gif->anim_start = fd->Seek(0, RW_SEEK_CUR);
	goto ok;
fail:
	//SDL_RWclose(fd);
	return gif;
ok:
	return gif;
}

gd_GIF* gd_copy_gif(gd_GIF *other) {
	if (other == NULL) {
		return NULL;
	}
    gd_GIF* gif = (gd_GIF*)calloc(1, sizeof(*gif) + 5 * other->width * other->height);
    
    memcpy(gif, other, sizeof(*gif) + 5 * other->width * other->height);
    gif->gct.size = other->gct.size;
    memcpy(gif->gct.colors, other->gct.colors, sizeof(other->gct.colors));
    gif->lct.size = other->lct.size;
    memcpy(gif->lct.colors, other->lct.colors, sizeof(other->lct.colors));
    
    gif->palette = &gif->gct;
    gif->canvas = (uint8_t *)&gif[1];
    gif->frame = &gif->canvas[3 * other->width * other->height];
	if (gif->bgindex) {
		memset(gif->frame, gif->bgindex, gif->width * gif->height);
    }
    return gif;
}

static void discard_sub_blocks(gd_GIF *gif) {
	uint8_t size;

	do {
		gif->fd->Read(&size, 1); // readed(gif->fd, &size, 1);;
		gif->fd->Seek(size, RW_SEEK_CUR);
	} while (size);
}

static void read_plain_text_ext(gd_GIF *gif) {
	if (gif->plain_text) {
		uint16_t tx, ty, tw, th;
		uint8_t cw, ch, fg, bg;
		off_t sub_block;
		gif->fd->Seek(1, RW_SEEK_CUR); /* block size = 12 */
		tx = read_num(gif->fd);
		ty = read_num(gif->fd);
		tw = read_num(gif->fd);
		th = read_num(gif->fd);
		gif->fd->Read(&cw, 1 * 1); // readed(gif->fd, &cw, 1);;
		gif->fd->Read(&ch, 1 * 1); // readed(gif->fd, &ch, 1);;
		gif->fd->Read(&fg, 1 * 1); // readed(gif->fd, &fg, 1);;
		gif->fd->Read(&bg, 1 * 1); // readed(gif->fd, &bg, 1);;
		sub_block = gif->fd->Seek(0, RW_SEEK_CUR);
		gif->plain_text(gif, tx, ty, tw, th, cw, ch, fg, bg);
		gif->fd->Seek(sub_block, RW_SEEK_SET);
	}
	else {
		/* Discard plain text metadata. */
		gif->fd->Seek(13, RW_SEEK_CUR);
	}
	/* Discard plain text sub-blocks. */
	discard_sub_blocks(gif);
}

static void read_graphic_control_ext(gd_GIF *gif) {
	uint8_t rdit;

	/* Discard block size (always 0x04). */
	gif->fd->Seek(1, RW_SEEK_CUR);
	gif->fd->Read(&rdit, 1 * 1); // readed(gif->fd, &rdit, 1);;
	gif->gce.disposal = (rdit >> 2) & 3;
	gif->gce.input = rdit & 2;
	gif->gce.transparency = rdit & 1;
	gif->gce.delay = read_num(gif->fd);
	gif->fd->Read(&gif->gce.tindex, 1 * 1); // readed(gif->fd, &gif->gce.tindex, 1);;
	/* Skip block terminator. */
	gif->fd->Seek(1, RW_SEEK_CUR);
}

static void read_comment_ext(gd_GIF *gif) {
	if (gif->comment) {
		off_t sub_block = gif->fd->Seek(0, RW_SEEK_CUR);
		gif->comment(gif);
		gif->fd->Seek(sub_block, RW_SEEK_SET);
	}
	/* Discard comment sub-blocks. */
	discard_sub_blocks(gif);
}

static void read_application_ext(gd_GIF *gif) {
	char app_id[8];
	char app_auth_code[3];

	/* Discard block size (always 0x0B). */
	gif->fd->Seek(1, RW_SEEK_CUR);
	/* Application Identifier. */
	gif->fd->Read(app_id, 8 * 1); // readed(gif->fd, app_id, 8);;
	/* Application Authentication Code. */
	gif->fd->Read(app_auth_code, 3 * 1); // readed(gif->fd, app_auth_code, 3);;
	if (!strncmp(app_id, "NETSCAPE", sizeof(app_id))) {
		/* Discard block size (0x03) and constant byte (0x01). */
		gif->fd->Seek(2, RW_SEEK_CUR);
		gif->loop_count = read_num(gif->fd);
		/* Skip block terminator. */
		gif->fd->Seek(1, RW_SEEK_CUR);
	}
	else if (gif->application) {
		off_t sub_block = gif->fd->Seek(0, RW_SEEK_CUR);
		gif->application(gif, app_id, app_auth_code);
		gif->fd->Seek(sub_block, RW_SEEK_SET);
		discard_sub_blocks(gif);
	}
	else {
		discard_sub_blocks(gif);
	}
}

static void read_ext(gd_GIF *gif) {
	uint8_t label;

	gif->fd->Read(&label, 1 * 1); // readed(gif->fd, &label, 1);;
	switch (label) {
	case 0x01:
		read_plain_text_ext(gif);
		break;
	case 0xF9:
		read_graphic_control_ext(gif);
		break;
	case 0xFE:
		read_comment_ext(gif);
		break;
	case 0xFF:
		read_application_ext(gif);
		break;
	default:
		fprintf(stderr, "unknown extension: %02X\n", label);
	}
}

static Table* new_table(int32_t key_size) {
	int32_t key;
	int32_t init_bulk = MAX(1 << (key_size + 1), 0x100);
	Table* table = (Table*)malloc(sizeof(*table) + sizeof(Entry) * init_bulk);
	if (table) {
		table->bulk = init_bulk;
		table->nentries = (1 << key_size) + 2;
		table->entries = (Entry*)(table + 1);
		for (key = 0; key < (1 << (key_size + 1)); key++)
			table->entries[key] = Entry { 1, 0xFFF, (uint8_t)key };
	}
	return table;
}

/* Add table entry. Return value:
*  0 on success
*  +1 if key size must be incremented after this addition
*  -1 if could not realloc table */
static int32_t add_entry(Table **tablep, uint16_t length, uint16_t prefix, uint8_t suffix) {
	Table *table = *tablep;
	if (table->nentries == table->bulk) {
		table->bulk *= 2;
		table = (Table*)realloc(table, sizeof(*table) + sizeof(Entry) * table->bulk);
		if (!table) return -1;
		table->entries = (Entry*)&table[1];
		*tablep = table;
	}

	table->entries[table->nentries] = Entry { length, prefix, suffix };
	//printf("entry %X {length=0x%04X prefix=0x%04X suffix=0x%02X}\n", table->nentries, table->entries[table->nentries].length, table->entries[table->nentries].prefix, table->entries[table->nentries].suffix);
	table->nentries++;
	if ((table->nentries & (table->nentries - 1)) == 0)
		return 1;
	return 0;
}

static uint16_t get_key(gd_GIF *gif, int32_t key_size, uint8_t *sub_len, uint8_t *shift, uint8_t *byte) {
	int32_t bits_read;
	int32_t rpad;
	int32_t frag_size;
	uint16_t key;

	key = 0;
	for (bits_read = 0; bits_read < key_size; bits_read += frag_size) {
		rpad = (*shift + bits_read) % 8;
		if (rpad == 0) {
			/* Update byte. */
			if (*sub_len == 0)
				gif->fd->Read(sub_len, 1 * 1); // readed(gif->fd, sub_len, 1);; /* Must be nonzero! */
			gif->fd->Read(byte, 1 * 1); // readed(gif->fd, byte, 1);;
			(*sub_len)--;
		}
		frag_size = MIN(key_size - bits_read, 8 - rpad);
		key |= ((uint16_t)((*byte) >> rpad)) << bits_read;
	}
	/* Clear extra bits to the left. */
	key &= (1 << key_size) - 1;
	*shift = (*shift + key_size) % 8;
	return key;
}

/* Decompress image pixels.
* Return 0 on success or -1 on out-of-memory (w.r.t. LZW code table). */
static int32_t read_image_data(gd_GIF *gif) {
	uint8_t sub_len, shift, byte;
	int32_t init_key_size, key_size, table_is_full;
	int32_t frm_off, str_len, p;
	uint16_t key, clear, stop;
	int32_t ret;
	Table *table;
	Entry entry;
	entry.suffix = 0;
	off_t start, end;

	gif->fd->Read(&byte, 1 * 1); // readed(gif->fd, &byte, 1);;
	key_size = (int32_t)byte;
	start = gif->fd->Seek(0, RW_SEEK_CUR);
	discard_sub_blocks(gif);
	end = gif->fd->Seek(0, RW_SEEK_CUR);
	gif->fd->Seek(start, RW_SEEK_SET);
	clear = 1 << key_size;
	stop = clear + 1;
	table = new_table(key_size);
	key_size++;
	init_key_size = key_size;
	sub_len = shift = 0;
	key = get_key(gif, key_size, &sub_len, &shift, &byte); /* clear code */
	frm_off = 0;
	str_len = 0;
	ret = 0;
	table_is_full = 0;
	while (1) {
		if (key == clear) {
			key_size = init_key_size;
			table->nentries = (1 << (key_size - 1)) + 2;
			table_is_full = 0;
		}
		else if (!table_is_full) {
			ret = add_entry(&table, str_len + 1, key, entry.suffix);
			if (ret == -1) {
				free(table);
				return -1;
			}
			if (table->nentries == 0x1000) {
				ret = 0;
				table_is_full = 1;
			}
		}
		key = get_key(gif, key_size, &sub_len, &shift, &byte);
		if (key == clear) continue;
		if (key == stop) break;
		if (ret == 1) key_size++;
		entry = table->entries[key];
		str_len = entry.length;
		while (1) {
			p = frm_off + entry.length - 1;
			int delawhere = (gif->fy + p / gif->fw) * gif->width + gif->fx + p % gif->fw;
			gif->frame[delawhere] = entry.suffix;
			if (entry.prefix == 0xFFF)
				break;
			else {
				entry = table->entries[entry.prefix];
			}
		}
		frm_off += str_len;
		if (key < table->nentries - 1 && !table_is_full)
			table->entries[table->nentries - 1].suffix = entry.suffix;
	}
	free(table);
	gif->fd->Read(&sub_len, 1 * 1); // readed(gif->fd, &sub_len, 1);; /* Must be zero! */
	gif->fd->Seek(end, RW_SEEK_SET);
	return 0;
}

/* Read image.
* Return 0 on success or -1 on out-of-memory (w.r.t. LZW code table). */
static int32_t read_image(gd_GIF *gif) {
	uint8_t fisrz;
	int32_t interlace;

	/* Image Descriptor. */

	gif->fx = read_num(gif->fd);
	gif->fy = read_num(gif->fd);
	gif->fw = read_num(gif->fd);
	gif->fh = read_num(gif->fd);

	gif->fd->Read(&fisrz, 1 * 1); // readed(gif->fd, &fisrz, 1);;
	interlace = fisrz & 0x40;
	/* Ignore Sort Flag. */
	/* Local Color Table? */
	if (fisrz & 0x80) {
		/* Read LCT */
		gif->lct.size = 1 << ((fisrz & 0x07) + 1);
		gif->fd->Read(gif->lct.colors, 3 * gif->lct.size * 1); // readed(gif->fd, gif->lct.colors, 3 * gif->lct.size);;
		gif->palette = &gif->lct;
	}
	else
		gif->palette = &gif->gct;
	/* Image Data. */

	return read_image_data(gif);
}

static void render_frame_rect(gd_GIF *gif, uint8_t *buffer) {
	int32_t i, j, k;
	i = gif->fy * gif->width + gif->fx;
	for (j = 0; j < gif->fh; j++) {
		for (k = 0; k < gif->fw; k++) {
			memcpy(&buffer[i + k], &gif->frame[(gif->fy + j) * gif->width + gif->fx + k], 1);
		}
		i += gif->width;
	}
}

static void dispose(gd_GIF *gif) {
	int32_t i, j, k;
	uint8_t *bgcolor;

	switch (gif->gce.disposal) {
	case 2: /* Restore to background color. */


		bgcolor = &gif->palette->colors[gif->bgindex * 3];
		i = gif->fy * gif->width + gif->fx;
		for (j = 0; j < gif->fh; j++) {
			for (k = 0; k < gif->fw; k++)
				memcpy(&gif->canvas[(i + k) * 3], bgcolor, 3);
			i += gif->width;
		}

		break;
	case 3: /* Restore to previous, i.e., don't update canvas.*/
		break;
	default:
		/* Add frame non-transparent pixels to canvas. */

		render_frame_rect(gif, gif->canvas);

	}
}

/* Return 1 if got a frame; 0 if got GIF trailer; -1 if error. */
int32_t gd_get_frame(gd_GIF *gif) {
	char sep;

	dispose(gif);

	gif->fd->Read(&sep, 1); // readed(gif->fd, &sep, 1);;

	while (sep != ',') {
		if (sep == ';')
			return 0;
		if (sep == '!')
			read_ext(gif);
		else return -1;
		gif->fd->Read(&sep, 1); // readed(gif->fd, &sep, 1);;
	}
	if (read_image(gif) == -1)
		return -1;
	return 1;
}

void gd_render_frame(gd_GIF *gif, uint8_t *buffer) {
	//memcpy(buffer, gif->canvas, gif->width * gif->height * 3);
	render_frame_rect(gif, buffer);
}

void gd_rewind(gd_GIF *gif) {
	gif->fd->Seek(gif->anim_start, RW_SEEK_SET);
}

void gd_close_gif(gd_GIF *gif) {
    if (gif == NULL) {
        return;
    }
	free(gif);
}
