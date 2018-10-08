#include "comunicacion_servidor.hpp"
  #include <sstream>

void comunicacion_servidor(int argc, char *argv[], tubos * todos_los_tubos){
	int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[TAMANO_BUFFER_COMUNICACION];
	bzero(buffer,TAMANO_BUFFER_COMUNICACION);
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) error("ERROR, no such host\n");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) error("ERROR connecting");

    string ROB;
    ostringstream ROB_aux;
    ROB_aux << "ROB;" << N_LEGS << ";" << GRADOS_LIBERTAD << ";" << CONFIG_ROB;
    ROB = ROB_aux.str();
    n = write(sockfd,ROB.c_str(),strlen(ROB.c_str()));
    if (n < 0) error("ERROR writing to socket");

	struct pollfd fd_sockfd;
	fd_sockfd.fd = sockfd;
	fd_sockfd.events = POLLIN;




	/* YA ESTÁ CONECTADO EL CLIENTE SOCKET CON EL SERVIDOR.*/





	while(1){
		if(poll(&fd_sockfd,1,TIME_OUT_POLL_MSEC)){ //Llego información desde el servidor.
			bzero(buffer, TAMANO_BUFFER_COMUNICACION);
			n = read(fd_sockfd.fd,buffer,TAMANO_BUFFER_COMUNICACION); if(n < 0)error("ERROR reading from socket");
            if (buffer[0]=='w'){
                if(buffer[1]=='K'){
                    n = write(todos_los_tubos->pipe_CS_to_CAM[1],buffer+2,TAMANO_BUFFER_COMUNICACION-2);  if(n < 0)error("ERROR writing from socket");
                }
                else{
			        n = write(todos_los_tubos->pipe_CS_to_MR[1],buffer+1,TAMANO_BUFFER_COMUNICACION-1);  if(n < 0)error("ERROR writing from socket");			
		        }
             } 
        }
		if(poll(&(todos_los_tubos->fd_MR_to_CS),1,TIME_OUT_POLL_MSEC)){ //Llego información desde proceso manejador_robot
			bzero(buffer, TAMANO_BUFFER_COMUNICACION);
			n = read(todos_los_tubos->fd_MR_to_CS.fd, buffer, TAMANO_BUFFER_COMUNICACION); if(n < 0)error("ERROR reading from pipe");
			n = write(fd_sockfd.fd,buffer,TAMANO_BUFFER_COMUNICACION);  if(n < 0)error("ERROR writing from socket");
		}
	}
	close(sockfd);



	
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

