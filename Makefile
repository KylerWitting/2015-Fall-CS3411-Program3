CC       = gcc

OBJ_FILE = encoder.o decoder.o
EXE_FILE = encoder decoder

all: encoder decoder

encoder: encoder.o
	${CC} -o encoder encoder.o

decoder: decoder.o
	${CC} -o decoder decoder.o

encoder.o: encoder.c
	${CC} -c encoder.c
					
decoder.o: decoder.c
	${CC} -c decoder.c
	
clean:
	rm -f ${OBJ_FILE} ${EXE_FILE}
