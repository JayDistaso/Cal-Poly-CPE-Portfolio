# Makefile for the secret driver. Adapted from hello Makefile
PROG=	secret
SRCS=	secret.c

DPADD+=	${LIBCHARDRIVER} ${LIBSYS}
LDADD+=	-lchardriver -lsys

MAN=

BINDIR?= /usr/sbin

.include <minix.service.mk>
