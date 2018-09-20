CC := g++-8 -std=c++11 -faligned-new# This is the main compiler

SRCDIR := ../src
BUILDDIR := ../build
SUBDIRS := $(wildcard */.)


SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall
INC :=  -I World -I Utilities -I Tracers -I GeometricObjects -I Samplers \
	-I GeometricObjects/Primitives -I /Library/Frameworks/SDL2.framework/Headers \
	-I . -I Graphics -I Cameras -I OpenCL/CLObjects -I Lights

all: $(SUBDIRS) $(OBJECTS)

$(SUBDIRS):
	$(MAKE) -C $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR)"; $(RM) -r $(BUILDDIR)

.PHONY: clean all $(SUBDIRS)
