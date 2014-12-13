/*
 * timer.c
 *
 * Created 28/11/2013
 * Author: ei12054
 */

#include "timer.h"
#include "i8254.h"

int timer_set_square(unsigned long timer, unsigned long freq) {

	unsigned char selectedTimer, controlRegister, lsb, msb;

	controlRegister = 0;

	if (timer == 0) {
		selectedTimer = TIMER_0;
		controlRegister |= TIMER_SEL0;
	}
	else if (timer == 1) {
		selectedTimer = TIMER_1;
		controlRegister |= TIMER_SEL1;
	}
	else if (timer == 2) {
		selectedTimer = TIMER_2;
		controlRegister |= TIMER_SEL2;
	}
	else
		return 1;
	freq = TIMER_FREQ/freq;
	lsb = (char) freq;
	msb = (char) (freq >> 8);

	if ( sys_outb(TIMER_CTRL, (controlRegister | TIMER_LSB_MSB | TIMER_SQR_WAVE | TIMER_BIN)) == OK &&
			sys_outb(selectedTimer, lsb) == OK && sys_outb(selectedTimer, msb) == OK ) {
		return 0;
	}

	return -1;
}

int timer_subscribe_int(const unsigned int timer0_id) {

	unsigned int hook_id = timer0_id;

	if (sys_irqsetpolicy(TIMER_IRQ, IRQ_REENABLE, &hook_id) != OK){
		//printf("timer sys_irqsetpolicy() failed!\n");
		return -1;
	}

	return 0;

}

int timer_unsubscribe_int(const unsigned int timer0_id) {

	unsigned int hook_id = timer0_id;

	if(!sys_irqrmpolicy(&hook_id)) {
		//printf("timer sys_irqrmpolicy() failed!\n");
		return -1;
	}

	return 0;
}

