objects = analyzer.o simulator.o turing.o

# vpath $.cpp ./src
# vpath $.h ./include
VPATH = src:include

turing : $(objects)
	g++ $(objects) -o turing

turing.o : analyzer.h simulator.h
analyzer.o : analyzer.h
simulator.o : simulator.h

.PHONY : clean
clean :
	-rm turing $(objects)