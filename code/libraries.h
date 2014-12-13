#ifndef LIBRARIES_H_
#define LIBRARIES_H_

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include <machine/int86.h>
#include <minix/com.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define DELAY_US    20000	/**< Delay, for tickdelay. */

/**
 * BIT reading.
 *
 * Macros to extract a given bit from a binary number.
 *
 */
#define BIT(n) 		(0x01<<(n))

#define BIT0(n)		(n & 0x01)
#define BIT1(n)		(n & 0x02)
#define BIT2(n)		(n & 0x04)
#define BIT3(n)		(n & 0x08)
#define BIT4(n)		(n & 0x10)
#define BIT5(n)		(n & 0x20)
#define BIT6(n)		(n & 0x40)
#define BIT7(n)		(n & 0x80)
#define BIT8(n)		(n & 0x100)
#define BIT9(n)		(n & 0x200)

#endif /* LIBRARIES_H_ */
