CC := g++-8 -std=c++11# This is the main compiler

BUILDDIR := build
TARGET = bin/runner.exe
INCLUDE = include
LIB := -framework OpenCL

SRCDIR := src
SRCEXT := cpp

SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

all: subdirs $(TARGET)

subdirs:
	$(MAKE) -C $(INCLUDE)

$(TARGET): $(OBJECTS) # updates binary whenever objects have changed
	@echo " Linking..."
	@echo " $(CC) $(wildcard $(BUILDDIR)/*.o) -o $(TARGET) $(LIB)"; $(CC) $(wildcard $(BUILDDIR)/*.o) -o $(TARGET) $(LIB)

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean all subdirs geometric_objects materials tracers utilities
