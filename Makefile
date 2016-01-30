CC = gcc
CCFLAGS = -std=c99 -g -Wall -Wextra
EXEC = mtutar
OBJS = mtutar.o

${EXEC}: ${OBJS}
	${CC} ${CCFLAGS} -o ${EXEC} ${OBJS}

.c.o:
	${CC} ${CCFLAGS} -c $<

convert.o: mtutar.c

run: ${EXEC}
	./${EXEC}

clean:
	rm -f ${EXEC} ${OBJS}
