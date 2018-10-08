/////////////////////////////////////////////////////////////////////////////////////////////////
//  Clase Leg                                                                                  //
//                                                                                             //
//  Usada en la Clase Robot para la implementacion de cualquier plataforma robotica            //
//  con n numero de extremidades.                                                              //
//                                                                                             //
//  Autor:  Oscar Silva Muñoz.                                                                 //
//  Fecha:  30/07/2013.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cmath>
#include "serial.h"
#include <stdlib.h>
#include "cinematica.h"
#include "cm700.h"
#include <time.h>
#include <sys/time.h>

#ifndef LEG_H
#define LEG_H

class Leg
{
    int index;
    int grados_libertad;
    int *IDS;
    int *Angulos_actuales;
    int *Angulos_finales;
    int *Velocidades_finales;
    float footOriginalPos[3];
    float LegNewPos[3];
    float LegNewPosWork[3];
    float Pas_LegNewPosWork[3];
    float Old_zNivel;
	float altura_inicial_patas;
	CM700 * cm700;
public:
    Leg(CM700 * cm700, int _index, int grados_libertad, int *angulos_iniciales, float *posicion_inicial, float altura_inicial_patas);
    // _index:  numero identificador de cada pata (desde cero en adelante)
    // grados_libertad: cantidad de grados de libertad de la pata
    // angulos_iniciales:   arreglo del tamaño de la cantidad de grados de libertad con los angulos iniciales de cada uno de los motores
    ~Leg(){}
    void caminar(float*NewPosMove, int * exclude);
    void rotar(float*NewPosRot, int * exclude);
    void cambiar_nivel(float *NewNivel, int * exclude);
    void cambiar_nivel_inclinacion(float ori_x, float ori_y, float *NewNivel, int * exclude);
    void cambiar_radio(float *NewMove, int sentido, int * exclude);
    void adoptar_posicion_trabajo(int fd_serial, float * NewPosMove, int sentido, int * exclude);
    void posicion_trabajo(float * NewPosMove, int sentido, int posicion_trabajo, int * exclude);
    int Motores_enable(int * exclude);
    void adaptar_valores(int * exclude, int * IDS, int * Angulos_actuales, int * nuevos_IDS, int * nuevos_Angulos_actuales);
    void adaptar_resultados(int * exclude, int * nuevos_angulos, int * nuevas_velocidades, int * angulos_originales, int * velocidades_originales);
};

#endif // LEG_H
