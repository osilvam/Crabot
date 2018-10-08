/* */
#ifndef CM700_H
#define CM700_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "serial.h"
#include "caracteristicas_robot.h"
#include "dynamixel.h"

#include <iostream>

/* MEMADDR */
#define P_MODEL_L				0
#define P_TORQUE_ENABLE			24

#define P_GOAL_POSITION_L       30
#define P_GOAL_POSITION_H       31
#define P_MOVING_SPEED_L        32
#define P_MOVING_SPEED_H        33

#define P_PRESENT_POSITION_L    36
#define P_PRESENT_POSITION_H    37
#define P_PRESENT_SPEED_L       38
#define P_PRESENT_SPEED_H       39
#define P_PRESENT_LOAD_L        40
#define P_PRESENT_LOAD_H        41
#define P_PRESENT_VOLTAGE       42
#define P_PRESENT_TEMPERATURE   43

using namespace std;

struct actuador {
	int id;

	int cposition;
	int tposition;

	int cspeed;
	int tspeed;

	int load;

	int volt;
	int current;
	int temperature;
};

class CM700 {
	int fd;
	char *serial_name;

	uint8_t buffer_in[255];
	char buffer_out[255];

	int num_actuadores;
	int actuadores_leg;

	struct actuador *actuadores;
	struct actuador servocam;
	time_t timestamp;
public:
	
	CM700(int, int);
	~CM700(void);

	int getActPosition(int id);
	void getLegPosition(int *ids, int *dst);
	void setLegPosition(int *ids, int *tpos, int *tspeed);
	void setAllLegPosition(int *tp, int *ts);
	void refreshAll();
	void moveAll();
	void setTorque(bool enable);
	int getCamPosition();
	void setCamPosition(int tpos, int tspeed);
	void printValues();
	
	// Se obtienen los datos de los sensores listos
	void obtener_datos_en_arreglo(char buffer[TAMANO_BUFFER_COMUNICACION], int * count);

	friend class Robot;
	friend class Leg;
};
#endif
