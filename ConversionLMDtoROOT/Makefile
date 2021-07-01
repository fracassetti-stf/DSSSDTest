# Where to install
BINDIR= /usr/bin

# compilation and link flags

ROOTCONFIG := root-config
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)

CPPFLAGS += -g $(ROOTCFLAGS)
LDFLAGS  += $(ROOTLIBS) $(ROOTGLIBS)
#LIBS += $(ROOTLIBS) $(ROOTGLIBS)
DEPS=conversion.hh RootLinkDef.h

.PHONY: all

all : Convert_Front

Convert_Front : Convert_Front.o
	g++ -Wall $(CPPFLAGS) $(LDFLAGS) -o Convert_Front Convert_Front.o

Convert_Front.o : Convert_Front.cc $(DEPS)
	g++ -Wall $(CPPFLAGS) $(LDFLAGS) -c Convert_Front.cc

clean:
	rm Convert_Front.o Convert_Front
