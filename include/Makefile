CC := g++ -std=c++11 # This is the main compiler

SRCDIR := ../src
BUILDDIR := ../build
SUBDIRS := $(filter-out ./,$(sort $(dir $(wildcard ./*/))))


SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall
INC :=  -I World -I Utilities -I Tracers -I GeometricObjects -I Samplers \
	-I GeometricObjects/Primitives -I /Library/Frameworks/SDL2.framework/Headers \
	-I . -I Graphics -I Cameras -I OpenCL/CLObjects -I OpenCL/CLMaterials \
	-I OpenCL/CLBRDFs -I Materials -I Lights -I /System/Library/Frameworks/OpenCL.framework/Headers

MAGICK := `Magick++-config --cppflags`

# Include OpenCL headers from NVIDIA GPU Computing Toolkit on Windows
ifdef OS
INC := $(INC) -I C:/Program\ Files/NVIDIA\ GPU\ Computing\ Toolkit/CUDA/v10.0/include -I C:/cygwin/usr/include/SDL2
endif

all: $(SUBDIRS) $(OBJECTS)

$(SUBDIRS):
	$(MAKE) -C $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $< $(MAGICK)"
	$(CC) $(CFLAGS) $(INC) -c -o $@ $< $(MAGICK)

clean:
	@echo " Cleaning..."
	@echo " $(RM) -r $(BUILDDIR)"
	$(RM) -r $(BUILDDIR)

.PHONY: clean all $(SUBDIRS)
