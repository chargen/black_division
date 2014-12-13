/*
 * mouse.c
 *
 * Created 28/11/2013
 * Author: ei12054
 */

#include "mouse.h"
#include "i8042.h"
#include "game.h"

int mouse_subscribe_int(unsigned int mouse_id) {

	unsigned int hook_id = mouse_id;

	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK) {
		return -1;

	}

	return 0;
}

int mouse_unsubscribe_int(unsigned int mouse_id) {

	unsigned int hook_id = mouse_id;

	if (!sys_irqrmpolicy(&hook_id)) {
		return -1;

	}

	return 0;
}

int in_buf_empty() {

	unsigned long data;

	while(1) {

		sys_inb(STAT_REG, &data);

		if ((data & IBF) == 0) {
			return 1;
		}

		tickdelay(micros_to_ticks(DELAY_US));

	}

	return 0;
}

int clean_out_buf() {

	unsigned long data;

	while(1) {

		sys_inb(STAT_REG, &data);

		if ((data & OBF) == 0) {
			break;
		}
		else {
			sys_inb(OUT_BUF, &data);
		}

		tickdelay(micros_to_ticks(DELAY_US));
	}

	return 0;
}

int send_cmd_to_mouse(unsigned char cmd) {

	unsigned long status;
	int attempts = 10;

	do{

		if(in_buf_empty()) {
			if(sys_outb(KBC_CMD_REG, WRITE_MOUSE_BYTE) != OK) {
				return -1;
			}
		}

		if(in_buf_empty()) {
			if(sys_outb(IN_BUF, cmd) != OK) {
				return -1;
			}
		}

		tickdelay(micros_to_ticks(DELAY_US));


		if (sys_inb(OUT_BUF, &status) != OK) {
			return -1;
		}

		attempts--;

	} while((status != MOUSE_STATUS_OK) && (attempts > 0));

	return 0;
}

int mouse_receive_packet() {

	unsigned long data;

	sys_inb(OUT_BUF, &data);

	return data;
}

