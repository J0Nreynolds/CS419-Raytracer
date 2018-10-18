CC := g++ -std=c++11 -faligned-new# This is the main compiler

SRCDIR := ../../src/World
BUILDDIR := ../../build

SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall
INC := -I . -I ../Utilities -I ../Tracers -I ../Graphics -I ../GeometricObjects \
	-I ../GeometricObjects/Primitives -I ../Samplers -I ../Cameras -I ../Lights \
	-I ../OpenCL -I ../OpenCL/CLObjects  -I ../Materials  -I ../BRDFs \
	-I ../OpenCL/CLMaterials -I ../OpenCL/CLBRDFs \
	-I ../GeometricObjects/CompoundObjects -I /Library/Frameworks/SDL2.framework/Headers

all: $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."
	@echo "$(RM) $(OBJECTS)"
	$(RM) $(OBJECTS)

.PHONY: clean all
