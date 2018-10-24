SHELL := bash
CC := g++ -std=c++11# This is the main compiler

SRCDIR := ../../src/GeometricObjects
BUILDDIR := ../../build
SUBDIRS := $(filter-out ./,$(sort $(dir $(wildcard ./*/))))


SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall
INC := -I . -I ../Utilities -I ../World -I Primitives  -I ../Materials \
	-I ../OpenCL/CLMaterials -I ../OpenCL/CLBRDFs \
	-I /System/Library/Frameworks/OpenCL.framework/Headers

# Include OpenCL headers from NVIDIA GPU Computing Toolkit on Windows
ifdef OS
INC := $(INC) -I C:/Program\ Files/NVIDIA\ GPU\ Computing\ Toolkit/CUDA/v10.0/include -I C:/cygwin/usr/include/SDL2
endif

all: $(SUBDIRS) $(OBJECTS)

$(SUBDIRS):
	$(MAKE) -C $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."
	@echo "$(RM) $(OBJECTS)"
	$(RM) $(OBJECTS)

.PHONY: clean all $(SUBDIRS)
