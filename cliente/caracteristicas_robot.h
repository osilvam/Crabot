#ifndef CARACTERISTICAS_ROBOT_H
#define CARACTERISTICAS_ROBOT_H
#include <cmath>
#include "tubos.hpp"

#define USE_USB2DXL
#define PUERTO_SERIAL "/dev/ttyUSB0"

#define _CRABOT
//#define _ARGO
//#define _ZBOT

#define _MX
//#define _AX

#ifdef _MX

//Caracteristicas_del_motor MX -----------------------------------------------------------------------------------------------------------------------------------
#define ANGULO_MAX (360.0*M_PI/180)
#define ANGULO_RESOLUCION 4095.0
#define VELOCIDAD_ANGULAR_MAX 11.938051
#define RESOLUCION_VELOCIDAD 1023
#define TRANSFORMAR_VELOCIDAD_A_NUMERO(X) (X*(RESOLUCION_VELOCIDAD/VELOCIDAD_ANGULAR_MAX))
#define TRANSFORMAR_NUMERO_A_VELOCIDAD(X) (X*(VELOCIDAD_ANGULAR_MAX/RESOLUCION_VELOCIDAD))
#define TRANSFORMAR_A_RADIANES(X) (X*(ANGULO_MAX/ANGULO_RESOLUCION))
#define TRANSFORMAR_A_NUMEROS(X) (X*(ANGULO_RESOLUCION/ANGULO_MAX))
#define ANGULOCERO 180
#define TAMANO_BUFFER_COMUNICACION 10000
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
#else
//Caracteristicas_del_motor AX -----------------------------------------------------------------------------------------------------------------------------------
#define ANGULO_MAX (300.0*M_PI/180)
#define ANGULO_RESOLUCION 1023.0
#define VELOCIDAD_ANGULAR_MAX 11.938051
#define RESOLUCION_VELOCIDAD 1023
#define TRANSFORMAR_VELOCIDAD_A_NUMERO(X) (X*(RESOLUCION_VELOCIDAD/VELOCIDAD_ANGULAR_MAX))
#define TRANSFORMAR_NUMERO_A_VELOCIDAD(X) (X*(VELOCIDAD_ANGULAR_MAX/RESOLUCION_VELOCIDAD))
#define TRANSFORMAR_A_RADIANES(X) (X*(ANGULO_MAX/ANGULO_RESOLUCION))
#define TRANSFORMAR_A_NUMEROS(X) (X*(ANGULO_RESOLUCION/ANGULO_MAX))
#define ANGULOCERO 150
#define TAMANO_BUFFER_COMUNICACION 10000
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif


#ifdef _CRABOT
//Características del robot--------------------------------------------------------------------------------------------------------------------------------------
#define CONFIG_ROB  "{'nombre' : 'CRABOT','autores' : ['Oscar Silva','Yen-kung Yu','Tenison','Andrés'],'numPatas' : 6,'numArticulaciones' : 4,'grafico' : [{'titulo' : 'Temperatura','valorMax' : 90,'valorMin' : 0,'unidad' : 'ºC'},{'titulo' : 'Corriente','valorMax' : 10,'valorMin' : 0,'unidad' : 'A'}],'control' : [{'titulo' : 'Mover','idGradosLibertad' : 0},{'titulo' : 'Nivelar','idGradosLibertad' : 5},{'titulo' : 'Girar','idGradosLibertad' : 0},{'titulo' : 'Inclinar','idGradosLibertad' : 0},{'titulo' : 'Kinect','idGradosLibertad' : 0}]}"
#define GRA (M_PI/180)
#define N_LEGS 6
#define GRADOS_LIBERTAD 4
#define STEP 5
#define VSTEP 15
#define INTERMODULEDELAY 4
#define CM0 4
#define CM1 16
#define MODULE_POS {0,3,1,5,2,4}
#define ANGULOS_INICIALES {(int)(TRANSFORMAR_A_NUMEROS(180*GRA)),(int)(TRANSFORMAR_A_NUMEROS(180*GRA)),(int)(TRANSFORMAR_A_NUMEROS(225*GRA)),(int)(TRANSFORMAR_A_NUMEROS(145*GRA))}
#define CANTIDAD_PASOS_ROTAR 6
#define PASOS_BAJAR 24
#define PASOS_SUBIR 6
#define Z_REINCORPORACION (PASOS_BAJAR + PASOS_SUBIR)
#define Z_DESCANSO 30

#define FACTOR_ALTURA_MAXIMA 1.2 //con respecto a la altura normal del robot   
#define FACTOR_ALTURA_MINIMA 0.8
#define VSTEP_DOWN 1 // mm
#define VSTEP_UP 1
#define ALTURA_DESCANSO 40
#define INCLINACION_X_MAX (10*GRA)
#define INCLINACION_Y_MAX (10*GRA)
#define STEP_INCLINACION 1 // de 1 a 10

#define VELOCIDAD_MOVIMIENTO 900
#define TIEMPO_LIMITE_DE_ESPERA_A_ALCANZAR_UN_PUNTO_SEG 0.051
#define ERROR_ACEPTABLE_COMPARADOR 0.1
#define ANGULO_IDENTIFICADOR_INICIAL 0
#define EXCLUDE_NIL {1,1,1,1}
#define EXCLUDE_ALL {0,0,0,0}
#define EXCLUDE_MOVE {0,1,1,1}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//DIMENSIONES FÍSICAS CRABOT--------------------------------------------------------------------------------------------------------------------------------------
#define RADIO 126
#define DISTANCIA_0 59
#define DISTANCIA_1 78
#define DISTANCIA_2 107
#define DISTANCIA_3 32
#define DISTANCIA_4 232
#define DISTANCIA_5 0
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif



#ifdef _ARGO
//Características del robot-----------------------------------------------------------------------------------------------------------------------------------------
#define CONFIG_ROB  "{'nombre' : 'ARGO','autores' : ['1','2','3','4'],'numPatas' : 4,'numArticulaciones' : 3,'grafico' : [{'titulo' : 'Temperatura','valorMax' : 90,'valorMin' : 0,'unidad' : 'ºC'},{'titulo' : 'Corriente','valorMax' : 10,'valorMin' : 0,'unidad' : 'A'}],'control' : [{'titulo' : 'Mover','idGradosLibertad' : 0},{'titulo' : 'Nivelar','idGradosLibertad' : 5},{'titulo' : 'Girar','idGradosLibertad' : 0},{'titulo' : 'Inclinar','idGradosLibertad' : 0},{'titulo' : 'Kinect','idGradosLibertad' : 0}]}"
#define GRA (M_PI/180)
#define N_LEGS 4
#define GRADOS_LIBERTAD 3
#define STEP 4
#define VSTEP 7
#define INTERMODULEDELAY 8
#define CM0 4
#define CM1 24
#define MODULE_POS {0,2,1,3}
#define ANGULOS_INICIALES {(int)(TRANSFORMAR_A_NUMEROS(150*GRA)),(int)(TRANSFORMAR_A_NUMEROS(180*GRA)),(int)(TRANSFORMAR_A_NUMEROS(120*GRA))}
#define CANTIDAD_PASOS_ROTAR 6
#define PASOS_BAJAR 20
#define PASOS_SUBIR 15
#define VELOCIDAD_MOVIMIENTO 900
#define TIEMPO_LIMITE_DE_ESPERA_A_ALCANZAR_UN_PUNTO_SEG 0.051
#define ERROR_ACEPTABLE_COMPARADOR 0.00001
#define ANGULO_IDENTIFICADOR_INICIAL (45*GRA)
#define EXCLUDE_NIL {1,1,1}
#define EXCLUDE_ALL {0,0,0}
#define EXCLUDE_MOVE {1,1,1}

#define FACTOR_ALTURA_MAXIMA 1.2 //con respecto a la altura normal del robot   
#define FACTOR_ALTURA_MINIMA 0.8
#define VSTEP_DOWN 1 // mm
#define VSTEP_UP 1
#define ALTURA_DESCANSO 40
#define INCLINACION_X_MAX (10*GRA)
#define INCLINACION_Y_MAX (10*GRA)
#define STEP_INCLINACION 1 // de 1 a 10

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//DIMENSIONES FÍSICAS ARGO-----------------------------------------------------------------------------------------------------------------------------------------------
#define RADIO 112
#define DISTANCIA_0 0
#define DISTANCIA_1 54
#define DISTANCIA_2 64
#define DISTANCIA_3 15
#define DISTANCIA_4 114
#define DISTANCIA_5 100
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif

#ifdef _ZBOT
#define CONFIG_ROB  "{'nombre' : 'CRABOT','autores' : ['Oscar Silva','Yen-kung Yu','Tenison','Andrés'],'numPatas' : 4,'numArticulaciones' : 3,'grafico' : [{'titulo' : 'Temperatura','valorMax' : 90,'valorMin' : 0,'unidad' : 'ºC'},{'titulo' : 'Corriente','valorMax' : 10,'valorMin' : 0,'unidad' : 'A'}],'control' : [{'titulo' : 'Mover','idGradosLibertad' : 0},{'titulo' : 'Nivelar','idGradosLibertad' : 5},{'titulo' : 'Girar','idGradosLibertad' : 0},{'titulo' : 'Inclinar','idGradosLibertad' : 0},{'titulo' : 'Kinect','idGradosLibertad' : 0}]}"
//Características del robot-----------------------------------------------------------------------------------------------------------------------------------------
#define GRA (M_PI/180)
#define N_LEGS 4
#define GRADOS_LIBERTAD 3
#define STEP 4
#define VSTEP 10
#define INTERMODULEDELAY 8
#define CM0 4
#define CM1 24
#define MODULE_POS {0,2,1,3}
#define ANGULOS_INICIALES {(int)(TRANSFORMAR_A_NUMEROS(180*GRA)),(int)(TRANSFORMAR_A_NUMEROS(210*GRA)),(int)(TRANSFORMAR_A_NUMEROS(140*GRA))}
#define CANTIDAD_PASOS_ROTAR 6
#define PASOS_BAJAR 20
#define PASOS_SUBIR 15
#define VELOCIDAD_MOVIMIENTO 900
#define TIEMPO_LIMITE_DE_ESPERA_A_ALCANZAR_UN_PUNTO_SEG 0.051
#define ERROR_ACEPTABLE_COMPARADOR 0.5
#define ANGULO_IDENTIFICADOR_INICIAL (45*GRA)
#define EXCLUDE_NIL {1,1,1}
#define EXCLUDE_ALL {0,0,0}
#define EXCLUDE_MOVE {1,1,1}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//DIMENSIONES FÍSICAS ZBOT-----------------------------------------------------------------------------------------------------------------------------------------------
#define RADIO 193
#define DISTANCIA_0 0
#define DISTANCIA_1 72
#define DISTANCIA_2 102
#define DISTANCIA_3 0
#define DISTANCIA_4 140
#define DISTANCIA_5 100

#define FACTOR_ALTURA_MAXIMA 1.2 //con respecto a la altura normal del robot
#define FACTOR_ALTURA_MINIMA 0.8
#define VSTEP_DOWN 1 // mm
#define VSTEP_UP 1
#define ALTURA_DESCANSO 40
#define INCLINACION_X_MAX (10*GRA)
#define INCLINACION_Y_MAX (10*GRA)
#define STEP_INCLINACION 1 // de 1 a 10

//------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif

//COMUNICACION SERIAL-----------------------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

//Linea-------------------------------------------------------------------------------------------------------------------------------------------------------------
#define LARGO_ENTRE_PUNTOS_MILIMETROS 1
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define DELTA_DERIVATIVE_FOR_JACOBIAN  0.00001

#define TIEMPO_ESPERA_EM_POLL_MILISECONDS 1
#define TIME_OUT_POLL_MSEC 10

/* Default baudrate */
#define AX_DEF_BAUDRATE B1000000

//-----------------------------------------------------------------------------------CM700-----------------------------
#define DEF_TIMEOUT		100000
#define SETPOSNDVEL		0x02
#define ASKPOSNDVEL		0x04
#define ASKDATACTID		0x08
#define ASKSENSOR		0x10
#define RPYPOSNVEL		0x40
#define RPYDATAID		0x41
#define SETTORQUE		0x80
#define SETTORQEXID		0x81
#define ERRORMOVING		2
#define ERRORREAD		3
#define _L16(x)			((x >> 0) & 0xFF)
#define _H16(x)			((x >> 8) & 0xFF)
#define _MW(x, y)		(y * 256 + x)

//
//----------------------------------------------------------------------------------------------------------------------

#endif
