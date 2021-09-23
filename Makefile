#####################################################################
ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)

EXE	=	Builder
CXX	=	g++ -g -w
CXXFLAGS      = -O -fbounds-check
LD	=	g++
LDFLAGS = 	-g
COPTS	=	-ggdb -fPIC -DLINUX -Wall -O0 
#COPTS	=	-g -fPIC -DLINUX -Wall
FLAGS	=	-ggdb -Wall -s -O0
#FLAGS	=	-Wall
DEPLIBS	=       -l curses
CXXFLAGS     += $(ROOTCFLAGS)
LIBS	=	$(ROOTLIBS)
GLIBS   =       $(ROOTGLIBS)
INCLUDEDIR =	-I.
OBJS	=	ReadConf.o Builder.o
INCLUDES =	INIReader.h ReadConf.h

#########################################################################

all	:	$(EXE)

clean	:
		/bin/rm -f $(OBJS) $(EXE)

$(EXE)	:	$(OBJS)
		/bin/rm -f $(EXE)
		$(LD) $(LDFLAGS) $(FLAGS) -o $(EXE) $(OBJS) $(DEPLIBS) $(LIBS) $(GLIBS)
$(OBJS)	:	$(INCLUDES) Makefile

%.o	:	%.c
		$(CXX) -c $(CXXFLAGS) $(COPTS) $(INCLUDEDIR) -c -o $@ $< #-std=c++0x

