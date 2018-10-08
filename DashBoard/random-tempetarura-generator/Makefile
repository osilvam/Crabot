CPP     = g++ --std=c99
CPPFLAGS = -Wall

all: random-temp

random-temp: random-temp.o
	$(CPP) $(CPPFLAGS) -o $@ $^

random-temp.o: random-temp.cpp

clean:
	rm -f random-temp.o

clobber: clean
	rm -f random-temp.o

.PHONY: all clean clobber
