CC := g++ -std=c++17# This is the main compiler
C := cc -std=c17


SRCDIR := ../../src/Utilities
BUILDDIR := ../../build

SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -fpermissive
INC := -I .

# Include OpenCL headers from NVIDIA GPU Computing Toolkit on Windows
ifdef OS
INC := $(INC) -I C:/Program\ Files/NVIDIA\ GPU\ Computing\ Toolkit/CUDA/v10.0/include
endif

all: $(OBJECTS) $(BUILDDIR)/ply.o
# all: $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
#
$(BUILDDIR)/ply.o: $(SRCDIR)/ply.c
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) -I . -c -o $@ $<"
	$(CC) $(CFLAGS) -I . -c -o $@ $<

clean:
	@echo " Cleaning..."
	@echo "$(RM) $(OBJECTS)"
	$(RM) $(OBJECTS)

.PHONY: clean all
