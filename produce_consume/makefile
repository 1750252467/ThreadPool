SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c, %, ${SRCS})

OUT: ${OBJS}
	
%: %.c
	gcc $^ -o $@ -lpthread

.PHONY: OUT clean rebuild
clean:
	${RM} ${OBJS}
rebuild: clean OUT
