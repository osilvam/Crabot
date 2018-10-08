CC=g++
CFLAGS=-c -Wall 
VAR= -O2 -larmadillo
LDFLAGS= 
SOURCES= main.cpp tubos.cpp comunicacion_servidor.cpp sensores.cpp sensores_motores.cpp manejador_robot.cpp camara.cpp cm700.cpp serial.cpp cinematica.cpp robot.cpp leg.cpp
OBJECTS=$(SOURCES:.cpp=.o) 
EXECUTABLE= robo
PUERTO= 51800
DIRECCION= aragorn.elo.utfsm.cl


all: $(SOURCES) $(EXECUTABLE) 
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(VAR) -L. -ldxl

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: 
	./$(EXECUTABLE) $(DIRECCION) $(PUERTO)

servidor: 
	./$(EXECUTABLE) $(filter-out $@,$(MAKECMDGOALS))

local:
	./$(EXECUTABLE) 127.0.0.1 51800
