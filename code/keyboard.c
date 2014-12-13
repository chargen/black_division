/*
 * keyboard.c
 *
 * Created 28/11/2013
 * Author: ei12054
 */

#include "keyboard.h"
#include "i8042.h"

int keyboard_set_leds(unsigned short int number) {

	unsigned long stat;
	unsigned long led = 0x00;

	switch(number) {
	case 0:
		led = 0x00;
		break;
	case 1:
		led = SCROLL_LCK;
		break;
	case 2:
		led = NUM_LCK | SCROLL_LCK;
		break;
	case 3:
		led = CAPS_LCK | NUM_LCK | SCROLL_LCK;
		break;
	default:
		return -1;
	}

	while(1){

		sys_inb(STAT_REG, &stat);

		if( (stat & IBF) == 0){

			sys_outb(IN_BUF, KBD_SWITCH_LEDS);
			break;
		}

		tickdelay(micros_to_ticks(DELAY_US));
	}

	while(1) {

		sys_inb(STAT_REG, &stat);

		if( (stat & IBF) == 0){

			sys_outb(IN_BUF, led);
			break;

		}

		tickdelay(micros_to_ticks(DELAY_US));
	}

	return 0;
}

int keyboard_subscribe_int(const unsigned int keyboard_id) {

	unsigned int hook_id = keyboard_id;

	if(sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK) {
		//printf("keyboard sys_irqsetpolicy() failed!\n");
		return -1;
	}

	return 0;

}

int keyboard_unsubscribe_int(const unsigned int keyboard_id) {

	unsigned int hook_id = keyboard_id;

	if (!sys_irqrmpolicy(&hook_id)) {
		//printf("keyboard sys_irqrmpolicy() failed!\n");
		return -1;
	}

	return 0;
}

unsigned long keyboard_scan() {

	unsigned long stat, data = 0;
	int tobreak = 0;

	do {

		if(sys_inb(STAT_REG, &stat) == OK) {

			if (stat & OBF) {

				sys_inb(OUT_BUF, &data);

				if ( (stat &(PAR_ERR | TO_ERR)) == 0 ) tobreak = 1;
				else return -1;
			}

		}

	} while(!tobreak);

	return data;
}
