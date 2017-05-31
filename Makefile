########
#
# Makefile for s3mod
# Copyright David Jeske (jeske@uiuc.edu) and Daniel Marks (dmarks@uiuc.edu)
#
########

CC	= gcc 
RM	= rm
LN	= ln -s

CFLAGS  = -O2
LFLAGS  = 

OBJS	= mod.o s3m.o main.o play.o mix.o cmdline.o dsp.o gus.o
CFILES	= mod.c s3m.c main.c play.c gus.c mix.c cmdline.c dsp.c

.c.o:
	$(CC) -c $(CFLAGS) $(DEFINES) $(INCLUDES) $<

all: 
	@echo "           **** S3MOD v1.09 ****"
	@echo "You must choose one of: "
	@echo "     make sun              - SunOS/Solaris"
	@echo "     make sgi              - SGI Irix"
	@echo "     make linux            - Voxware DSP/GUS"
	@echo "     make dec              - Dec OSF/1"
	@echo "     make clean            - remove all .o files"
	@echo "     make clobber          - remove all .o and targets"
	@echo ""

sgi:
	$(RM) -f dsp.c
	$(LN) DspDrivers/sgi.c dsp.c
	make CC=cc CFLAGS=-O2 LIBS="-laudio -lm" s3mod

sun:
	$(RM) -f dsp.c
	$(LN) DspDrivers/sun.c dsp.c
	make CC=gcc CFLAGS=-O2 DEFINES=-DSUN \
	   INCLUDES="-I/usr/demo/SOUND/include" \
	   LIBS="-L/usr/demo/SOUND/lib -laudio -lm" s3mod

dec:
	$(RM) -f dsp.c
	$(LN) DspDrivers/dec.c dsp.c
	make CC=gcc CFLAGS=-O2 DEFINES=-DDEC" s3mod

linux:
	$(RM) -f dsp.c
	$(LN) DspDrivers/linux_dsp.c dsp.c
	make CC=gcc CFLAGS=-O2 DEFINES="-DLINUX -DGUS" s3mod

s3mod:	$(OBJS)
	$(CC) $(CFLAGS) -o s3mod $(OBJS) $(LFLAGS) $(LIBS)

clean:
	-$(RM) -f *~ *.bak *.o dsp.c

clobber: clean
	-$(RM) -f s3mod

dep:
	makedepend $(CFLAGS) $(LFLAGS) $(LIBS) $(CFILES)

# DO NOT DELETE THIS LINE -- make depend depends on it.

dsp.c:
	echo "You need to link the dsp driver file for your system onto dsp.c"
	echo

mod.o: mod.h gus.h main.h
s3m.o: mod.h gus.h
main.o: mod.h play.h
main.o: main.h gus.h cmdline.h
play.o: mod.h play.h gus.h main.h
gus.o: gus.h mod.h play.h tables.h
mix.o: mod.h gus.h
cmdline.o: cmdline.h
dsp.o: dsp.h
