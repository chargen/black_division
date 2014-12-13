/*
 * vbe.c
 *
 * Created 28/11/2013
 * Author: ei12054
 */

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define MODE_INFO_BLOCK_SIZE 256

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	mmap_t map_address;
	struct reg86u reg86;

	/* Initialized low memory area */
	lm_init();

	/* Allocates a memory block in low memory area */
	if(lm_alloc(sizeof(vbe_mode_info_t), &map_address) == NULL) {
		return -1;
	}

	reg86.u.w.ax = 0x4F01; // VBE get mode info
	reg86.u.w.es = PB2BASE(map_address.phys);
	reg86.u.w.di = PB2OFF(map_address.phys);
	reg86.u.w.cx = mode;
	reg86.u.b.intno = 0x10;

	sys_int86(&reg86);

	memcpy(vmi_p, map_address.virtual, MODE_INFO_BLOCK_SIZE);
	lm_free(&map_address);

  return 0;
}

