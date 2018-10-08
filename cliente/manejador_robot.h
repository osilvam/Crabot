#ifndef MANEJADOR_ROBOT_H
#define MANEJADOR_ROBOT_H

#include <iostream>
#include <poll.h>
#include <string.h>
#include "robot.h"
#include "caracteristicas_robot.h"
#include <cmath>
#include "cm700.h"

//void manejador_robot(int * tubo_intermediario_robot, int * tubo_robot_intermediario);
//void obtener_accion(std::string cadena, Robot * crabot, FILE * stream_intermediario_robot, struct pollfd * tubo_intermediario_robot, int fd_serial);
//std::string extraer_informacion_por_tubos2(FILE * stream);
int determinar_cadena(char cadena[TAMANO_BUFFER_COMUNICACION-1]);
void caminata(char cadena[TAMANO_BUFFER_COMUNICACION-1], Robot * crabot, tubos * todos_los_tubos);
void girar(char cadena[TAMANO_BUFFER_COMUNICACION-1], Robot * crabot, tubos * todos_los_tubos);
void cambiar_altura(char cadena[TAMANO_BUFFER_COMUNICACION-1], Robot * crabot, tubos * todos_los_tubos);
void inclinar_robot(char cadena[TAMANO_BUFFER_COMUNICACION-1], Robot * crabot, tubos * todos_los_tubos);
void relajar_robot(char cadena[TAMANO_BUFFER_COMUNICACION-1], Robot * crabot, tubos * todos_los_tubos);
void reincorporar_robot(char cadena[TAMANO_BUFFER_COMUNICACION-1], Robot * crabot, tubos * todos_los_tubos);
void obtener_accion(char cadena[TAMANO_BUFFER_COMUNICACION], Robot * crabot, tubos * todos_los_tubos);
void manejador_robot(tubos * todos_los_tubos);

#endif
