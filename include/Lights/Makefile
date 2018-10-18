CC := g++ -std=c++11# This is the main compiler

SRCDIR := ../../src/Lights
BUILDDIR := ../../build

SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall
INC := -I . -I ../Utilities -I ../OpenCL -I ../OpenCL/CLObjects -I ../World \
	-I ../Samplers -I ../Graphics -I ../Tracers -I ../GeometricObjects \
	-I ../GeometricObjects/Primitives -I ../Materials -I ../OpenCL/CLMaterials \
	-I ../OpenCL/CLBRDFs -I ../Cameras

all: $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo "$(RM) $(OBJECTS)"; $(RM) $(OBJECTS)

.PHONY: clean all
