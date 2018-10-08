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
		int pipe_CS_to_MR[2];
		int pipe_MR_to_CS[2];
		int pipe_CAM_to_CS[2];
		int pipe_CS_to_CAM[2];

		struct pollfd fd_MR_to_CS;
		struct pollfd fd_CS_to_MR;
		struct pollfd fd_CAM_to_CS;
		struct pollfd fd_CS_to_CAM;

		tubos();
};

#endif
