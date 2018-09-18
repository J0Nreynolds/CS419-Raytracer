CC := g++-8 -std=c++11# This is the main compiler

SRCDIR := ../../src/Tracers
BUILDDIR := ../../build

SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall
INC := -I . -I ../World -I ../Utilities -I ../GeometricObjects -I ../Graphics -I \
	../GeometricObjects/Primitives -I ../Samplers -I ../Cameras \
	-I ../OpenCL/CLObjects -I /Library/Frameworks/SDL2.framework/Headers

all: $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo "$(RM) $(OBJECTS)"; $(RM) $(OBJECTS)

.PHONY: clean all
