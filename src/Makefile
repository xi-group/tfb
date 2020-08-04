# Data obtained from the shell/OS
OS	:=	$(shell uname)

CC	=	cc
CFLAGS	=	-Wall -Werror -pedantic
INC	=	-I.
LDINC	=	-L.
LDFLAGS =	

# Debug (make DEBUG=yes $target)
ifeq ($(DEBUG), yes)
	CFLAGS	+=	-g
endif

# OS-specific
ifeq ("${OS}", "Linux")
	CFLAGS	+=	-D_GNU_SOURCE
endif

PROG	=	tfb

SRC	=	tfb.c
OBJ	=	tfb.o

all:
	$(CC) $(CFLAGS) -c $(SRC) $(INC)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJ) $(LDINC) $(LDFLAGS)

cppcheck:
	@cppcheck --enable=all *.c *.h

static:
	$(CC) $(CFLAGS) -c $(SRC) $(INC)
	$(CC) $(CFLAGS) -static -o $(PROG).static $(OBJ) $(LDFLAGS)

clean:
	rm -rf $(PROG) *.o *.core *.static
