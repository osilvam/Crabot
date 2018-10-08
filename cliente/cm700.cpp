#ifndef CM700_CPP
#define CM700_CPP

#include "cm700.h"

#define CAM_ID 25

//#define _DEBUG

/* Numero de motores totales, numero de motores por pata */
CM700::CM700(int num_actuadores, int grado_libertad) {
	this->num_actuadores = num_actuadores;
	this->actuadores_leg = grado_libertad;

	actuadores = (struct actuador *)
		malloc(num_actuadores * sizeof(struct actuador));

	memset(buffer_in, 0, sizeof(uint8_t));
	memset(buffer_out, 0, sizeof(char));

	for (int i = 0; i < num_actuadores; i++) {
		actuadores[i].id = i + 1;
		actuadores[i].cposition = 0;
		actuadores[i].tposition = 0;
		actuadores[i].cspeed = 0;
		actuadores[i].tspeed = 0;
		actuadores[i].load = 0;
		actuadores[i].volt = 0;
		actuadores[i].temperature = 0;
		actuadores[i].current = 0;
	}

	servocam.id = CAM_ID;
	servocam.cposition = 0;
	servocam.tposition = 0;
	servocam.cspeed = 0;
	servocam.tspeed = 0;
	servocam.load = 0;
	servocam.volt = 0;
	servocam.temperature = 0;
	servocam.current = 0;

#ifdef USE_USB2DXL
	int devIndex = 0; /* ttyUSB0 */
	int baudRate = 1; /* 1 Mbps */

	if (dxl_initialize(devIndex, baudRate) == 0) {
		cout << "Failed to open USB2Dynamixel!" << endl;
		cout << "Press Enter key to terminate..." << endl;
		cin.get();
		return;
	} else {
		cout << "USB2Dynamixel inicializado!" << endl;
	}
#else
	serial_name = strdup(PUERTO_SERIAL);
	#ifdef _MX
	fd = serial_open(serial_name, B4000000);
	#else
	fd = serial_open(serial_name, B1000000);
	#endif
#endif

	timestamp = 0;
}

CM700::~CM700(void) {
	free(actuadores);
	free(serial_name);
	serial_close(fd);
}

/* Obtener la posicion de un motor en especifico */
int CM700::getActPosition(int id)
{
	for (int j = 0; j < num_actuadores; j++) {
		if (actuadores[j].id == id) {
			return actuadores[j].cposition;
		}
	}
	return -1;
}

/* Obtener la posicion de los motores de una pata */
void CM700::getLegPosition(int *ids, int *dst)
{
	for (int q = 0; q < actuadores_leg; q++) {
		dst[q] = actuadores[ids[q] - 1].cposition;
	}

	/*
	for (int i = 0; i < num_actuadores; i++) {
		if (actuadores[i].id == ids[0]) {
			for (int j = 0; j < actuadores_leg; j++) {
				dst[j] = actuadores[i + j].cposition;
			}
			break;
		}
	}
	*/
}

/* Setear posicion y velocidad de una pata */
void CM700::setLegPosition(int *ids, int *tp, int *ts)
{
	for (int q = 0; q < actuadores_leg; q++) {
		actuadores[ids[q] - 1].tposition = tp[q];
		actuadores[ids[q] - 1].tspeed = ts[q];
	}

	/*
	for (int i = 0; i < num_actuadores; i++) {
		if (actuadores[i].id == ids[0]) {
			for (int j = 0; j < actuadores_leg; j++) {
				actuadores[i + j].tposition = tp[j];
				actuadores[i + j].tspeed = ts[j];
			}
			break;
		}
	}
	*/
}

/* Setear posicion y velocidad de todas las patas */
void CM700::setAllLegPosition(int *tp, int *ts)
{
	for (int i = 0; i < num_actuadores; i++) {
		actuadores[i].tposition = tp[i];
		actuadores[i].tspeed = ts[i];
	}
}

void CM700::refreshAll()
{
#ifdef USE_USB2DXL
	for (int i = 0; i < num_actuadores; i++) {
		dxl_set_txpacket_id(actuadores[i].id);
		dxl_set_txpacket_length(4);
		dxl_set_txpacket_instruction(INST_READ); 
		dxl_set_txpacket_parameter(0, P_PRESENT_POSITION_L); 
		dxl_set_txpacket_parameter(1, 11);
		dxl_txrx_packet();

		if (dxl_get_result() != COMM_RXSUCCESS) {
			continue;
		}
	 
		actuadores[i].cposition = _MW(dxl_get_rxpacket_parameter(0), dxl_get_rxpacket_parameter(1));
		actuadores[i].cspeed = _MW(dxl_get_rxpacket_parameter(2), dxl_get_rxpacket_parameter(3)) & 0x3FF;
		actuadores[i].load = _MW(dxl_get_rxpacket_parameter(4), dxl_get_rxpacket_parameter(5));
		actuadores[i].volt = dxl_get_rxpacket_parameter(6);
		actuadores[i].temperature = dxl_get_rxpacket_parameter(7);
		// Modificar para MX !!!!!111uno
		actuadores[i].current = 0;
//		actuadores[i].is_moving = dxl_get_rxpacket_parameter(10) == 1 ? true : false;
	}
#else
	int valid_actuators = 0;

	buffer_out[0] = 0xFF;
	buffer_out[1] = ASKPOSNDVEL;
	buffer_out[2] = num_actuadores;

	serial_io_flush(fd);
	serial_send(fd, buffer_out, 3);
	serial_read(fd, buffer_in, 3, DEF_TIMEOUT);

	if (buffer_in[0] != 0xFF || buffer_in[1] != RPYPOSNVEL) {
		serial_io_flush(fd);
		return;
	}

	valid_actuators = buffer_in[2];

	serial_read(fd, buffer_in, 9 * valid_actuators, DEF_TIMEOUT);
	serial_io_flush(fd);
/*
	if (buffer_in[0] != num_actuadores)
		return;
*/
	for (int i = 0; i < valid_actuators; i++) {
		for (int j = 0; j < num_actuadores; j++) {
			if (actuadores[j].id == buffer_in[0 + 9 * i]) {
				actuadores[j].cposition = _MW(buffer_in[1 + 9 * i], buffer_in[2 + 9 * i]);
				actuadores[j].cspeed = _MW(buffer_in[3 + 9 * i], buffer_in[4 + 9 * i]) & 0x3FF;
				actuadores[j].load = _MW(buffer_in[5 + 9 * i], buffer_in[6 + 9 * i]);
				actuadores[j].volt = buffer_in[7 + 9 * i];
				actuadores[j].temperature = buffer_in[8 + 9 * i];
			}
		}
	}
#endif
	timestamp = time(NULL);
}

void CM700::moveAll()
{
#ifdef USE_USB2DXL
	int param_per_actuator = 4;

	dxl_set_txpacket_id(BROADCAST_ID);
	dxl_set_txpacket_length((param_per_actuator + 1) * num_actuadores + 4);
	dxl_set_txpacket_instruction(INST_SYNC_WRITE);
	dxl_set_txpacket_parameter(0, P_GOAL_POSITION_L);
	dxl_set_txpacket_parameter(1, param_per_actuator);

	for (int i = 0; i < num_actuadores; i++) {
		dxl_set_txpacket_parameter(2 + 5 * i, actuadores[i].id);
		dxl_set_txpacket_parameter(2 + 5 * i + 1, _L16(actuadores[i].tposition));
		dxl_set_txpacket_parameter(2 + 5 * i + 2, _H16(actuadores[i].tposition));
		dxl_set_txpacket_parameter(2 + 5 * i + 3, _L16(actuadores[i].tspeed));
		dxl_set_txpacket_parameter(2 + 5 * i + 4, _H16(actuadores[i].tspeed));
	}

	dxl_txrx_packet();
#else
	buffer_out[0] = 0xFF;
	buffer_out[1] = SETPOSNDVEL;
	buffer_out[2] = num_actuadores;

	for (int i = 0; i < num_actuadores; i++) {
		buffer_out[5 * i + 3] = 254 - actuadores[i].id;
		buffer_out[5 * i + 4] = _L16(actuadores[i].tposition);
		buffer_out[5 * i + 5] = _H16(actuadores[i].tposition);
		buffer_out[5 * i + 6] = _L16(actuadores[i].tspeed);
		buffer_out[5 * i + 7] = _H16(actuadores[i].tspeed);
	}

	serial_io_flush(fd);
	serial_send(fd, buffer_out, 5 * num_actuadores + 3);
#endif
}

void CM700::setTorque(bool enable)
{
	int value = 0;
	if (enable == true) {
		this->moveAll();
		value = 1;
	} else {
		value = 0;
	}

#ifdef USE_USB2DXL
	dxl_set_txpacket_id(BROADCAST_ID);
	dxl_set_txpacket_length(2 * num_actuadores + 4);
	dxl_set_txpacket_instruction(INST_SYNC_WRITE);
	dxl_set_txpacket_parameter(0, P_TORQUE_ENABLE);
	dxl_set_txpacket_parameter(1, 1);

	for (int i = 0; i < num_actuadores; i++) {
		dxl_set_txpacket_parameter(2 + 2 * i, actuadores[i].id);
		dxl_set_txpacket_parameter(3 + 2 * i, value);
	}

	dxl_txrx_packet();
#else
	buffer_out[0] = 0xFF;
	buffer_out[1] = SETTORQUE;
	buffer_out[2] = num_actuadores;
	buffer_out[3] = value;

	serial_io_flush(fd);
	serial_send(fd, buffer_out, 4);
#endif
}

int CM700::getCamPosition()
{
	int retval = -1;
#ifdef USE_USB2DXL
	dxl_set_txpacket_id(servocam.id);
	dxl_set_txpacket_length(4);
	dxl_set_txpacket_instruction(INST_READ); 
	dxl_set_txpacket_parameter(0, P_PRESENT_POSITION_L); 
	dxl_set_txpacket_parameter(1, 11);
	dxl_txrx_packet();

	if (dxl_get_result() != COMM_RXSUCCESS) {
		return retval;
	}

	servocam.cposition = _MW(dxl_get_rxpacket_parameter(0), dxl_get_rxpacket_parameter(1));
	servocam.cspeed = _MW(dxl_get_rxpacket_parameter(2), dxl_get_rxpacket_parameter(3)) & 0x3FF;
	servocam.load = _MW(dxl_get_rxpacket_parameter(4), dxl_get_rxpacket_parameter(5));
	servocam.volt = dxl_get_rxpacket_parameter(6);
	servocam.temperature = dxl_get_rxpacket_parameter(7);
#else
	buffer_out[0] = 0xFF;
	buffer_out[1] = ASKDATACTID;
	buffer_out[2] = 1;
	buffer_out[3] = 254 - servocam.id;

	serial_io_flush(fd);
	serial_send(fd, buffer_out, 4);
	serial_read(fd, buffer_in, 3, DEF_TIMEOUT);

	if (buffer_in[0] != 0xFF || buffer_in[1] != RPYDATAID) {
		serial_io_flush(fd);
		return retval;
	}

	if (buffer_in[2] == 1) {
		serial_read(fd, buffer_in, 9, DEF_TIMEOUT);
		servocam.cposition = _MW(buffer_in[1], buffer_in[2]);
		servocam.cspeed = _MW(buffer_in[3], buffer_in[4]) & 0x3FF;
		servocam.load = _MW(buffer_in[5], buffer_in[6]);
		servocam.volt = buffer_in[7];
		servocam.temperature = buffer_in[8];
		
		retval = servocam.cposition;
	}
	serial_io_flush(fd);
#endif
	return retval;
}

void CM700::setCamPosition(int tpos, int tspeed)
{
	servocam.tposition = tpos;
	servocam.tspeed = tspeed;

#ifdef USE_USB2DXL
	dxl_set_txpacket_id(BROADCAST_ID);
	dxl_set_txpacket_length((4 + 1) * 1 + 4);
	dxl_set_txpacket_instruction(INST_SYNC_WRITE);
	dxl_set_txpacket_parameter(0, P_GOAL_POSITION_L);
	dxl_set_txpacket_parameter(1, 4);
	dxl_set_txpacket_parameter(2, servocam.id);
	dxl_set_txpacket_parameter(3, _L16(servocam.tposition));
	dxl_set_txpacket_parameter(4, _H16(servocam.tposition));
	dxl_set_txpacket_parameter(5, _L16(servocam.tspeed));
	dxl_set_txpacket_parameter(6, _H16(servocam.tspeed));
	dxl_txrx_packet();
#else
	buffer_out[0] = 0xFF;
	buffer_out[1] = SETPOSNDVEL;
	buffer_out[2] = 1;
	buffer_out[3] = 254 - servocam.id;
	buffer_out[4] = _L16(tpos);
	buffer_out[5] = _H16(tpos);
	buffer_out[6] = _L16(tspeed);
	buffer_out[7] = _H16(tspeed);

	serial_io_flush(fd);
	serial_send(fd, buffer_out, 8);
#endif
}

void CM700::printValues() {
	for (int i = 0; i < num_actuadores; i++) {
		printf("[%03u] [CP: %4u|TP: %4u] [CV: %4u|TV: %4u]\n",
			actuadores[i].id,
			actuadores[i].cposition, actuadores[i].tposition,
			actuadores[i].cspeed, actuadores[i].tspeed);
	}
	printf("[%03u] [CP: %4u|TP: %4u] [CV: %4u|TV: %4u]\n",
			servocam.id,
			servocam.cposition, servocam.tposition,
			servocam.cspeed, servocam.tspeed);
}

/* Se obtienen los datos de los sensores listos para */
void CM700::obtener_datos_en_arreglo(char buffer[TAMANO_BUFFER_COMUNICACION], int * count)
{
	memset(buffer, 0, *count);
    buffer[0]= 'M';
    *count = 0; 
    for (int k = 0; k < N_LEGS; k++) {
        for (int i = 0; i < GRADOS_LIBERTAD; i++) { 
            buffer[1 + *count] = (char)(actuadores[i + k * GRADOS_LIBERTAD].temperature);
            //cout << "actuadores["<< i+k*GRADOS_LIBERTAD << "].temperature: " << actuadores[i+k*GRADOS_LIBERTAD].temperature << endl;
            *count += 1;   
        }
    }
    for (int k = 0; k < N_LEGS; k++){
        for (int i = 0; i < GRADOS_LIBERTAD; i++)
        {
        	// Se necesitan solo 2 bytes para contener el valor de load.
            for (int j = 0; j < 2; j++) {
                buffer[1 + *count] = (char)(actuadores[i + k * GRADOS_LIBERTAD].load >> j * 8);
                *count += 1;
            }
        }
    }
}

#endif
