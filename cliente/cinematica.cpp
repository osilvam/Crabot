#ifndef CINEMATICA_CPP
#define CINEMATICA_CPP
#include "cinematica.h"


mat punta(int identificador,float altura_inicial_patas, float * angulos, int * exclude){
    float nuevos_angulos[GRADOS_LIBERTAD];
    float angulos_iniciales[] = ANGULOS_INICIALES;
    int indice = 0;
    for(int i = 0; i < GRADOS_LIBERTAD; i++){
        if(exclude[i] == 1){
            nuevos_angulos[i] = angulos[indice];
            indice++;
        }
        else
            nuevos_angulos[i] = TRANSFORMAR_A_RADIANES(angulos_iniciales[i]);
    }
    #ifdef _ARGO
    return rotz(identificador*2*M_PI/N_LEGS + ANGULO_IDENTIFICADOR_INICIAL)*traslacion(RADIO,0,altura_inicial_patas)*rotz(-(nuevos_angulos[0]-150.0*GRA))*traslacion(DISTANCIA_1,0,0)*roty(150.0*GRA-nuevos_angulos[1])*traslacion(DISTANCIA_2,0,-DISTANCIA_3)*roty(nuevos_angulos[2]-150.0*GRA)*traslacion(-DISTANCIA_5,0,-DISTANCIA_4);
    #endif
    #ifdef _CRABOT
    return rotz(identificador*2*M_PI/N_LEGS + ANGULO_IDENTIFICADOR_INICIAL)*traslacion(RADIO,0,altura_inicial_patas)*rotx(-(180.0*GRA-nuevos_angulos[0]))*traslacion(DISTANCIA_0,0,0)*rotz((nuevos_angulos[1]-180.0*GRA))*traslacion(DISTANCIA_1,0,0)*roty(180.0*GRA-nuevos_angulos[2])*traslacion(DISTANCIA_2,0,-DISTANCIA_3)*roty(180.0*GRA-nuevos_angulos[3])*traslacion(-DISTANCIA_5,0,-DISTANCIA_4);
    #endif
     #ifdef _ZBOT
    return rotz(identificador*2*M_PI/N_LEGS + ANGULO_IDENTIFICADOR_INICIAL)*traslacion(RADIO,0,altura_inicial_patas)*rotz(-(nuevos_angulos[0]-180.0*GRA))*traslacion(DISTANCIA_1,0,0)*roty(180.0*GRA-nuevos_angulos[1])*traslacion(DISTANCIA_2,0,-DISTANCIA_3)*roty(-nuevos_angulos[2]+180.0*GRA)*traslacion(-DISTANCIA_5,0,-DISTANCIA_4);
    #endif
}

mat punta_inclinacion(float ori_x, float ori_y, int identificador, float altura_inicial_patas, float * angulos, int * exclude){
    float nuevos_angulos[GRADOS_LIBERTAD];
    float angulos_iniciales[] = ANGULOS_INICIALES;
    int indice = 0;
    for(int i = 0; i < GRADOS_LIBERTAD; i++){
        if(exclude[i] == 1){
            nuevos_angulos[i] = angulos[indice];
            indice++;
        }
        else
            nuevos_angulos[i] = TRANSFORMAR_A_RADIANES(angulos_iniciales[i]);
    }
    #ifdef _ARGO
    return rotx(ori_x)*roty(ori_y)*rotz(identificador*2*M_PI/N_LEGS + ANGULO_IDENTIFICADOR_INICIAL)*traslacion(RADIO,0,altura_inicial_patas)*rotz(-(nuevos_angulos[0]-150.0*GRA))*traslacion(DISTANCIA_1,0,0)*roty(150.0*GRA-nuevos_angulos[1])*traslacion(DISTANCIA_2,0,-DISTANCIA_3)*roty(nuevos_angulos[2]-150.0*GRA)*traslacion(-DISTANCIA_5,0,-DISTANCIA_4);
    #endif
    #ifdef _CRABOT
    return rotx(ori_x)*roty(ori_y)*rotz(identificador*2*M_PI/N_LEGS + ANGULO_IDENTIFICADOR_INICIAL)*traslacion(RADIO,0,altura_inicial_patas)*rotx(-(150.0*GRA-nuevos_angulos[0]))*traslacion(DISTANCIA_0,0,0)*rotz((nuevos_angulos[1]-150.0*GRA))*traslacion(DISTANCIA_1,0,0)*roty(150.0*GRA-nuevos_angulos[2])*traslacion(DISTANCIA_2,0,-DISTANCIA_3)*roty(nuevos_angulos[3]-150.0*GRA)*traslacion(-DISTANCIA_5,0,-DISTANCIA_4);
    #endif
    #ifdef _ZBOT
    return rotx(ori_x)*roty(ori_y)*rotz(identificador*2*M_PI/N_LEGS + ANGULO_IDENTIFICADOR_INICIAL)*traslacion(RADIO,0,altura_inicial_patas)*rotz(-(nuevos_angulos[0]-180.0*GRA))*traslacion(DISTANCIA_1,0,0)*roty(180.0*GRA-nuevos_angulos[1])*traslacion(DISTANCIA_2,0,-DISTANCIA_3)*roty(-nuevos_angulos[2]+180.0*GRA)*traslacion(-DISTANCIA_5,0,-DISTANCIA_4);
    #endif
}

mat rotx(float x){
	mat A;
	A 	<< 1		<< 0 		<< 0 			<< 0 		<< endr
		<< 0 		<< cos(x) 	<< -sin(x) 		<< 0 		<< endr
		<< 0 		<< sin(x) 	<< cos(x) 		<< 0 		<< endr
		<< 0 		<< 0 		<< 0 			<< 1 		<< endr;
	return A;
}


mat roty(float x){
	mat A;
	A 	<< cos(x) 	<< 0 		<< sin(x) 		<< 0 		<< endr
		<< 0 		<< 1 		<< 0 			<< 0 		<< endr
		<< -sin(x)	<< 0 		<< cos(x) 		<< 0 		<< endr
		<< 0 		<< 0 		<< 0 			<< 1	 	<< endr;
	return A;
}

mat rotz(float x){
	mat A;
	A 	<< cos(x) 	<< -sin(x) 	<< 0 		<< 0 		<< endr
		<< sin(x) 	<< cos(x) 	<< 0 		<< 0 		<< endr
		<< 0 		<< 0 		<< 1 		<< 0 		<< endr
		<< 0 		<< 0 		<< 0 		<< 1		<< endr;
	return A;
}

mat traslacion(float x, float y, float z){
	mat A;
	A 	<< 1 << 0 << 0 << x << endr
		<< 0 << 1 << 0 << y << endr
		<< 0 << 0 << 1 << z << endr
		<< 0 << 0 << 0 << 1 << endr;
	return A;
}


float distancia(float * posicion_inicial, float * posicion_final){
	return sqrt((posicion_inicial[0] - posicion_final[0])*(posicion_inicial[0] - posicion_final[0]) + (posicion_inicial[1] - posicion_final[1])*(posicion_inicial[1] - posicion_final[1]) + (posicion_inicial[2] - posicion_final[2])*(posicion_inicial[2] - posicion_final[2]));
}

float * vector_pendiente(float * posicion_inicial, float * posicion_final){
	float * pendiente;
	pendiente = (float *)malloc(sizeof(float)*3);
	float distance = distancia(posicion_inicial, posicion_final);
	pendiente[0] = (posicion_final[0] - posicion_inicial[0]) / distance;	
	pendiente[1] = (posicion_final[1] - posicion_inicial[1]) / distance;
	pendiente[2] = (posicion_final[2] - posicion_inicial[2]) / distance;	
	return pendiente;
}

void mover_linea(int identificador,float altura_inicial_patas,float * angulos_iniciales, float * posicion_final, float velocity, int * IDS, int * exclude, int * angulos_a_enviar, int * velocidades){

	float angulos_anteriores[motores_enable(exclude)];
	mat M_angulos_proximos(motores_enable(exclude),1);
	mat M_velocidades_motores;
	mat M_jacobiano;
	mat M_velocidad;
	mat M_pendiente;
	mat M_posicion_inicial = punta(identificador, altura_inicial_patas, angulos_iniciales, exclude);


	float * pendiente;
	float posicion_inicial[] = {(float)M_posicion_inicial(0,3),(float)M_posicion_inicial(1,3),(float)M_posicion_inicial(2,3)};	
	float distance = distancia(posicion_inicial, posicion_final);
	for(int i=0;i<motores_enable(exclude);i++){angulos_anteriores[i]=angulos_iniciales[i];/*velocidades[i]=600;*/}
	posicion_inicial[0] = M_posicion_inicial(0,3); posicion_inicial[1] = M_posicion_inicial(1,3);posicion_inicial[2] = M_posicion_inicial(2,3);
	pendiente = vector_pendiente( posicion_inicial, posicion_final); 
	M_pendiente << pendiente[0] << endr << pendiente[1] << endr << pendiente[2] << endr;	
	M_velocidad << pendiente[0]*velocity << endr << pendiente[3]*velocity << endr << pendiente[2]*velocity << endr ;
	for(int i=0; i<motores_enable(exclude); i++)M_angulos_proximos(i,0) = angulos_iniciales[i];
	float i=0.0;
	while(i < distance){ 
		M_jacobiano = jacobiano_motores(identificador, altura_inicial_patas, angulos_anteriores, exclude);
		M_angulos_proximos =  solve(M_jacobiano,(M_pendiente*LARGO_ENTRE_PUNTOS_MILIMETROS)) + M_angulos_proximos;
		M_velocidades_motores = solve(M_jacobiano,M_velocidad);
		for(int j=0;j<motores_enable(exclude);j++){
		    angulos_anteriores[j] = acondicionar_angulos(M_angulos_proximos(j,0));
		    angulos_a_enviar[j] = (int)TRANSFORMAR_A_NUMEROS(angulos_anteriores[j]);
		    velocidades[j] = fabs(TRANSFORMAR_VELOCIDAD_A_NUMERO(M_velocidades_motores(j,0)));
		}
		i = i + LARGO_ENTRE_PUNTOS_MILIMETROS; 
	}	

    for(int i=0; i<motores_enable(exclude); i++) velocidades[i] = 600;
    usleep(50);
}

mat jacobiano_motores(int identificador, float altura_inicial_patas, float * angulos_iniciales, int * exclude){
	mat punta_result = punta(identificador, altura_inicial_patas, angulos_iniciales, exclude);
	mat punta_result_dt[motores_enable(exclude)];
	mat d_punta_result_dt[motores_enable(exclude)];
	mat jacobiano(3,motores_enable(exclude));

	for(int i = 0; i < motores_enable(exclude); i++){
		angulos_iniciales[i] = angulos_iniciales[i] + DELTA_DERIVATIVE_FOR_JACOBIAN;
		punta_result_dt[i] = punta(identificador, altura_inicial_patas, angulos_iniciales,exclude);
		d_punta_result_dt[i] = (punta_result_dt[i]-punta_result)/DELTA_DERIVATIVE_FOR_JACOBIAN;
		angulos_iniciales[i] = angulos_iniciales[i] - DELTA_DERIVATIVE_FOR_JACOBIAN;
	}
	for(int i = 0; i < motores_enable(exclude); i++){
		for(int j=0; j < 3; j++){
			jacobiano(j,i) = d_punta_result_dt[i](j,3);
		}
	}
	return jacobiano;
}



void kinematicPosition(int identificador, float altura_incial_patas, int * angulos_aux, float * posTipx, float * posTipy, float * posTipz, int * exclude){	
	mat A;
	float angulos[GRADOS_LIBERTAD];
	for(int i =0 ; i < GRADOS_LIBERTAD; i++) angulos[i] = TRANSFORMAR_A_RADIANES(angulos_aux[i]);
	A = punta(identificador, altura_incial_patas, angulos, exclude);
	*posTipx = A(0,3);	
	*posTipy = A(1,3);
	*posTipz = A(2,3);
}

void kinematicPosition_inclinacion(float ori_x, float ori_y, int identificador, float altura_incial_patas, int * angulos_aux, float * posTipx, float * posTipy, float * posTipz, int * exclude){	
	mat A;
	float angulos[GRADOS_LIBERTAD];
	for(int i =0 ; i < GRADOS_LIBERTAD; i++) angulos[i] = TRANSFORMAR_A_RADIANES(angulos_aux[i]);
	A = punta_inclinacion(ori_x, ori_y, identificador, altura_incial_patas, angulos, exclude);
	*posTipx = A(0,3);	
	*posTipy = A(1,3);
	*posTipz = A(2,3);
}


void calcular_mover(int index, float altura_inicial_patas, float * posicion_final, int * angulos_iniciales, int * IDS, int * exclude, int * angulos_a_enviar, int * velocidades){
	float angulos_iniciales_rad[motores_enable(exclude)];
	for(int i =0 ; i < motores_enable(exclude); i++) angulos_iniciales_rad[i]= TRANSFORMAR_A_RADIANES(angulos_iniciales[i]);
	mover_linea(index, altura_inicial_patas, angulos_iniciales_rad, posicion_final, VELOCIDAD_MOVIMIENTO, IDS, exclude, angulos_a_enviar, velocidades);	
}

int comparador(float *angulos, int * IDS){ // no implementada por costo de tiempo.
	return 1;
}

int entre_angular(float f1, float f2){
	if(fabs(f1-f2) <= ERROR_ACEPTABLE_COMPARADOR ){
		return 1;
	}
	else if(fabs(f1-f2) > 6){
		if(fabs(f1+f2-2*M_PI) <= ERROR_ACEPTABLE_COMPARADOR)
			return 1;			
		}
	return 0;
}

float acondicionar_angulos(float angulo){
	while(angulo<0)angulo+=2*M_PI;
	while(angulo>2*M_PI)angulo-=2*M_PI;
	return angulo;
}

int motores_enable(int * exclude){
    int count  = 0;
    for(int i = 0; i < GRADOS_LIBERTAD; i++){
        if(exclude[i] == 1)
            count++;
    }
    return count;
}

void calcular_mover_inclinacion(float ori_x, float ori_y, int index, float altura_inicial_patas, float * posicion_final, int * angulos_iniciales, int * IDS, int * exclude, int * angulos_a_enviar, int * velocidades){
	float angulos_iniciales_rad[motores_enable(exclude)];
	for(int i =0 ; i < motores_enable(exclude); i++) angulos_iniciales_rad[i]= TRANSFORMAR_A_RADIANES(angulos_iniciales[i]);
	mover_linea_inclinacion(ori_x, ori_y, index, altura_inicial_patas, angulos_iniciales_rad, posicion_final, VELOCIDAD_MOVIMIENTO, IDS, exclude, angulos_a_enviar, velocidades);	
}

void mover_linea_inclinacion(float ori_x, float ori_y, int identificador,float altura_inicial_patas,float * angulos_iniciales, float * posicion_final, float velocity, int * IDS, int * exclude, int * angulos_a_enviar, int * velocidades){

	float angulos_anteriores[motores_enable(exclude)];
	mat M_angulos_proximos(motores_enable(exclude),1);
	mat M_velocidades_motores;
	mat M_jacobiano;
	mat M_velocidad;
	mat M_pendiente;
	mat M_posicion_inicial = punta_inclinacion(ori_x, ori_y, identificador, altura_inicial_patas, angulos_iniciales, exclude);


	float * pendiente;
	float posicion_inicial[] = {(float)M_posicion_inicial(0,3),(float)M_posicion_inicial(1,3),(float)M_posicion_inicial(2,3)};	
	float distance = distancia(posicion_inicial, posicion_final);
	for(int i=0;i<motores_enable(exclude);i++){angulos_anteriores[i]=angulos_iniciales[i];/*velocidades[i]=600;*/}
	posicion_inicial[0] = M_posicion_inicial(0,3); posicion_inicial[1] = M_posicion_inicial(1,3);posicion_inicial[2] = M_posicion_inicial(2,3);
	pendiente = vector_pendiente( posicion_inicial, posicion_final); 
	M_pendiente << pendiente[0] << endr << pendiente[1] << endr << pendiente[2] << endr;	
	M_velocidad << pendiente[0]*velocity << endr << pendiente[3]*velocity << endr << pendiente[2]*velocity << endr ;
	for(int i=0; i<motores_enable(exclude); i++)M_angulos_proximos(i,0) = angulos_iniciales[i];
	float i=0.0;
	while(i < distance){ 
		M_jacobiano = jacobiano_motores(identificador, altura_inicial_patas, angulos_anteriores, exclude);
		M_angulos_proximos =  solve(M_jacobiano,(M_pendiente*LARGO_ENTRE_PUNTOS_MILIMETROS)) + M_angulos_proximos;
		M_velocidades_motores = solve(M_jacobiano,M_velocidad);
		for(int j=0;j<motores_enable(exclude);j++){
		    angulos_anteriores[j] = acondicionar_angulos(M_angulos_proximos(j,0));
		    angulos_a_enviar[j] = (int)TRANSFORMAR_A_NUMEROS(angulos_anteriores[j]);
		    velocidades[j] = fabs(TRANSFORMAR_VELOCIDAD_A_NUMERO(M_velocidades_motores(j,0)));
		}
		i = i + LARGO_ENTRE_PUNTOS_MILIMETROS; 
	}	

    for(int i=0; i<motores_enable(exclude); i++) velocidades[i] = 600;
    usleep(50);
}
#endif
