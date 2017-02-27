# New Makefile that automatically depends itself
#
# $Id: Makefile,v 1.3 1996/12/17 19:52:37 chase Exp $
#

IFLAGS = 
DFLAGS = 
CXX = g++ # --std=c++11
CC  = $(GCC)
GCC = g++ # --std=c++11
LD  = $(CXX)

LIBS = 

WFLAGS = -Wall 
SYMFLAGS = -g

PROFILE = #-pg 
OPTFLAGS =#-O
CFLAGS = $(OPTFLAGS) $(PROFILE) $(WFLAGS) $(IFLAGS) $(SYMFLAGS)
CXXFLAGS = $(CFLAGS)
CPPFLAGS = $(IFLAGS) $(DFLAGS)
LDFLAGS = $(PROFILE) -g 

PROGRAM = proj2
#CXXSRCS = Source.cpp
CXXSRCS = $(shell ls *.cpp)

SRCS = $(CXXSRCS)
OBJS = $(CXXSRCS:.cpp=.o)

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(LD) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)

test: $(PROGRAM)
	-@./$(PROGRAM) testMultiplyMatrixA
	-@./$(PROGRAM) testMultiplyMatrixB
	-@./$(PROGRAM) testMultiplyMatrixC
	-@./$(PROGRAM) testTransposeA
	-@./$(PROGRAM) testTransposeB
	-@./$(PROGRAM) testTransposeC
	-@./$(PROGRAM) testMultiplyMatrixPtrA
	-@./$(PROGRAM) testMultiplyMatrixPtrB
	-@./$(PROGRAM) testMultiplyMatrixPtrC

clean:
	-rm -f $(OBJS) $(PROGRAM)

undepend:
	-rm -f $(OBJS:%.o=.%.d) 

spotless: clean undepend

# auto depend stuff for GNU make only
depend: undepend
	@echo ""
	@echo "Dependences are handled automatically, just \"make\""

ifneq ($(strip $(CSRCS)),)
.%.d: %.c 
	$(SHELL) -ec '$(GCC) -MM $(CPPFLAGS) $< > $@'


include $(CSRCS:%.c=.%.d)
endif 

ifneq ($(strip $(CXXSRCS)),)
.%.d: %.cpp
	$(SHELL) -ec '$(GCC) -MM $(CPPFLAGS) $< > $@'

include $(CXXSRCS:%.cpp=.%.d) 
endif 
