CC=g++
CFLAGS=-c -Wall 
VAR= 
LDFLAGS= 
SOURCES= servidor_socket.cpp tubos.cpp
OBJECTS=$(SOURCES:.cpp=.o) 
EXECUTABLE=sserv

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(VAR)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: 
	./$(EXECUTABLE) 51800
