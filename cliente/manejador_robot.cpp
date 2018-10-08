#ifndef MANEJADOR_ROBOT_CPP
#define MANEJADOR_ROBOT_CPP

#include "manejador_robot.h"

void manejador_robot(tubos * todos_los_tubos)
{ 
	int modulePos[] = MODULE_POS;
	int n;
	CM700 * cm700 = new CM700(GRADOS_LIBERTAD*N_LEGS,GRADOS_LIBERTAD);
	Robot * crabot = new Robot(cm700, modulePos, todos_los_tubos);
	char cadena[TAMANO_BUFFER_COMUNICACION];
	
	while(1){
	
		if(poll(&(todos_los_tubos->fd_CS_to_MR),1,TIEMPO_ESPERA_EM_POLL_MILISECONDS)){
			n = read(todos_los_tubos->pipe_CS_to_MR[0], cadena, TAMANO_BUFFER_COMUNICACION); if(n<0){perror("error en lectura por tubos");exit(1);}
			obtener_accion(cadena, crabot, todos_los_tubos);
		}
		crabot->sensorizacion();
		usleep(10000);		 
	}
}

void obtener_accion(char cadena[TAMANO_BUFFER_COMUNICACION], Robot * crabot, tubos * todos_los_tubos)
{
	if(cadena[0] == 'C')
		caminata(cadena+1, crabot, todos_los_tubos);
    else if(cadena[0] == 'G')
        girar(cadena+1, crabot, todos_los_tubos);
    else if(cadena[0] == 'A')
        cambiar_altura(cadena+1, crabot, todos_los_tubos);
    else if(cadena[0] == 'I')
        inclinar_robot(cadena+1, crabot, todos_los_tubos);
    else if(!strcmp(cadena,(char *)"relajar"))
        relajar_robot(cadena+1, crabot, todos_los_tubos);
    else if(!strcmp(cadena,(char *)"reincorporar"))
        reincorporar_robot(cadena+1, crabot, todos_los_tubos);
}

void caminata(char cadena[TAMANO_BUFFER_COMUNICACION-1], Robot * crabot, tubos * todos_los_tubos)
{
	int angulo = determinar_cadena(cadena);
	cout <<  "angulo: " <<angulo << endl;
	int exclude_move[] = EXCLUDE_MOVE;
	cout <<  "angulo enviado:  " <<angulo*GRA + ANGULO_IDENTIFICADOR_INICIAL << endl;
	while(1){		
		crabot->caminar(angulo*GRA + ANGULO_IDENTIFICADOR_INICIAL, exclude_move);//direccion x desde el punto de vista del comienzo de las matrices de rotacion.
		if(poll(&(todos_los_tubos->fd_CS_to_MR),1,TIEMPO_ESPERA_EM_POLL_MILISECONDS)){//A llegado peticion del usuario.
			bzero(cadena, TAMANO_BUFFER_COMUNICACION-1);				
			if(read((todos_los_tubos->fd_CS_to_MR).fd, cadena, TAMANO_BUFFER_COMUNICACION) < 0){perror("error leyendo desde tubos");exit(1);}
			if(!strcmp(cadena,(char *)"detener")){
				crabot->detener(exclude_move);
				break;
			}
		}
	}
}

void girar(char cadena[TAMANO_BUFFER_COMUNICACION-1], Robot * crabot, tubos * todos_los_tubos)
{
    int direccion = determinar_cadena(cadena);    
	int exclude_move[] = EXCLUDE_MOVE;
    if (direccion == 0){
        cout << "direccion: derecha" << endl;
        while(1){		
		    crabot->girar_derecha(exclude_move);//direccion x desde el punto de vista del comienzo de las matrices de rotacion.
		    if(poll(&(todos_los_tubos->fd_CS_to_MR),1,TIEMPO_ESPERA_EM_POLL_MILISECONDS)){//A llegado peticion del usuario.
			    bzero(cadena, TAMANO_BUFFER_COMUNICACION-1);				
			    if(read((todos_los_tubos->fd_CS_to_MR).fd, cadena, TAMANO_BUFFER_COMUNICACION) < 0){perror("error leyendo desde tubos");exit(1);}
		        if(!strcmp(cadena,(char *)"detener")){
			        crabot->detener(exclude_move);
			        break;
		        }
	        }
	    }
    }
    else{
        if (direccion == 1){
            cout << "direccion: izquierda" << endl;
            while(1){		
		        crabot->girar_izquierda(exclude_move);//direccion x desde el punto de vista del comienzo de las matrices de rotacion.
		        if(poll(&(todos_los_tubos->fd_CS_to_MR),1,TIEMPO_ESPERA_EM_POLL_MILISECONDS)){//A llegado peticion del usuario.
			        bzero(cadena, TAMANO_BUFFER_COMUNICACION-1);				
			        if(read((todos_los_tubos->fd_CS_to_MR).fd, cadena, TAMANO_BUFFER_COMUNICACION) < 0){perror("error leyendo desde tubos");exit(1);}
		            if(!strcmp(cadena,(char *)"detener")){
			            crabot->detener(exclude_move);
			            break;
		            }
	            }
	        }
        }
    }
    
}

void cambiar_altura(char cadena[TAMANO_BUFFER_COMUNICACION-1], Robot * crabot, tubos * todos_los_tubos)
{   
    static int altura_pasada = 0;
    
    char * pch;
    pch = strtok (cadena,";");
    if(pch == NULL){perror("error 1, mensaje mal recibido en 'cambiar_altura'.");exit(1);}
    int direccion = atoi(pch);
    pch = strtok (NULL, ";");
    if(pch == NULL){perror("error 2, mensaje mal recibido en 'cambiar_altura'.");exit(1);}
    int magnitud = atoi(pch);
    if(!direccion)  magnitud = -magnitud;
    
	int exclude_move[] = EXCLUDE_MOVE;
	int cantidad_movimiento = magnitud - altura_pasada;
    if (cantidad_movimiento < 0){
        cout << "direccion: bajar" << endl;
        while(crabot->bajar(false, -cantidad_movimiento, exclude_move)){		
	        if(poll(&(todos_los_tubos->fd_CS_to_MR),1,TIEMPO_ESPERA_EM_POLL_MILISECONDS)){//A llegado peticion del usuario.
		        bzero(cadena, TAMANO_BUFFER_COMUNICACION-1);				
		        if(read((todos_los_tubos->fd_CS_to_MR).fd, cadena, TAMANO_BUFFER_COMUNICACION) < 0){perror("error leyendo desde tubos");exit(1);}
	            if(!strcmp(cadena,(char *)"detener")){
		            crabot->detener(exclude_move);
		            break;
	            }
            }
        }
    }else{
        if (cantidad_movimiento > 0){
            cout << "direccion: subir" << endl;
            while(crabot->levantar(false, cantidad_movimiento, exclude_move)){		
                if(poll(&(todos_los_tubos->fd_CS_to_MR),1,TIEMPO_ESPERA_EM_POLL_MILISECONDS)){//A llegado peticion del usuario.
		            bzero(cadena, TAMANO_BUFFER_COMUNICACION-1);				
		            if(read((todos_los_tubos->fd_CS_to_MR).fd, cadena, TAMANO_BUFFER_COMUNICACION) < 0){perror("error leyendo desde tubos");exit(1);}
	                if(!strcmp(cadena,(char *)"detener")){
		                crabot->detener(exclude_move);
		                break;
	                }
                }
            }
        }
    }
    crabot->detener(exclude_move);
    altura_pasada = magnitud;
}
   
void inclinar_robot(char cadena[TAMANO_BUFFER_COMUNICACION-1], Robot * crabot, tubos * todos_los_tubos)
{
    char * pch;
    pch = strtok (cadena,";");
    if(pch == NULL){perror("error 1, mensaje mal recibido en 'inclinar_robot'.");exit(1);}
    int direccion = atoi(pch);
    pch = strtok (NULL, ";");
    if(pch == NULL){perror("error 2, mensaje mal recibido en 'inclinar_robot'.");exit(1);}
    int magnitud = atoi(pch);
	int exclude_move[] = EXCLUDE_MOVE;
	cout << "inclinar_robot" << endl;
    while(crabot->cambiar_inclinacion(direccion, magnitud, exclude_move)){
        if(poll(&(todos_los_tubos->fd_CS_to_MR),1,TIEMPO_ESPERA_EM_POLL_MILISECONDS)){//A llegado peticion del usuario.
            bzero(cadena, TAMANO_BUFFER_COMUNICACION-1);				
            if(read((todos_los_tubos->fd_CS_to_MR).fd, cadena, TAMANO_BUFFER_COMUNICACION) < 0){perror("error leyendo desde tubos");exit(1);}
            if(!strcmp(cadena,(char *)"detener")){
                crabot->detener(exclude_move);
                break;
            }
        }
    }
    crabot->detener(exclude_move);
}

void relajar_robot(char cadena[TAMANO_BUFFER_COMUNICACION-1], Robot * crabot, tubos * todos_los_tubos)
{
    int exclude_move[] = EXCLUDE_MOVE;
    cout << "relajar" << endl;
    crabot->relajar(exclude_move);
}

void reincorporar_robot(char cadena[TAMANO_BUFFER_COMUNICACION-1], Robot * crabot, tubos * todos_los_tubos)
{
    int exclude_move[] = EXCLUDE_MOVE;
    cout << "reincorporar" << endl;
    crabot->reincorporar(exclude_move);
}

int determinar_cadena(char cadena[TAMANO_BUFFER_COMUNICACION-1])
{
	return atoi(cadena); //Por implementar.
}


#endif
