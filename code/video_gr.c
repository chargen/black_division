/*
 * video_gr.c
 *
 * Created 28/11/2013
 * Author: ei12054
 */

#include "video_gr.h"
#include "vbe.h"
#include "game.h"

/* Private global variables */

static unsigned char *video_mem;		/**< @brief Process address to which VRAM is mapped */
static unsigned char *buffer;			/**< @brief pointer to the video buffer's adress */
static unsigned int buffer_size;		/**< @brief buffer size */

static unsigned h_res;			/**< @brief Horizontal screen resolution in pixels */
static unsigned v_res;			/**< @brief Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /**< @brief Number of VRAM bits per pixel */

const int default_width = 2;

/** @name Chars */
/**@{
 *
 * Chars Relative Position
 */
enum {
	SYMBOL_A,
	SYMBOL_B,
	SYMBOL_C,
	SYMBOL_D,
	SYMBOL_E,
	SYMBOL_F,
	SYMBOL_G,
	SYMBOL_H,
	SYMBOL_I,
	SYMBOL_J,
	SYMBOL_K,
	SYMBOL_L,
	SYMBOL_M,
	SYMBOL_N,
	SYMBOL_O,
	SYMBOL_P,
	SYMBOL_Q,
	SYMBOL_R,
	SYMBOL_S,
	SYMBOL_T,
	SYMBOL_U,
	SYMBOL_V,
	SYMBOL_W,
	SYMBOL_X,
	SYMBOL_Y,
	SYMBOL_Z
};
/**@} end of Chars */

/** @name Vbe Info Struct */
/**@{
 *
 * Info received by the VBE
 */
typedef struct
{
	phys_bytes vram_base;
	unsigned long vram_size;
	void * vram_base_virtual;
	unsigned h_res;
	unsigned v_res;
	unsigned bits_per_pixel;
} VBE_INFO;
/**@} end of Vbe Info Struct */

int vg_init(unsigned short mode) {

	struct reg86u reg86;
	struct mem_range range;

	VBE_INFO vb_info;
	vbe_mode_info_t vmi_p;

	/* Get mode information from vbe */
	if(vbe_get_mode_info(mode, &vmi_p) != 0) {
		return -1;
	}

	vb_info.vram_base = vmi_p.PhysBasePtr;
	vb_info.h_res = vmi_p.XResolution;
	vb_info.v_res = vmi_p.YResolution;
	vb_info.bits_per_pixel = vmi_p.BitsPerPixel;
	vb_info.vram_size = vb_info.h_res * vb_info.v_res * vb_info.bits_per_pixel;

	//range.mr_base = vmi_p.PhysBasePtr;
	range.mr_base = (phys_bytes)(vb_info.vram_base);
	range.mr_limit = range.mr_base + vb_info.vram_size;

	if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &range) != OK) {
		return -1;
	}

	video_mem = vm_map_phys(SELF, (void *)range.mr_base, vb_info.vram_size);

	buffer = malloc(vb_info.h_res * vb_info.v_res * (vb_info.bits_per_pixel / 8));

	if(buffer == NULL) {
		return -1;
	}

	buffer_size = vb_info.h_res * vb_info.v_res * (vb_info.bits_per_pixel / 8);

	reg86.u.b.ah = 0x4f;
	reg86.u.b.al = 0x02;
	reg86.u.w.bx = 1 << 14 | mode;
	reg86.u.b.intno = 0x10;

	if(sys_int86(&reg86) != OK) {
		return -1;
	}

	h_res = vb_info.h_res;
	v_res = vb_info.v_res;
	bits_per_pixel = vb_info.bits_per_pixel;

	return 0;
}

int vg_exit() {

  struct reg86u reg86;

  reg86.u.b.intno = 0x10; /**< @brief BIOS video services */
  reg86.u.b.ah = 0x00;    /**< @brief Set Video Mode function */
  reg86.u.b.al = 0x03;    /**< @brief 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      return -1;
  }

  return 0;
}

void swap(unsigned int *a, unsigned int *b) {
	unsigned int c = *a;
	*a = *b;
	*b = c;
}

int vg_fill(unsigned int color) {

	unsigned long i = 0;
	unsigned long size = (h_res * v_res);

	for (i = 0; i < size; i++) {
		*(buffer + i) = color;
	}

	return 0;
}

int vg_set_pixel(unsigned int x, unsigned int y, unsigned int color) {

	if((x >= h_res) || (y >= v_res)) {
		return -1;
	}

	*(buffer + x + y*h_res) = color;

	return 0;
}

unsigned long vg_get_pixel(unsigned int x, unsigned int y) {
	if((x >= h_res) || (y >= v_res)) {
		return -1;
	}
	else {
		return *(video_mem + x + y*h_res);
	}
}

int vg_draw_buffer_to_mem() {
	unsigned char *ptr_buff = buffer;
	unsigned char *ptr_mem = video_mem;
	unsigned int i = 0;

	while(i < buffer_size) {
		i++;
		*ptr_mem = *ptr_buff;
		ptr_buff++;
		ptr_mem++;
	}

	return 0;
}

int vg_draw_number(const unsigned int xi, const unsigned int yi, const unsigned int color, unsigned long number, const unsigned int size) {

	/*
	 *  -0-
	 * |   |
	 * 1   2
	 * |   |
	 *  -3-
	 * |   |
	 * 4   5
	 * |   |
	 *  -6-
	 */
	unsigned long mask = 0x0;

	unsigned long x = xi;
	unsigned long y = yi;

	unsigned int width = 0;

	switch((unsigned int)(number)) {
	case 0:
		mask = 0x77; // 111 0111
		break;
	case 1:
		mask = 0x24; // 010 0100
		break;
	case 2:
		mask = 0x5D; // 101 1101
		break;
	case 3:
		mask = 0x6D; // 110 1101
		break;
	case 4:
		mask = 0x2E; // 010 1110
		break;
	case 5:
		mask = 0x6B; // 110 1011
		break;
	case 6:
		mask = 0x7B; // 111 1011
		break;
	case 7:
		mask = 0x25; // 010 0101
		break;
	case 8:
		mask = 0x7F; // 111 1111
		break;
	case 9:
		mask = 0x6F; // 110 1111
		break;
	default:
		return -1;
	}

	/* Horizontal lines */
	if(BIT0(mask)) {
		x += default_width;
		for(; width < default_width; width++, y++) {
			if(vg_draw_line(x, y, x+size, y, color) == -1) {
				return -1;
			}
		}
		width = 0;
		y = yi;
		x = xi;
	}
	if(BIT3(mask)) {
		x += default_width;
		y += default_width + size;
		for(; width < default_width; width++, y++) {
			if(vg_draw_line(x, y, x+size, y, color) == -1) {
				return -1;
			}
		}
		width = 0;
		y = yi;
		x = xi;
	}
	if(BIT6(mask)) {
		x += default_width;
		y += 2*default_width + 2*size;
		for(; width < default_width; width++, y++) {
			if(vg_draw_line(x, y, x+size, y, color) == -1) {
				return -1;
			}
		}
		width = 0;
		y = yi;
		x = xi;
	}
	/* Vertical lines */
	if(BIT1(mask)) {
		y += default_width;
		for(; width < default_width; width++, x++) {
			if(vg_draw_line(x, y, x, y+size, color) == -1) {
				return -1;
			}
		}
		width = 0;
		y = yi;
		x = xi;
	}
	if(BIT4(mask)) {
		y += 2*default_width + size;
		for(; width < default_width; width++, x++) {
			if(vg_draw_line(x, y, x, y+size, color) == -1) {
				return -1;
			}
		}
		width = 0;
		y = yi;
		x = xi;
	}
	if(BIT2(mask)) {
		x += default_width + size;
		y += default_width;
		for(; width < default_width; width++, x++) {
			if(vg_draw_line(x, y, x, y+size, color) == -1) {
				return -1;
			}
		}
		width = 0;
		y = yi;
		x = xi;
	}
	if(BIT5(mask)) {
		x += default_width + size;
		y += 2*default_width + size;
		for(; width < default_width; width++, x++) {
			if(vg_draw_line(x, y, x, y+size, color) == -1) {
				return -1;
			}
		}
		width = 0;
		y = yi;
		x = xi;
	}

	return (size);
}

int vg_draw_special(const unsigned int xi, const unsigned int yi, const unsigned int color, const char symbol, const unsigned int size) {
	unsigned long Hmask = 0x0;
	unsigned long Vmask = 0x0;

	unsigned int width = 0;

	unsigned long x = xi;
	unsigned long y = yi;

	if(symbol == '(') {
		Hmask = 0x101; // 01 0000 0001
		Vmask = 0x055; // 00 0101 0101
	}
	else if(symbol == ')') {
		Hmask = 0x202; // 10 0000 0010
		Vmask = 0x0AA; // 00 1010 1010
	}
	else if(symbol == '\'') {
		Vmask = 0x100; // 01 0000 0000
	}

	if(Hmask) {
		if(BIT0(Hmask)) {
			for(; width < default_width; width++, y++) {
				if(vg_draw_line(x, y, x+size, y, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
		if(BIT1(Hmask)) {
			x += size;
			for(; width < default_width; width++, y++) {
				if(vg_draw_line(x, y, x+size, y, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
		if(BIT2(Hmask)) {
			y += size;
			for(; width < default_width; width++, y++) {
				if(vg_draw_line(x, y, x+size, y, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
		if(BIT3(Hmask)) {
			x += size + default_width;
			y += size;
			for(; width < default_width; width++, y++) {
				if(vg_draw_line(x, y, x+size, y, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
		if(BIT4(Hmask)) {
			y += 2*size;
			for(; width < default_width; width++, y++) {
				if(vg_draw_line(x, y, x+size, y, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
		if(BIT5(Hmask)) {
			x += size;
			y += 2*size;
			for(; width < default_width; width++, y++) {
				if(vg_draw_line(x, y, x+size, y, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
		if(BIT6(Hmask)) {
			y += 3*size;
			for(; width < default_width; width++, y++) {
				if(vg_draw_line(x, y, x+size, y, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
		if(BIT7(Hmask)) {
			x += size;
			y += 3*size;
			for(; width < default_width; width++, y++) {
				if(vg_draw_line(x, y, x+size, y, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
		if(BIT8(Hmask)) {
			y += 4*size;
			for(; width < default_width; width++, y++) {
				if(vg_draw_line(x, y, x+size, y, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
		if(BIT9(Hmask)) {
			x += size;
			y += 4*size;
			for(; width < default_width; width++, y++) {
				if(vg_draw_line(x, y, x+size, y, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
	}
	// Vertical lines
	if(Vmask) {
		if(BIT0(Vmask)) {
			for(; width < default_width; width++, x++) {
				if(vg_draw_line(x, y, x, y+size, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
		if(BIT1(Vmask)) {
			x += 2*size;
			for(; width < default_width; width++, x++) {
				if(vg_draw_line(x, y, x, y+size, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
		if(BIT2(Vmask)) {
			y += size;
			for(; width < default_width; width++, x++) {
				if(vg_draw_line(x, y, x, y+size, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
		if(BIT3(Vmask)) {
			x += 2*size;
			y += size;
			for(; width < default_width; width++, x++) {
				if(vg_draw_line(x, y, x, y+size, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
		if(BIT4(Vmask)) {
			y += 2*size;
			for(; width < default_width; width++, x++) {
				if(vg_draw_line(x, y, x, y+size, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
		if(BIT5(Vmask)) {
			x += 2*size;
			y += 2*size;
			for(; width < default_width; width++, x++) {
				if(vg_draw_line(x, y, x, y+size, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
		if(BIT6(Vmask)) {
			y += 3*size;
			for(; width < default_width; width++, x++) {
				if(vg_draw_line(x, y, x, y+size, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
		if(BIT7(Vmask)) {
			x += 2*size;
			y += 3*size;
			for(; width < default_width; width++, x++) {
				if(vg_draw_line(x, y, x, y+size, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
		if(BIT8(Vmask)) {
			x += size;
			for(; width < default_width; width++, x++) {
				if(vg_draw_line(x, y, x, y+2*size, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
		if(BIT9(Vmask)) {
			x += size;
			y += 2*size;
			for(; width < default_width; width++, x++) {
				if(vg_draw_line(x, y, x, y+2*size, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
	}

	return 0;
}

int vg_draw_char(unsigned int xi, unsigned int yi, const unsigned int color, const char symbol, const unsigned int size) {

	int number_code = 48;
	int to_lower = 97;

	unsigned long Hmask = 0x0;
	unsigned long Vmask = 0x0;
	unsigned long Dmask = 0x0;

	unsigned short int lowered = 0;
	unsigned short int duplicate = 0;
	unsigned short int dot = 0;
	unsigned short int doTimes = 1;

	unsigned long x = xi;
	unsigned long y = yi;

	unsigned int width = 0;

	if((symbol >= '0') && (symbol <= '9')) {
		return vg_draw_number(xi,yi,color,((unsigned int)(symbol) - number_code), 2*size);
	}

	if((symbol == '(') || (symbol == '\'') || (symbol == ')')) {
		return vg_draw_special(xi,yi,color,symbol,size);
	}

	switch(((int)(symbol) - to_lower)) {
	case SYMBOL_A:
		Hmask = 0x3F0; // 11 1111 0000
		Vmask = 0x0E0; // 00 1110 0000
		break;
	case SYMBOL_B:
		Hmask = 0x330; // 11 0011 0000
		Vmask = 0x0F5; // 00 1111 0101
		break;
	case SYMBOL_C:
		Hmask = 0x330; // 11 0011 0000
		Vmask = 0x050; // 00 0101 0000
		break;
	case SYMBOL_D:
		Hmask = 0x330; // 11 0011 0000
		Vmask = 0x0FA; // 00 1111 1010
		break;
	case SYMBOL_E:
		Hmask = 0x3F0; // 11 1111 0000
		Vmask = 0x070; // 00 0111 0000
		break;
	case SYMBOL_F:
		Hmask = 0x032; // 00 0011 0010
		Vmask = 0x300; // 11 0000 0000
		break;
	case SYMBOL_G:
		Hmask = 0x333; // 11 0011 0011
		Vmask = 0x0EF; // 00 1110 1111
		lowered = 1;
		break;
	case SYMBOL_H:
		Hmask = 0x030; // 00 0011 0000
		Vmask = 0x0F5; // 00 1111 0101
		break;
	case SYMBOL_I:
		Vmask = 0x200; // 10 0000 0000
		dot = 1;
		break;
	case SYMBOL_J:
		Hmask = 0x100; // 01 0000 0000
		Vmask = 0x340; // 11 0100 0000
		lowered = 1;
		dot = 1;
		break;
	case SYMBOL_K:
		Vmask = 0x055; // 00 0101 0101
		Dmask = 0x05;  // 101
		break;
	case SYMBOL_L:
		Vmask = 0x300; // 11 0000 0000
		break;
	case SYMBOL_M:
		Hmask = 0x030; // 00 0011 0000
		Vmask = 0x0F0; // 00 1111 0000
		duplicate = 1;
		break;
	case SYMBOL_N:
		Hmask = 0x030; // 00 0011 0000
		Vmask = 0x0F0; // 00 1111 0000
		break;
	case SYMBOL_O:
		Hmask = 0x330; // 11 0011 0000
		Vmask = 0x0F0; // 00 1111 0000
		break;
	case SYMBOL_P:
		Hmask = 0x033; // 00 0011 0011
		Vmask = 0x05F; // 00 0101 1111
		lowered = 1;
		break;
	case SYMBOL_Q:
		Hmask = 0x033; // 00 0011 0011
		Vmask = 0x0AF; // 00 1010 1111
		lowered = 1;
		break;
	case SYMBOL_R:
		Hmask = 0x020; // 00 0010 0000
		Vmask = 0x200; // 10 0000 0000
		break;
	case SYMBOL_S:
		Hmask = 0x3F0; // 11 1111 0000
		Vmask = 0x090; // 00 1001 0000
		break;
	case SYMBOL_T:
		Hmask = 0x00C; // 00 0000 1100
		Vmask = 0x300; // 11 0000 0000
		break;
	case SYMBOL_U:
		Hmask = 0x300; // 11 0000 0000
		Vmask = 0x0F0; // 00 1111 0000
		break;
	case SYMBOL_V:
		Vmask = 0x050; // 00 0101 0000
		Dmask = 0x02; // 010
		break;
	case SYMBOL_W:
		Vmask = 0x050; // 00 0101 0000
		Dmask = 0x02; // 010
		duplicate = 1;
		break;
	case SYMBOL_X:
		Dmask = 0x06; // 110
		break;
	case SYMBOL_Y:
		Hmask = 0x330; // 11 0011 0000
		Vmask = 0x0AF; // 00 1010 1111
		lowered = 1;
		break;
	case SYMBOL_Z:
		Hmask = 0x330; // 11 0011 0000
		Dmask = 0x02; // 010
		break;
	default:
		return 0;
	}

	if(lowered) {
		yi += 2*size;
		y += 2*size;
		if(dot) {
			for(; width < default_width; width++, y++) {
				if(vg_draw_line(x+size, y-size, x+size+default_width, y-size, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
	}
	else {
		if(dot) {
			for(; width < default_width; width++, y++) {
				if(vg_draw_line(x+size, y+size, x+size+default_width, y+size, color) == -1) {
					return -1;
				}
			}
			width = 0;
			y = yi;
			x = xi;
		}
	}

	if(duplicate) {
		doTimes += 1;
	}

	while(doTimes > 0) {
		// Horizontal lines
		if(Hmask) {
			if(BIT0(Hmask)) {
				for(; width < default_width; width++, y++) {
					if(vg_draw_line(x, y, x+size, y, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT1(Hmask)) {
				x += size;
				for(; width < default_width; width++, y++) {
					if(vg_draw_line(x, y, x+size, y, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT2(Hmask)) {
				y += size;
				for(; width < default_width; width++, y++) {
					if(vg_draw_line(x, y, x+size, y, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT3(Hmask)) {
				x += size + default_width;
				y += size;
				for(; width < default_width; width++, y++) {
					if(vg_draw_line(x, y, x+size, y, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT4(Hmask)) {
				y += 2*size;
				for(; width < default_width; width++, y++) {
					if(vg_draw_line(x, y, x+size, y, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT5(Hmask)) {
				x += size;
				y += 2*size;
				for(; width < default_width; width++, y++) {
					if(vg_draw_line(x, y, x+size, y, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT6(Hmask)) {
				y += 3*size;
				for(; width < default_width; width++, y++) {
					if(vg_draw_line(x, y, x+size, y, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT7(Hmask)) {
				x += size;
				y += 3*size;
				for(; width < default_width; width++, y++) {
					if(vg_draw_line(x, y, x+size, y, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT8(Hmask)) {
				y += 4*size;
				for(; width < default_width; width++, y++) {
					if(vg_draw_line(x, y, x+size, y, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT9(Hmask)) {
				x += size;
				y += 4*size;
				for(; width < default_width; width++, y++) {
					if(vg_draw_line(x, y, x+size, y, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
		}
		// Vertical lines
		if(Vmask) {
			if(BIT0(Vmask)) {
				for(; width < default_width; width++, x++) {
					if(vg_draw_line(x, y, x, y+size, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT1(Vmask)) {
				x += 2*size;
				for(; width < default_width; width++, x++) {
					if(vg_draw_line(x, y, x, y+size, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT2(Vmask)) {
				y += size;
				for(; width < default_width; width++, x++) {
					if(vg_draw_line(x, y, x, y+size, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT3(Vmask)) {
				x += 2*size;
				y += size;
				for(; width < default_width; width++, x++) {
					if(vg_draw_line(x, y, x, y+size, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT4(Vmask)) {
				y += 2*size;
				for(; width < default_width; width++, x++) {
					if(vg_draw_line(x, y, x, y+size, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT5(Vmask)) {
				x += 2*size;
				y += 2*size;
				for(; width < default_width; width++, x++) {
					if(vg_draw_line(x, y, x, y+size, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT6(Vmask)) {
				y += 3*size;
				for(; width < default_width; width++, x++) {
					if(vg_draw_line(x, y, x, y+size, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT7(Vmask)) {
				x += 2*size;
				y += 3*size;
				for(; width < default_width; width++, x++) {
					if(vg_draw_line(x, y, x, y+size, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT8(Vmask)) {
				x += size;
				for(; width < default_width; width++, x++) {
					if(vg_draw_line(x, y, x, y+2*size, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT9(Vmask)) {
				x += size;
				y += 2*size;
				for(; width < default_width; width++, x++) {
					if(vg_draw_line(x, y, x, y+2*size, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
		}
		// Diagonal lines
		if(Dmask) {
			if(BIT0(Dmask)) {
				y += 2*size;
				x += default_width;
				for(; width < default_width; width++, y++) {
					if(vg_draw_line(x, y, x+size, y-size, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT1(Dmask)) {
				y += 4*size;
				for(; width < default_width; width++, y++) {
					if(vg_draw_line(x, y, x+2*size, y-2*size, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
			if(BIT2(Dmask)) {
				y += 2*size;
				for(; width < default_width; width++, y++) {
					if(vg_draw_line(x, y, x+2*size, y+2*size, color) == -1) {
						return -1;
					}
				}
				width = 0;
				y = yi;
				x = xi;
			}
		}
		doTimes--;
		xi += 2*size;
		x += 2*size;
	}
	if(duplicate)
		return (5*size/2);
	else return (size/2);
}

int vg_draw_line(unsigned int xi, unsigned int yi, unsigned int xf, unsigned int yf, unsigned int color) {

	unsigned long i;
	long double deltax;
	long double deltay;
	long double m;
	long double b;

	/* Verifications */
	if((yi >= v_res) || (yf >= v_res) || (xf >= h_res) || (xi >= h_res)) {
		return -1;
	}

	/* Verifications END*/

	deltax = (long double)(xf) - (long double)(xi);
	deltay = (long double)(yf) - (long double)(yi);
	m = deltay/deltax;
	b = (long double)(yi) - m*xi;

	if(xi == xf) { /* draw vertical line */
		if(yf < yi) {
			swap(&xi,&xf);
			swap(&yi,&yf);
		}
		for(i = yi; i <= yf; i++) {
			vg_set_pixel(xi,i,color);
		}
	}
	else if(yi == yf) { /* draw horizontal line */
		if(xf < xi) {
			swap(&xi,&xf);
			swap(&yi,&yf);
		}
		for(i = xi; i <= xf ; i++) {
			vg_set_pixel(i,yi,color);
		}
	}
	//***********************************************
	//   		USED ONLY FOR CHARACTERS
	//***********************************************
	else if(m == 1) {
		if(xf < xi) {
			swap(&xi,&xf);
			swap(&yi,&yf);
		}
		for(; xi < xf; xi++, yi++) {
			vg_set_pixel(xi,yi,color);
		}
	}
	else if(m == -1) {
		if(xf < xi) {
			swap(&xi,&xf);
			swap(&yi,&yf);
		}
		for(; xi <= xf; xi++, yi--) {
			vg_set_pixel(xi,yi,color);
		}
	}
	//***********************************************
	else if(fabs(m) < 1) {
		if(xf < xi) {
					swap(&xi,&xf);
					swap(&yi,&yf);
				}
		vg_set_pixel(xi,yi,color);
		while(xi <= xf) {
			if(m*xi + b > yi + 0.5) {
				if(m > 0)
					yi++;
				else
					yi--;
			}
			xi++;
			vg_set_pixel(xi,yi,color);
		}
	}
	//***********************************************
	else if(fabs(m) > 1) {
		if(yf < yi) {
				swap(&xi,&xf);
				swap(&yi,&yf);
			}
		m = 1.0/m;
		vg_set_pixel(xi,yi,color);
		while(yi <= yf) {
			if(((yi - b)*m) < (xi + 0.5))
				if(m > 0)
					xi++;
				else
					xi--;
			yi++;
			vg_set_pixel(xi,yi,color);
		}
	}
	//***********************************************

	return 0;
}

int vg_draw_square(unsigned int xi, unsigned int yi, unsigned int size, unsigned int color) {

	if(!size) {
		return -2;
	}

	// horizontal
	if (vg_draw_line( xi, yi, (xi + size), yi, color ) == -1) {
		return -1;
	}
	if (vg_draw_line( xi, (yi + size), (xi + size), (yi + size), color ) == -1) {
		return -1;
	}
	// vertical
	if (vg_draw_line( xi, yi, xi, (yi + size), color ) == -1) {
		return -1;
	}
	if (vg_draw_line( (xi + size), yi, (xi + size), (yi + size), color ) == -1) {
		return -1;
	}

	return 0;
}

int vg_draw_rectangle(unsigned int xi, unsigned int yi, unsigned int xf, unsigned int yf, unsigned int color) {

	/* horizontal */
	if (vg_draw_line( xi, yi, xf, yi, color ) == -1) {
		return -1;
	}
	if (vg_draw_line( xi, yf, xf, yf, color ) == -1) {
		return -1;
	}
	/* vertical */
	if (vg_draw_line( xi, yi, xi, yf, color ) == -1) {
		return -1;
	}
	if (vg_draw_line( xf, yi, xf, yf, color ) == -1) {
		return -1;
	}

	return 0;
}

int vg_draw_death_square(unsigned int xi, unsigned int yi, unsigned int size, unsigned int color[]) {

	if(!size) {
		return -2;
	}

	/* horizontal */
	// upper left line
	if (vg_draw_line( xi, yi, (xi + (size/2)), yi, color[0]) == -1) {
		return -1;
	}
	// upper right line
	if (vg_draw_line( xi + (size/2), yi, (xi + size), yi, color[1]) == -1) {
		return -1;
	}

	// lower left line
	if (vg_draw_line( xi, (yi + size), (xi + (size/2)), (yi + size), color[5]) == -1) {
		return -1;
	}
	// lower right line
	if (vg_draw_line( xi + (size/2), (yi + size), (xi + size), (yi + size), color[4]) == -1) {
		return -1;
	}

	/* vertical */
	// upper left line
	if (vg_draw_line( xi, yi, xi, (yi + (size/2)), color[7] ) == -1) {
		return -1;
	}
	// lower left line
	if (vg_draw_line( xi, yi + (size/2), xi, (yi + size), color[6] ) == -1) {
		return -1;
	}

	// upper right line
	if (vg_draw_line( (xi + size), yi, (xi + size), (yi + (size/2)), color[2] ) == -1) {
		return -1;
	}
	// lower right line
	if (vg_draw_line( (xi + size), yi + (size/2), (xi + size), (yi + size), color[3] ) == -1) {
		return -1;
	}

	return 0;

}

int vg_fill_section(unsigned int xi, unsigned int yi, unsigned int xf, unsigned int yf, unsigned int color) {
	unsigned long col;
	unsigned int x = xi;
	unsigned int y = yi;

	if((yf >= v_res) || (xf >= h_res)) {
		return -1;
	}

	for (; y < yf; y++) {
		for (; x < xf; x++) {
			vg_set_pixel(x,y,color);
		}
		x = xi;
	}

	return 0;
}
