CC := g++ -std=c++11 -g # This is the main compiler

BUILDDIR := build
TARGET = bin/runner.exe
INCLUDE = include
ifdef OS
LIB := -lOpenCL -lSDL2 -lpng
LIBL := -L C:/Program\ Files\ \(x86\)/GnuWin32/lib \
    -L C:/Program\ Files/NVIDIA\ GPU\ Computing\ Toolkit/CUDA/v10.0/lib/Win32 \
    -L C:/cygwin/lib
else
LIB := -framework OpenCL -lSDL2 -lpng -lboost_system
endif
MAGICK := `Magick++-config --cppflags --libs`

SRCDIR := src
SRCEXT := cpp
OBJEXT := o

SOURCES := $(wildcard ./src/*.cpp)
OBJECTS := $(wildcard ./build/*.o)

all: subdirs $(TARGET)

subdirs:
	$(MAKE) -C $(INCLUDE)

# Update TARGET if
$(TARGET): $(OBJECTS) # 1. Target doesn't exist  2. Objects have changed
	@echo " Linking..."
	@echo " $(CC) $(OBJECTS) -o $(TARGET) $(LIBL) $(LIB) $(MAGICK)"; $(CC) $(OBJECTS) -o $(TARGET) $(LIBL) $(LIB) $(MAGICK)

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean all subdirs
