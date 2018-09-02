CC := g++-8 -std=c++11# This is the main compiler

SRCDIR := ../src
BUILDDIR := ../build
SUBDIRS := $(wildcard */.)


SRCEXT := cpp
SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -framework OpenCL
INC := -I . -I Utilities -I GeometricObjects -I GeometricObjects/Primitives

all: $(SUBDIRS) $(OBJECTS)

$(SUBDIRS):
	$(MAKE) -C $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR)"; $(RM) -r $(BUILDDIR)

.PHONY: clean all $(SUBDIRS)
