#ifndef CINEMATICA_H
#define CINEMATICA_H

#include <cmath>
#include "caracteristicas_robot.h"
#include <armadillo>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace arma;

mat rotx(float x);
mat roty(float x);
mat rotz(float x);
mat traslacion(float x,float y,float z);
mat punta(int identificador,float altura_incial_patas ,float * angulos, int * exclude);
mat punta_inclinacion(float ori_x, float ori_y, int identificador,float altura_incial_patas ,float * angulos, int * exclude);
//mat aproximacion_angulos_proximos(float * angulos, float xf, float yf, float zf);
float distancia(float x_i,float x_f,float y_i,float y_f,float z_i ,float z_f);
float * vector_pendiente(float x_i,float y_i, float z_i,float x_f,float y_f ,float z_f);
mat jacobiano_motores(int identificador, float altura_inicial_patas, float * angulos_iniciales, int * exclude);
void mover_linea(int identificador,float altura_incial_patas ,float * angulos_iniciales, float * posicion_final, float velocity, int * IDS, int * exclude, int * angulos_a_enviar, int * velocidades);
void mover_linea_inclinacion(float ori_x, float ori_y, int identificador,float altura_incial_patas ,float * angulos_iniciales, float * posicion_final, float velocity, int * IDS, int * exclude, int * angulos_a_enviar, int * velocidades);
void kinematicPosition(int identificador,float altura_incial_patas, int * angulos_aux, float * posTipx, float * posTipy, float * posTipz, int * exclude);
void kinematicPosition_inclinacion(float ori_x, float ori_y, int identificador, float altura_incial_patas, int * angulos_aux, float * posTipx, float * posTipy, float * posTipz, int * exclude);
void calcular_mover(int index, float altura_incial_patas,float * posicion_final, int * angulos_iniciales, int * IDS, int * exclude, int * angulos_a_enviar, int * velocidades);
void calcular_mover_inclinacion(float ori_x, float ori_y, int index, float altura_incial_patas,float * posicion_final, int * angulos_iniciales, int * IDS, int * exclude, int * angulos_a_enviar, int * velocidades);
void move_leg(int d_serial, int num , int *IDS, float * angulos_anteriores, float * velocidades);
int comparador(float *angulos, int * IDS);
int entre_angular(float f1, float f2);
float acondicionar_angulos(float angulo);
int motores_enable(int * exclude);

#endif
