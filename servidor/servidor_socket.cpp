/*
	ALOJADO EN SERVIDOR!
*/


#ifndef MAIN_CPP
#define MAIN_CPP


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <unistd.h>
#include <poll.h>
	
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include "tubos.hpp"



using namespace std;
#define TIME_OUT_POLL_MSEC 30
#define TAMANO_BUFFER 10000
#define RUTA_TEMPERATURA "/var/www/html/DashBoard/temperatura"
//#define RUTA_CONFIG "/var/www/html/DashBoard/config"//no funcional hasta que warren explique el ultimo config

void clasificador(int sock, tubos * todos_los_tubos);
void error(const char *msg);
void robot(int sock, tubos * todos_los_tubos);
char * extraer_informacion_por_tubos(FILE * stream);
void arduino(int sock, tubos * todos_los_tubos);
void sensores(char cadena[]);

void ZombieHandler(int signal_num) // CADA VEZ QUE TERMINA UN PROCESO HIJO SE EJECUTA ESTA INTERRUPCIÓN.
{
	 signal(SIGCHLD, ZombieHandler);
	 int status;
     waitpid(WAIT_ANY,&status,WNOHANG); /* 	ELIMINA LOS PROCESOS HIJOS YA TERMINADOS, PARA QUE NO QUEDEN EN ESTADO ZOMBIE. */
}


int PID_ROBOT = 0;
int N_LEGS;
int GRADOS_LIBERTAD;


int main(int argc, char *argv[]){

	if (argc < 2) { /* EL USUARIO DEBE IINTRODUCIR EL PUERTO  A USAR */
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

    signal(SIGCHLD, ZombieHandler);//Para la elimiación de procesos hijos.

	//Se inicializan todas las tuberías que luego usarán los procesos hijos para comunicarse entre ellos.
	
	
	//------------------------------------------------------------------------------- Se comienza a crear un servidor socket.  
	 int sockfd, newsockfd, portno; 
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//--------------Este pedazo de código deja el socket listo para usarse -----------
	
	int yes=1;
	if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) { // ESTA FUNCIÓN ES LA QUE CIERRA TODOS LOS CODIGOS QUE SE ESTEN EJECUTANDO EN AQUEL PUERTO PERO SÓLO SI EL PUERTO YA NO ESTÁ EN USO, O SEA ES PERFECTA. 
			perror("setsockopt");
			exit(1);
	} 
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
	 
     listen(sockfd,5);
     clilen = sizeof(cli_addr);






    tubos * todos_los_tubos = new tubos();






	 //------------------------------------------------------------------------------- Ya está listo el servidor socket.
	while (1) { // EL PROCESO PRINCIPAL SE MANTIENE ATENDIENDO LLAMADAS POR SOCKET. 
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
		printf("Conected\n");
		 if (newsockfd < 0) 
			 error("ERROR on accept");
		clasificador(newsockfd, todos_los_tubos);
		close(newsockfd);
	 } 










	close(sockfd);
	return 0; 
}



void clasificador(int sock, tubos * todos_los_tubos){
	int n;
	char buffer[TAMANO_BUFFER];
	bzero(buffer,TAMANO_BUFFER);
	n = read(sock,buffer,TAMANO_BUFFER);
	cout << buffer << endl;
	if (n < 0) error("ERROR reading from socket");
	if(buffer[0]=='w'){
		write(todos_los_tubos->tubo_internet_robot[1], buffer, TAMANO_BUFFER);
	}
	else if(buffer[0]== 'R' && buffer[1] == 'O' && buffer[2]== 'B'){
		int i=4;
		int j=0;
		int k=0;
		while(buffer[i] != ';'){
			i+=1;
		}
		char numero_patas[i-4];
		for(j=4; j<i; j++){
			numero_patas[j-4] = buffer[j];
		}
		if(i == 5)
			N_LEGS = numero_patas[0]-48;
		else
			N_LEGS = atoi(numero_patas);
		
		i+=1;
		int aux_i = i;
		while(buffer[i]!=';'){
			i+=1;
		}
		char numero_motores_por_pata[i-aux_i];
		//cout << "i: " << i << "aux_i: " << aux_i << endl;
		for (k = aux_i; k < i; k++){
			//cout << "k: " << k << endl;
			numero_motores_por_pata[k-aux_i] = buffer[k];
		}
		if(k == (aux_i+1))
			GRADOS_LIBERTAD = (int)(numero_motores_por_pata[0])-48;
		else 
			GRADOS_LIBERTAD = atoi(numero_motores_por_pata);
		/*
		char ruta_config[] = RUTA_CONFIG;
		ofstream archivo[1];
		archivo[0].open(ruta_config, ios::trunc);
		char buffer_aux[TAMANO_BUFFER - (i+1)];
		strcpy (buffer_aux, buffer + i + 1);
		archivo[0] << buffer_aux; 
		archivo[0].close();
		*/

		printf("Numero de patas robot: %d\t numero de motores por pata: %d\n", N_LEGS, GRADOS_LIBERTAD);
		robot(sock,todos_los_tubos);
	}
	else if (buffer[0]== 'A' && buffer[1] == 'R' && buffer[2]== 'D'){
		arduino(sock,todos_los_tubos); 
	}
}



void robot(int sock, tubos * todos_los_tubos){
	if(PID_ROBOT != 0){
		kill (PID_ROBOT, SIGTERM);
		kill (PID_ROBOT, SIGKILL);
	}
	PID_ROBOT = fork();
	if(PID_ROBOT == 0){
		
		struct pollfd fd_socket_robot;
		fd_socket_robot.fd = sock;
		fd_socket_robot.events = POLLIN;

		char cadena[TAMANO_BUFFER];
		int n;
		while(1){
			if(poll(&(todos_los_tubos->fd_internet_robot),1,TIME_OUT_POLL_MSEC)){ //A llegado peticion del usuario.
				bzero(cadena,TAMANO_BUFFER);
				read(todos_los_tubos->tubo_internet_robot[0], cadena, TAMANO_BUFFER);
				write(sock, cadena ,TAMANO_BUFFER);
				usleep(1000);
			}	
			else if(poll(&fd_socket_robot,1,TIME_OUT_POLL_MSEC)){
				bzero(cadena,TAMANO_BUFFER);
				n = read(sock,cadena,TAMANO_BUFFER);if(n<0){perror("Error al leer por socket");exit(1);}
				if(cadena[0]=='M'){
					sensores(cadena+1);
				}
				else if (cadena[0]=='A')
				{
					write(todos_los_tubos->tubo_robot_arduino[1], cadena+1, TAMANO_BUFFER - 1);
				}
			}
		}
	}
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void sensores(char cadena[])
{
	int i;
	int j;
	int contador=0;
	int temperature[N_LEGS][GRADOS_LIBERTAD];

	char ruta_temperatura[] = RUTA_TEMPERATURA;
	ofstream archivo[1];
	archivo[0].open(ruta_temperatura, ios::trunc);
	
	for(i=0; i < N_LEGS; i++)
	{
		for (j = 0; j < GRADOS_LIBERTAD; j++)
		{
			temperature[i][j] = (int)(cadena[i*GRADOS_LIBERTAD + j]);
			contador+=1;
			archivo[0] << temperature[i][j] << "\t";
			//cout << "temp " << 1 + j + 3*i << ":" << temperature[i][j]  << endl;
		}
	}
	archivo[0].close();
}

void arduino(int sock, tubos * todos_los_tubos)//Implementar.
{
	char cadena[TAMANO_BUFFER];
	while(1){
		if(poll(&(todos_los_tubos->fd_robot_arduino),1,TIME_OUT_POLL_MSEC)){ //A llegado peticion del usuario.
			bzero(cadena,TAMANO_BUFFER);
			read(todos_los_tubos->tubo_robot_arduino[0], cadena, TAMANO_BUFFER-1);
			write(sock, cadena ,TAMANO_BUFFER-1);
		}
		usleep(5000);
	}
}
#endif
