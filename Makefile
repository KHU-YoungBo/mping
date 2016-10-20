CC          = gcc

WARNINGS    += -W -Wall -Wshadow
WARNINGS    += -Wno-unused-function
WARNINGS    += -Wno-unused-parameter

CFLAGS      += -c
CFLAGS      += -std=gnu99
CFLAGS      += -g
CFLAGS      += $(INCLUDE)
CFLAGS      += $(WARNINGS)

LDFLAGS     += $(LIBS)

SRCS        = mping.c
OBJS        = $(SRCS:.c=.o)


MPING       = mping

.PHONY: all clean

all: $(MPING)

$(MPING): $(OBJS) 
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $< $(CFLAGS)

clean:
	$(RM) $(MPING) *.o

