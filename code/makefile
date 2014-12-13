# Makefile for the lcom project

COMPILER_TYPE= gnu

CC=gcc

PROG=	project
SRCS=	main.c video_gr.c vbe.c timer.c speaker.c keyboard.c mouse.c rtc.c game.c devices.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS} liblm.a
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>