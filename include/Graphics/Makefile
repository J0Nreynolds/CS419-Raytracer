SHELL := bash
CC := g++ -std=c++11# This is the main compiler

SRCDIR := ../../src/Graphics
BUILDDIR := ../../build

SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -lpng
INC := -I . -I ../Utilities -I ../World -I ../GeometricObjects -I ../Tracers \
	-I ../GeometricObjects/Primitives -I ../Samplers  -I ../Cameras -I ../Lights \
	-I ../OpenCL/CLObjects -I ../OpenCL/CLMaterials -I ../OpenCL/CLBRDFs -I ../Materials \
	 -I /Library/Frameworks/SDL2.framework/Headers

 # Include OpenCL headers from NVIDIA GPU Computing Toolkit on Windows
 ifdef OS
 INC := $(INC) -I C:/Program\ Files/NVIDIA\ GPU\ Computing\ Toolkit/CUDA/v10.0/include -I C:/cygwin/usr/include/SDL2 -I C:/Program\ Files\ \(x86\)/GnuWin32/include
 endif

all: $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"
	$(CC) $(CFLAGS) $(INC) -c -o $@ $< -lPNG

clean:
	@echo " Cleaning..."
	@echo "$(RM) $(OBJECTS)"
	$(RM) $(OBJECTS)

.PHONY: clean all
