CC := g++-8 -std=c++11 -g # This is the main compiler

BUILDDIR := build
TARGET = bin/runner.exe
INCLUDE = include
LIB := -framework OpenCL -framework SDL2 -lpng

SRCDIR := src
SRCEXT := cpp
OBJEXT := o

SOURCES = $(shell find src/ -type f -name '*.cpp')
OBJECTS = $(shell find build/ -type f -name '*.o')

all: subdirs $(TARGET)

subdirs:
	$(MAKE) -C $(INCLUDE)

# Update TARGET if
$(TARGET): $(OBJECTS) # 1. Target doesn't exist  2. Objects have changed
	@echo " Linking..."
	@echo " $(CC) $(shell find build/ -type f -name '*.o') -o $(TARGET) $(LIB)"; $(CC) $(shell find build/ -type f -name '*.o') -o $(TARGET) $(LIB)

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean all subdirs
