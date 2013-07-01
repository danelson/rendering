# set the C compiler, for C++ use g++

# use this setting on most sane machines
CC = gcc

# use this setting on the engin elf machines
#CC = gcc296

# set the path to the include directory
INCDIR =../include

# set the flags for the C and C++ compiler to give lots of warnings
CFLAGS = -I$(INCDIR) -O2 -Wall -Wstrict-prototypes -Wnested-externs \
							-Wmissing-prototypes -Wmissing-declarations
CPPFLAGS = $(CFLAGS)

# library tool defs
AR = ar
ARFLAGS = rv
RANLIB = ranlib

# path to the object file directory
ODIR = obj

# path to the lib directory
LIBDIR =.

# path to the bin directory
BINDIR =../bin

# put all of the relevant include files here
_DEPS = ppmIO.h

# convert them to point to the right place
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

# put a list of all the object files (with .o endings)
_COMMON = ppmIO.o image.o perlin.o line.o circle.o ellipse.o point.o polyline.o drawstate.o \
			polygon.o scanlineSkeleton.o matrix.o vector.o view.o lighting.o module.o plyRead.o \
			ray.o ray_object.o ray_module.o
			

# convert them to point to the right place
COMMON = $(patsubst %,$(ODIR)/%,$(_COMMON))

# patterns for compiling source code
$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.C $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# output library
OUTPUT_LIB = libimageIO.a

# make rules
all: $(OUTPUT_LIB)

$(OUTPUT_LIB) : $(COMMON) $(DEPS)
	rm -f $@
	$(AR) $(ARFLAGS) $@ $(COMMON)
	$(RANLIB) $@

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

