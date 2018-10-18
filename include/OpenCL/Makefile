CC := g++ -std=c++11# This is the main compiler

SRCDIR := ../../src/OpenCL
BUILDDIR := ../../build
SUBDIRS := $(wildcard */.)


SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall
INC := -I . -I ./CLObjects -I ../World -I ../Samplers -I ../Utilities \
	-I ../Graphics -I ../GeometricObjects/Primitives -I ../GeometricObjects \
	-I ../Tracers -I ../Cameras -I ../Lights -I ../Materials -I ./CLMaterials \
	-I ./CLBRDFs

all: $(SUBDIRS) $(OBJECTS)

$(SUBDIRS):
	$(MAKE) -C $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."
	@echo "$(RM) $(OBJECTS)"
	$(RM) $(OBJECTS)

.PHONY: clean all $(SUBDIRS)
