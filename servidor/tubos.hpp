#ifndef TUBOS_H
#define TUBOS_H


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

class tubos{
	public:
		// Primero una lista de todos los tubos.
	int tubo_internet_robot[2];
	int tubo_robot_internet[2];
	int tubo_internet_arduino[2];
	int tubo_robot_arduino[2];
	int tubo_arduino_robot[2];
	

	struct pollfd fd_internet_robot;
	struct pollfd fd_robot_internet;
	struct pollfd fd_internet_arduino;
	struct pollfd fd_robot_arduino;
	struct pollfd fd_arduino_robot;

	tubos();
};

#endif
