#include "camara.hpp"
void camara(tubos * todos_los_tubos){ //funcion a llenar.
	CM700 * cm700 = new CM700(GRADOS_LIBERTAD*N_LEGS,GRADOS_LIBERTAD);
	int velocity = 100;
	int tpos = 0;
	int n;
	char cadena[TAMANO_BUFFER_COMUNICACION];
	while(1){
		if(poll(&(todos_los_tubos->fd_CS_to_CAM),1,TIEMPO_ESPERA_EM_POLL_MILISECONDS)){
			n = read(todos_los_tubos->pipe_CS_to_CAM[0], cadena, TAMANO_BUFFER_COMUNICACION); if(n<0){perror("error en lectura por tubos");exit(1);}
			tpos = atoi(cadena);
			tpos = (int)(tpos*(ANGULO_RESOLUCION/360.0));
			cout << "Valor a enviar a camara: " << tpos << endl;
			cm700->setCamPosition(tpos, velocity);
		}
		usleep(10000);
	}
}