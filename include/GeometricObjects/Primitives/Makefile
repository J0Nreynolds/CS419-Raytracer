SHELL := bash
CC := g++ -std=c++11# This is the main compiler

SRCDIR := ../../../src/GeometricObjects/Primitives
BUILDDIR := ../../../build

SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall
INC := -I . -I .. -I ../../Utilities -I ../../OpenCL/CLObjects \
 	-I ../../OpenCL/CLMaterials -I ../../OpenCL/CLBRDFs -I ../../Materials \
	-I /System/Library/Frameworks/OpenCL.framework/Headers -I ../../Samplers

# Include OpenCL headers from NVIDIA GPU Computing Toolkit on Windows
ifdef OS
INC := $(INC) -I C:/Program\ Files/NVIDIA\ GPU\ Computing\ Toolkit/CUDA/v10.0/include -I C:/cygwin/usr/include/SDL2
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
