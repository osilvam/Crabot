#ifndef MAIN_H
#define MAIN_H


/*
#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <string>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <unistd.h>
#include <poll.h>
#include <stdlib.h>
#include <netdb.h> 
*/

#include "manejador_robot.h"
#include "comunicacion_servidor.hpp"
#include "caracteristicas_robot.h"
#include "sensores.hpp"
#include "sensores_motores.hpp"
#include "camara.hpp"

void ZombieHandler(int signal_num);

#endif
