CC := g++ -std=c++11 # This is the main compiler

SRCDIR := ../../src/World
BUILDDIR := ../../build

SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall
INC := -I . -I ../Utilities -I ../Tracers -I ../Graphics -I ../GeometricObjects \
	-I ../GeometricObjects/Primitives -I ../Samplers -I ../Cameras -I ../Lights \
	-I ../OpenCL -I ../OpenCL/CLObjects -I ../Materials -I ../Textures -I ../BRDFs \
	-I ../OpenCL/CLMaterials -I ../OpenCL/CLBRDFs -I ../BTDFs \
	-I ../GeometricObjects/CompoundObjects -I /Library/Frameworks/SDL2.framework/Headers \
	-I /System/Library/Frameworks/OpenCL.framework/Headers -I ../Mappings

MAGICK := `Magick++-config --cppflags`

# Include OpenCL headers from NVIDIA GPU Computing Toolkit on Windows
ifdef OS
INC := $(INC) -I C:/Program\ Files/NVIDIA\ GPU\ Computing\ Toolkit/CUDA/v10.0/include -I C:/cygwin/usr/include/SDL2
endif

all: $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $< $(MAGICK)"
	$(CC) $(CFLAGS) $(INC) -c -o $@ $< $(MAGICK)

clean:
	@echo " Cleaning..."
	@echo "$(RM) $(OBJECTS)"
	$(RM) $(OBJECTS)

.PHONY: clean all
