/////////////////////////////////////////////////////////////////////////////////////////////////
//  Clase Robot                                                                                //
//                                                                                             //
//  Usar para la implementacion de cualquier plataforma robotica con n numero de extremidades. //
//                                                                                             //
//  Autor:  Oscar Silva Muñoz.                                                                 //
//  Fecha:  30/07/2013.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ROBOT_H
#define ROBOT_H
#include <iostream>
#include <vector>
#include "leg.h"
#include <cmath>
#include "serial.h"
#include <time.h>
#include <stdlib.h>
#include "cinematica.h"
#include "caracteristicas_robot.h"
#include <unistd.h>
#include "cm700.h"

class Robot
{
    int n_legs;//numero de patas
    int grados_libertad;//cantidad de motores por pata
    float radio;//radio desde el centro del robot hasta la punta de una pata
    int interModuleDelay;//cantidad de movimientos entre patas
    int *modulePos;//orden de movimiento de las patas//modificable a la cantidad de patas
    int tableLength;//largo de las tablas de movimiento
	float altura_inicial_patas;//Para simplicidad del código se calcula la altura inicial de las patas
    std::vector<float> xMovementTable;
    std::vector<float> yMovementTable;
    std::vector<float> zMovementTable;
    std::vector<float> rMovementTable;
    std::vector<float> rRotationTable;
    std::vector<float> thetaRotationTable;
    float *xNivelTable;
    float *yNivelTable;
    float *zNivelTable;
    float *posTipx;
    float *posTipy;
    float *posTipz;
    float posTipzSUMA;
    float *zTipMove;
    int vstep_down;//pasos en que baja el centro de masa
    int vstep_up;//pasos en que sube el centro de masa
    int phase, rot;//fases de caminata y rotacion a ia izquierda y derecha
    float r, z, theta;
    int c;
    int cm0;
    int cm1;

    float direction;//direccion de caminata
    float forwardVel;//velocidad de caminata(amplifica la cantidad de movimiento en (x,y))
    int aux;
    //int aux2;

    float step;//cantidad de movimiento en (x,y) para movimientos
    float vstep;//cantidad de movimiento en z para movimientos

    int rotar_pasos;// pasos para generar una rotacion de 60°
    float rotar_angle;//cantidad de movimiento en grados para generar rotacion

    float ori_x;
    float ori_y;
    float MAX_ori_x;
    float MAX_ori_y;
    float angulo_paso;

    std::vector<Leg> Legs;
    
    bool relajado;
    
    int work_area;
    float aux_position;
    int pata_1, pata_2;
    float workstep;
    float orientation;
    
    CM700 * cm700;
    tubos * todos_los_tubos;
	
	#ifdef _ZBOT
	int * ventosas;
	int iterador_ventosas;
	#endif


public:
    Robot(CM700 * cm700, int * _modulePos, tubos * todos_los_tubos);

    ~Robot(){}
    void detener(int * exclude);
    void caminar(float _direction, int * exclude);
    void girar_izquierda(int * exclude);
    void girar_derecha( int * exclude);
    int levantar(bool reincorporar, int magnitud, int * exclude);
    int bajar(bool descanso, int magnitud, int * exclude);
    int cambiar_inclinacion(int direccion, int magnitud, int * exclude);    
    int orientarse_objeto( float object_position, int * exclude);
    int posicion_trabajo( int posicion_trabajo, int * exclude);
    int cambiar_radio( int sentido, int * exclude);
    void relajar(int * exclude);
    void reincorporar(int * exclude);
    void obtener_posicion_actual();
    void obtener_posicion_actual_inclinacion();
    float obtener_radio(int * angulos_iniciales);
	float altura_inicial_patas_f(int * angulos_iniciales);
    void sensorizacion();
    void obtener_datos_en_arreglo_2(char buffer[TAMANO_BUFFER_COMUNICACION], int * count);
    void mover_motor_camara(int angulo);
};

void obtener_datos_en_arreglo(CM700 * cm700, char buffer[TAMANO_BUFFER_COMUNICACION], int * count);
#endif // ROBOT_H
