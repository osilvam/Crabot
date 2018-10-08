#ifndef LEG_CPP
#define LEG_CPP
#include "leg.h"

Leg::Leg(CM700 * cm700, int _index, int _grados_libertad, int *angulos_iniciales, float *posicion_inicial, float _altura_inicial_patas)
{
    index = _index;
	altura_inicial_patas = _altura_inicial_patas;
    grados_libertad = _grados_libertad;
    Old_zNivel = 0;
    footOriginalPos[0] = posicion_inicial[0];
    footOriginalPos[1] = posicion_inicial[1];
    footOriginalPos[2] = posicion_inicial[2];
    
    this->cm700 = cm700;

	IDS = (int *)malloc(sizeof(int)*grados_libertad);
	Angulos_actuales = (int *)malloc(sizeof(int)*grados_libertad);
	Velocidades_finales = (int *)malloc(sizeof(int)*grados_libertad);
	Angulos_finales = (int *)malloc(sizeof(int)*grados_libertad);

    for(int i = 0; i < grados_libertad; i++){
        Angulos_actuales[i] = angulos_iniciales[i];
        Angulos_finales[i] = angulos_iniciales[i];
        Velocidades_finales[i] = VELOCIDAD_MOVIMIENTO;
        IDS[i] = grados_libertad*index + i + 1;
    }
	cm700->setLegPosition(IDS, Angulos_finales, Velocidades_finales);
	
}

void Leg::caminar(float *NewPosMove, int * exclude)
{    
    LegNewPos[0] = footOriginalPos[0] + NewPosMove[0];
    LegNewPos[1] = footOriginalPos[1] + NewPosMove[1];
    LegNewPos[2] = footOriginalPos[2] + NewPosMove[2] + Old_zNivel;
	cm700->getLegPosition(IDS,Angulos_actuales);
	int nuevos_IDS[Motores_enable(exclude)];
	int nuevos_Angulos_actuales[Motores_enable(exclude)];	
	int resultado_angulos[Motores_enable(exclude)];
	int resultado_velocidades[Motores_enable(exclude)];
	adaptar_valores(exclude, IDS, Angulos_actuales, nuevos_IDS, nuevos_Angulos_actuales);
    calcular_mover(index, altura_inicial_patas ,LegNewPos, nuevos_Angulos_actuales, nuevos_IDS, exclude, resultado_angulos, resultado_velocidades);    
    adaptar_resultados(exclude, resultado_angulos, resultado_velocidades, Angulos_finales, Velocidades_finales);
    cm700->setLegPosition(IDS, Angulos_finales, Velocidades_finales);
}

void Leg::rotar(float *NewPosRot, int * exclude)
{	
    float x = NewPosRot[0]*cos(index*2*M_PI/N_LEGS+NewPosRot[1]);
    float y = NewPosRot[0]*sin(index*2*M_PI/N_LEGS+NewPosRot[1]);
    
    LegNewPos[0] = x;
    LegNewPos[1] = y;
    LegNewPos[2] = NewPosRot[2]+Old_zNivel;    
    
    cm700->getLegPosition(IDS,Angulos_actuales);
    
	int nuevos_IDS[Motores_enable(exclude)];
	int nuevos_Angulos_actuales[Motores_enable(exclude)];	
	int resultado_angulos[Motores_enable(exclude)];
	int resultado_velocidades[Motores_enable(exclude)];
	
	adaptar_valores(exclude, IDS, Angulos_actuales, nuevos_IDS, nuevos_Angulos_actuales);		
	calcular_mover(index, altura_inicial_patas ,LegNewPos, nuevos_Angulos_actuales, nuevos_IDS, exclude, resultado_angulos, resultado_velocidades);	
	adaptar_resultados(exclude, resultado_angulos, resultado_velocidades, Angulos_finales, Velocidades_finales);	
	cm700->setLegPosition(IDS, Angulos_finales, Velocidades_finales);
}

void Leg::cambiar_nivel(float *NewNivel, int * exclude)
{
    LegNewPos[0] = NewNivel[0];
    LegNewPos[1] = NewNivel[1];
    LegNewPos[2] = NewNivel[2];
    
    cm700->getLegPosition(IDS,Angulos_actuales);
    
	int nuevos_IDS[Motores_enable(exclude)];
	int nuevos_Angulos_actuales[Motores_enable(exclude)];	
	int resultado_angulos[Motores_enable(exclude)];
	int resultado_velocidades[Motores_enable(exclude)];
	
	adaptar_valores(exclude, IDS, Angulos_actuales, nuevos_IDS, nuevos_Angulos_actuales);		
	calcular_mover(index, altura_inicial_patas ,LegNewPos, nuevos_Angulos_actuales, nuevos_IDS, exclude, resultado_angulos, resultado_velocidades);	
	adaptar_resultados(exclude, resultado_angulos, resultado_velocidades, Angulos_finales, Velocidades_finales);	
	cm700->setLegPosition(IDS, Angulos_finales, Velocidades_finales);
	
    Old_zNivel = LegNewPos[2];
}

void Leg::cambiar_nivel_inclinacion(float ori_x, float ori_y, float *NewNivel, int * exclude)
{
    float Old_position[3];
    LegNewPos[0] = NewNivel[0];
    LegNewPos[1] = NewNivel[1];
    LegNewPos[2] = NewNivel[2];
    
    cm700->getLegPosition(IDS,Angulos_actuales);
    
	int nuevos_IDS[Motores_enable(exclude)];
	int nuevos_Angulos_actuales[Motores_enable(exclude)];	
	int resultado_angulos[Motores_enable(exclude)];
	int resultado_velocidades[Motores_enable(exclude)];
	
	adaptar_valores(exclude, IDS, Angulos_actuales, nuevos_IDS, nuevos_Angulos_actuales);	
	calcular_mover_inclinacion(ori_x, ori_y, index, altura_inicial_patas ,LegNewPos, nuevos_Angulos_actuales, nuevos_IDS, exclude, resultado_angulos, resultado_velocidades);	
	adaptar_resultados(exclude, resultado_angulos, resultado_velocidades, Angulos_finales, Velocidades_finales);	
	cm700->setLegPosition(IDS, Angulos_finales, Velocidades_finales);
		
	int exclude_nil[] = EXCLUDE_NIL;
	kinematicPosition(index , altura_inicial_patas, Angulos_finales, &Old_position[0], &Old_position[1], &Old_position[2], exclude_nil);
    Old_zNivel = Old_position[2];
}

void Leg::cambiar_radio(float *NewMove, int sentido, int * exclude)
{
    if(NewMove != NULL){
        float x, y, z, orientation;
        if(sentido == 0)//aumentar radio
            orientation = index*2*M_PI/N_LEGS;
        else//disminuir radio
            orientation = index*2*M_PI/N_LEGS + M_PI;
        
        x = NewMove[0]*cos(orientation);
        y = NewMove[0]*sin(orientation);
        z = NewMove[1];
        LegNewPos[0] = footOriginalPos[0] + x;
        LegNewPos[1] = footOriginalPos[1] + y;
        LegNewPos[2] = footOriginalPos[2] + z + Old_zNivel;

        cm700->getLegPosition(IDS,Angulos_actuales);
        
	    int nuevos_IDS[Motores_enable(exclude)];
	    int nuevos_Angulos_actuales[Motores_enable(exclude)];	
	    int resultado_angulos[Motores_enable(exclude)];
	    int resultado_velocidades[Motores_enable(exclude)];
	
	    adaptar_valores(exclude, IDS, Angulos_actuales, nuevos_IDS, nuevos_Angulos_actuales);		
	    calcular_mover(index, altura_inicial_patas ,LegNewPos, nuevos_Angulos_actuales, nuevos_IDS, exclude, resultado_angulos, resultado_velocidades);
	    adaptar_resultados(exclude, resultado_angulos, resultado_velocidades, Angulos_finales, Velocidades_finales);
	    cm700->setLegPosition(IDS, Angulos_finales, Velocidades_finales);
	    
    }else{
        footOriginalPos[0] = LegNewPos[0];
        footOriginalPos[1] = LegNewPos[1];
        footOriginalPos[2] = LegNewPos[2] - Old_zNivel;
    }        
}

void Leg::posicion_trabajo(float * NewPosMove, int sentido, int posicion_trabajo, int * exclude)
{
    float orientation = NewPosMove[2] + sentido*M_PI/4;
    float x = NewPosMove[0]*cos(orientation);
    float y = NewPosMove[0]*sin(orientation);
    float z = NewPosMove[1];
    
    if(posicion_trabajo == 1){
        LegNewPosWork[0] = footOriginalPos[0] + x;
        LegNewPosWork[1] = footOriginalPos[1] + y;
        LegNewPosWork[2] = footOriginalPos[2] + z + Old_zNivel;
        
        for(int i = 0; i < 3; i++)
            Pas_LegNewPosWork[i] = LegNewPosWork[i];
            
    }else{
        LegNewPosWork[0] = Pas_LegNewPosWork[0] + x;
        LegNewPosWork[1] = Pas_LegNewPosWork[1] + y;
        LegNewPosWork[2] = Pas_LegNewPosWork[2] + z + Old_zNivel;
    }
    
    cm700->getLegPosition(IDS,Angulos_actuales);
    
    int nuevos_IDS[Motores_enable(exclude)];
    int nuevos_Angulos_actuales[Motores_enable(exclude)];    
    int resultado_angulos[Motores_enable(exclude)];
    int resultado_velocidades[Motores_enable(exclude)];

    adaptar_valores(exclude, IDS, Angulos_actuales, nuevos_IDS, nuevos_Angulos_actuales);	
    calcular_mover(index, altura_inicial_patas, LegNewPosWork, nuevos_Angulos_actuales, nuevos_IDS, exclude, resultado_angulos, resultado_velocidades);
    adaptar_resultados(exclude, resultado_angulos, resultado_velocidades, Angulos_finales, Velocidades_finales);
    cm700->setLegPosition(IDS, Angulos_finales, Velocidades_finales);
    
    LegNewPosWork[2] = LegNewPosWork[2] - Old_zNivel;
}

int Leg::Motores_enable(int * exclude)
{
    int count  = 0;
    
    for(int i = 0; i < GRADOS_LIBERTAD; i++){
        if(exclude[i] == 1)
            count++;
    }
    
    return count;
}

void Leg::adaptar_valores(int * exclude, int * IDS, int * Angulos_actuales, int * nuevos_IDS, int * nuevos_Angulos_actuales)
{    
	int indice = 0;
	
    for(int i = 0; i < GRADOS_LIBERTAD;i++){
	    if(exclude[i] == 1){
	        nuevos_IDS[indice] = IDS[i];
	        nuevos_Angulos_actuales[indice] = Angulos_actuales[i];
	        indice++;
	    }
	}
}
void Leg::adaptar_resultados(int * exclude, int * nuevos_angulos, int * nuevas_velocidades, int * angulos_originales, int * velocidades_originales)
{
    int indice = 0;
    
    for(int i = 0; i < GRADOS_LIBERTAD;i++){
        if(exclude[i] == 1){
            angulos_originales[i] = nuevos_angulos[indice];
            velocidades_originales[i] = nuevas_velocidades[indice];
            indice++;
        }
    }
}
#endif
