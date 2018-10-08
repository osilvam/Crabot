#include "main.hpp"

int main(int argc, char *argv[]){
	if (argc < 3){fprintf(stderr,"ERROR, no a ingresado informacion suficiente para la coneccion socket\n");exit(1);}

	tubos * todos_los_tubos = new tubos();	

	int pid;// process id.
	pid=fork(); if(pid<0){perror("Error en fork()");exit(1);}
	if(pid == 0){ // Hijo
		pid = fork();if(pid<0){perror("Error en fork()");exit(1);}
		if(pid == 0){ // Hijo del hijo.
			pid = fork();
			if(pid == 0){
				pid = fork();
				if(pid == 0){
					sensores_motores(); // no implementada
				}
				else{
					sensores(); // no implementada	
				}
			}			
			else {
				comunicacion_servidor(argc, argv, todos_los_tubos); // Este proceso se tiene que mejorar con el objeto que hizo felipe vera
			}			
		}
		else{
			manejador_robot(todos_los_tubos); // no implementada
		}
	}
	else{ //Padre.
		camara(todos_los_tubos);	
	}
	return 0;
}


void ZombieHandler(int signal_num) // CADA VEZ QUE TERMINA UN PROCESO HIJO SE EJECUTA ESTA INTERRUPCIÓN.
{
	signal(SIGCHLD, ZombieHandler);
	int status;
	waitpid(WAIT_ANY,&status,WNOHANG); /* 	ELIMINA LOS PROCESOS HIJOS YA TERMINADOS, PARA QUE NO QUEDEN EN ESTADO ZOMBIE. */
}