CXX 		:= g++
CXX_FLAGS 	:= -Wall -g -std=c++0x
SRCS		:= process_test.cpp Process.cpp
OBJS 		:= $(SRCS:.cpp=.o)

all: process_test child

process_test: ${OBJS}
	${CXX} $(CXX_FLAGS) -o $@ ${OBJS}

child: child.o
	${CXX} $(CXX_FLAGS) -o $@ child.o

%.o : %.cpp
	$(CXX) $(CXX_FLAGS) -MD -o $@ -c $< 
	@cp $*.d $*.P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	 	  -e '/^$$/ d' -e 's/$$/ :/' < $*.P >> $*.d; \
	  rm -f $*.P

clean:
	-rm *.o *.d *~
