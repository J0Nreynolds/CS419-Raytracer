CC := g++-8 -std=c++11# This is the main compiler

BUILDDIR := build
TARGET = bin/runner.exe
INCLUDE = include
LIB := -framework OpenCL

all: subdirs $(TARGET)

geometric_objects:
	$(MAKE) -C $(INCLUDE)/GeometricObjects

materials:
	$(MAKE) -C $(INCLUDE)/Materials

tracers:
	$(MAKE) -C $(INCLUDE)/Tracers

utilities:
	$(MAKE) -C $(INCLUDE)/Utilities

subdirs:
	$(MAKE) -C $(INCLUDE)

$(TARGET):
	@echo " Linking..."
	@echo " $(CC) $(wildcard $(BUILDDIR)/*.o) -o $(TARGET) $(LIB)"; $(CC) $(wildcard $(BUILDDIR)/*.o) -o $(TARGET) $(LIB)

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean all subdirs geometric_objects materials tracers utilities
