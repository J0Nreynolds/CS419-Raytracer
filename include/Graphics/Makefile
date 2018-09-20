CC := g++-8 -std=c++11# This is the main compiler

SRCDIR := ../../src/Graphics
BUILDDIR := ../../build

SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -lpng
INC := -I . -I ../Utilities -I ../World -I ../GeometricObjects -I ../Tracers \
	-I ../GeometricObjects/Primitives -I ../Samplers  -I ../Cameras -I ../Lights \
	-I ../OpenCL/CLObjects -I /Library/Frameworks/SDL2.framework/Headers

all: $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo "$(RM) $(OBJECTS)"; $(RM) $(OBJECTS)

.PHONY: clean all
