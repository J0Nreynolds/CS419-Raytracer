SHELL := bash
CC := g++ -std=c++11# This is the main compiler

SRCDIR := ../../src/Cameras
BUILDDIR := ../../build

SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall
INC := -I . -I ../Utilities -I ../World -I ../Samplers -I ../Graphics -I \
	../GeometricObjects/Primitives -I ../GeometricObjects -I ../Tracers -I ../Lights \
	-I ../OpenCL/CLObjects -I ../OpenCL -I ../Materials -I ../OpenCL/CLMaterials \
	-I ../OpenCL/CLBRDFs -I /Library/Frameworks/SDL2.framework/Headers

# Include OpenCL headers from NVIDIA GPU Computing Toolkit on Windows
ifdef OS
INC := $(INC) -I C:/Program\ Files/NVIDIA\ GPU\ Computing\ Toolkit/CUDA/v10.0/include
endif

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
