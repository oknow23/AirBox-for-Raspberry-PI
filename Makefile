CC?=gcc
TARGET = $(notdir $(shell pwd))  

ifeq ($(TARGET_PREFIX),i586-wrs-linux-)
  LIB_PATH = lib_MI
else
  LIB_PATH = lib_64
endif

ifeq ($(CC),gcc)
	LIB_PATH = lib_ubuntu
endif

DIR_SRC = ${PWD}/src
CFLAGS=-I${PWD}/include
LDFLAGS=-L${PWD}/${LIB_PATH} -lpthread -lm
LDFLAGS_x86=-L${PWD}/lib_x86/ -lpthread -lini
OBJS := $(patsubst ${DIR_SRC}/%.c, ${DIR_SRC}/%.o, ${wildcard ${DIR_SRC}/*.c})

%.o: %.c
	${CC} -c -o $@ $< $(CFLAGS) $(LDFLAGS)

$(TARGET):$(OBJS)
	${CC} -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
