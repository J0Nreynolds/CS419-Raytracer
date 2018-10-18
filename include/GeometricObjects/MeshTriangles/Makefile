CC := g++ -std=c++11# This is the main compiler

SRCDIR := ../../../src/GeometricObjects/MeshTriangles
BUILDDIR := ../../../build

SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall
INC := -I . -I .. -I ../../Utilities -I ../../Materials -I ../../OpenCL/CLMaterials \
	-I ../../OpenCL/CLBRDFs

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
