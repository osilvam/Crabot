#ifndef COMUNICACION_SERVIDOR_H
#define COMUNICACION_SERVIDOR_H

#include "caracteristicas_robot.h"

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


void error(const char *msg);
void comunicacion_servidor(int argc, char *argv[], tubos * todos_los_tubos);
using namespace std;

#endif
