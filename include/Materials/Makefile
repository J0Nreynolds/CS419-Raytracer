SHELL := bash
CC := g++ -std=c++11# This is the main compiler

SRCDIR := ../../src/Materials
BUILDDIR := ../../build

SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall
INC := -I . -I ../Utilities -I ../BRDFs -I ../Samplers -I ../World -I ../Graphics \
-I ../Tracers -I ../Lights -I ../OpenCL -I ../OpenCL/CLObjects -I ../GeometricObjects \
-I ../GeometricObjects/Primitives -I ../Cameras  -I ../OpenCL/CLMaterials -I ../OpenCL/CLBRDFs

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
