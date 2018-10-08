#include "robot.h"


Robot::Robot(CM700 * cm700, int * _modulePos, tubos * todos_los_tubos)
{
    this->cm700 = cm700;
    this->todos_los_tubos = todos_los_tubos;
    cm700->refreshAll();
    //int _modulePos[] = MODULE_POS;
    int exclude_nil[] = EXCLUDE_NIL;
    int angulos_iniciales[] = ANGULOS_INICIALES;
    n_legs = N_LEGS;    
    radio = obtener_radio(angulos_iniciales);
    grados_libertad = GRADOS_LIBERTAD;
    interModuleDelay = INTERMODULEDELAY;
    tableLength = (n_legs - 1)*interModuleDelay + 1;
    modulePos = _modulePos;
    xMovementTable = std::vector<float> (tableLength,0);
    yMovementTable = std::vector<float> (tableLength,0);
    zMovementTable = std::vector<float> (tableLength,0);
    rMovementTable = std::vector<float> (tableLength,0);
    rRotationTable = std::vector<float> (tableLength,radio);
    thetaRotationTable = std::vector<float> (tableLength,0);
    xNivelTable = (float *)malloc(sizeof(float)*n_legs);
    yNivelTable = (float *)malloc(sizeof(float)*n_legs);
    zNivelTable = (float *)malloc(sizeof(float)*n_legs);
    posTipx = (float *)malloc(sizeof(float)*n_legs);
    posTipy = (float *)malloc(sizeof(float)*n_legs);
    posTipz = (float *)malloc(sizeof(float)*n_legs);
    zTipMove = (float *)malloc(sizeof(float)*n_legs);
	altura_inicial_patas =  altura_inicial_patas_f(angulos_iniciales);
	
	#ifdef _ZBOT
	ventosas = (int *)malloc(sizeof(int)*n_legs);
	iterador_ventosas = -1;
	#endif
	
    for(int i  = 0; i < n_legs; i++){
		kinematicPosition(i, altura_inicial_patas, angulos_iniciales, &posTipx[i], &posTipy[i], &posTipz[i], exclude_nil);
        float pos_actual[3] = {posTipx[i],posTipy[i],posTipz[i]};
        Legs.push_back(Leg(cm700,i, grados_libertad, angulos_iniciales, pos_actual, altura_inicial_patas));
        xNivelTable[i] = 0;
        yNivelTable[i] = 0;
        zNivelTable[i] = 0;
		
		#ifdef _ZBOT
		ventosas[i] = 1;
		#endif
    }
	
	#ifdef _ZBOT
	//actuador_ventosas(ventosas);//no implementada
	#endif
	
    cm700->moveAll();

    vstep_down = VSTEP_DOWN;
    vstep_up = VSTEP_UP;
    phase = 0;
    rot = 0;
    r = 0;
    z = 0;
    theta = 0;
    c = 8;
    cm0 = CM0;
    cm1 = CM1;
    aux = 0;
    //----caminar------------------------//
    direction = 0;
    forwardVel = 1;
    step = STEP;
    vstep = VSTEP;
    //-----------------------------------//
    //----rotar--------------------------//
    rotar_pasos = CANTIDAD_PASOS_ROTAR;
    rotar_angle = ((2*M_PI/n_legs)/rotar_pasos)/cm1;
    //-----------------------------------//
    //----inclinacion--------------------//
    ori_x = 0;
    ori_y = 0;
    MAX_ori_x = INCLINACION_X_MAX;
    MAX_ori_y = INCLINACION_Y_MAX;
    //-----------------------------------//
    relajado = false;
    
    work_area = 0;

    workstep = /*sqrt(2)**/(radio - RADIO - DISTANCIA_0)/(2*CM1);
    

}

void Robot::detener(int* exclude)
{
    float NewPos[3];
    r = z = 0;
    phase = rot = 0;
    c = cm1/2;
    aux = 0;
    xMovementTable = std::vector<float> (tableLength,0);
    yMovementTable = std::vector<float> (tableLength,0);
    zMovementTable = std::vector<float> (tableLength,0);
    rMovementTable = std::vector<float> (tableLength,0);
    rRotationTable = std::vector<float> (tableLength,radio);
    thetaRotationTable = std::vector<float> (tableLength,0);
    cm700->refreshAll();
    for(int i = 0; i < n_legs; i++){
        NewPos[0] = xMovementTable[modulePos[i]*interModuleDelay];
        NewPos[1] = yMovementTable[modulePos[i]*interModuleDelay];
        NewPos[2] = zMovementTable[modulePos[i]*interModuleDelay];
        Legs[i].caminar(NewPos, exclude);
    }
	#ifdef _ZBOT
	for(int i = 0; i < n_legs; i++)
		ventosas[i] = 1;
	iterador_ventosas = -1;
	//actuador_ventosas(ventosas);//no implementada
	#endif
    cm700->moveAll();
}

void Robot::caminar(float direction, int * exclude)
{
    float NewPos[3];
    if(phase == 2){
        r=r+step*(8/cm0);
        z=z-vstep;
        c=c+1;
        if(c>=cm0){
            phase=0;
            c=0;
        }
    }
    if(phase==1){
        r=r+step*(8/cm0);
        z=z+vstep;
        c=c+1;
        if (c>=cm0){
            phase=2;
            c=0;
        }
    }
    if (phase==0){
        r=r-(16.0/cm1)*step;
        c=c+1;
        if (c>=cm1){
            phase=1;
            c=0;
			#ifdef _ZBOT
			if(iterador_ventosas == 3)
				iterador_ventosas = 0;
			else
				iterador_ventosas++;
			for(int i = 0; i < n_legs; i++){
				if(iterador_ventosas == i)
					ventosas[i] = 0;
				else
					ventosas[i] = 1;
			}
			//actuador_ventosas(ventosas);//no implementada
			#endif
        }
    }

    float x = forwardVel*r*cos(direction);
    float y = forwardVel*r*sin(direction);

    xMovementTable.pop_back();
    yMovementTable.pop_back();
    zMovementTable.pop_back();
    xMovementTable.insert(xMovementTable.begin(),x);
    yMovementTable.insert(yMovementTable.begin(),y);
    zMovementTable.insert(zMovementTable.begin(),z);


    /*    int * contador = (int *) malloc(sizeof(int));
    cm700->refreshAll();

    *contador = TAMANO_BUFFER_COMUNICACION;
    char buffer[TAMANO_BUFFER_COMUNICACION];
    cm700->obtener_datos_en_arreglo(buffer, contador);
    write(todos_los_tubos->pipe_MR_to_CS[1], buffer, *contador+1);
    delete(contador);
*/
    sensorizacion();

    for(int i = 0; i < n_legs; i++){
        NewPos[0] = xMovementTable[modulePos[i]*interModuleDelay];
        NewPos[1] = yMovementTable[modulePos[i]*interModuleDelay];
        NewPos[2] = zMovementTable[modulePos[i]*interModuleDelay];
        Legs[i].caminar(NewPos, exclude);
    }
    cm700->moveAll();
}

void Robot::girar_izquierda(int * exclude)
{
    float NewPos[3];

    if(rot == 2){
        theta = theta + 2.0*rotar_angle;
        z=z-vstep;
        c=c+1	;
        if(c>=cm0){
            rot=0;
            c=0;
        }
    }
    if(rot == 1){
        theta = theta + 2.0*rotar_angle;
        z=z+vstep;
        c=c+1;
        if(c>=cm0){
            rot=2;
            c=0;
        }
    }
    if(rot == 0){
        theta = theta - (16.0/cm1)*rotar_angle;
        c=c+1;
        if(c>=cm1){
            rot=1;
            c=0;
        }
    }

    rRotationTable.pop_back();
    thetaRotationTable.pop_back();
    zMovementTable.pop_back();
    rRotationTable.insert(rRotationTable.begin(),radio);
    thetaRotationTable.insert(thetaRotationTable.begin(),theta);
    zMovementTable.insert(zMovementTable.begin(),z);

    cm700->refreshAll();
    for(int i = 0; i < n_legs; i++){
        NewPos[0] = rRotationTable[modulePos[i]*interModuleDelay];
        NewPos[1] = thetaRotationTable[modulePos[i]*interModuleDelay];
        NewPos[2] = zMovementTable[modulePos[i]*interModuleDelay];
        Legs[i].rotar(NewPos, exclude);
    }
    cm700->moveAll();
}

void Robot::girar_derecha(int * exclude)
{
    float NewPos[3];

    if(rot == 2){
        theta = theta - 2.0*rotar_angle;
        z=z-vstep;
        c=c+1	;
        if(c>=cm0){
            rot=0;
            c=0;
        }
    }
    if(rot == 1){
        theta = theta - 2.0*rotar_angle;
        z=z+vstep;
        c=c+1;
        if(c>=cm0){
            rot=2;
            c=0;
        }
    }
    if(rot == 0){
        theta = theta + (16.0/cm1)*rotar_angle;
        c=c+1;
        if(c>=cm1){
            rot=1;
            c=0;
        }
    }

    rRotationTable.pop_back();
    thetaRotationTable.pop_back();
    zMovementTable.pop_back();
    rRotationTable.insert(rRotationTable.begin(),radio);
    thetaRotationTable.insert(thetaRotationTable.begin(),theta);
    zMovementTable.insert(zMovementTable.begin(),z);

    cm700->refreshAll();
    for(int i = 0; i < n_legs; i++){
        NewPos[0] = rRotationTable[modulePos[i]*interModuleDelay];
        NewPos[1] = thetaRotationTable[modulePos[i]*interModuleDelay];
        NewPos[2] = zMovementTable[modulePos[i]*interModuleDelay];
        Legs[i].rotar(NewPos, exclude);
    }
    cm700->moveAll();
}

int Robot::bajar(bool descanso, int magnitud, int * exclude)
{
    float NewPos[3];
    static int cantidad_bajada = 0;
    if(aux == 0){
        obtener_posicion_actual();
        int SUMA_zNivel = 0;
        for(int i = 0; i < n_legs; i++){
            xNivelTable[i] = posTipx[i];
            yNivelTable[i] = posTipy[i];
            zNivelTable[i] = posTipz[i];
            SUMA_zNivel = SUMA_zNivel + zNivelTable[i];
        }        
        if(descanso){
            cantidad_bajada = abs(altura_inicial_patas- ALTURA_DESCANSO); 
        }
        else{
            cantidad_bajada = altura_inicial_patas*(magnitud/8.0)*(FACTOR_ALTURA_MAXIMA - FACTOR_ALTURA_MINIMA);
        }   
        aux = 1;
    }
    
    if(cantidad_bajada >= 0){
        cm700->refreshAll();
        for(int i = 0; i < n_legs; i++){
            zNivelTable[i] = zNivelTable[i] + VSTEP_DOWN;
            NewPos[0] = xNivelTable[i];
            NewPos[1] = yNivelTable[i];
            NewPos[2] = zNivelTable[i];
            Legs[i].cambiar_nivel(NewPos, exclude);
        }
        cm700->moveAll();
        cantidad_bajada = cantidad_bajada - VSTEP_DOWN;
        return 1;
    }else
        return 0;
}

int Robot::levantar(bool reincorporar, int magnitud, int * exclude)
{
    float NewPos[3];    
    static int cantidad_subida = 0;
    if(aux == 0){
        obtener_posicion_actual();   
        int SUMA_zNivel = 0;  
        for(int i = 0; i < n_legs; i++){
            xNivelTable[i] = posTipx[i];
            yNivelTable[i] = posTipy[i];
            zNivelTable[i] = posTipz[i];
            SUMA_zNivel+=zNivelTable[i];
        }   
        if(reincorporar){
            cantidad_subida = abs(altura_inicial_patas - ALTURA_DESCANSO);      
        }else{  
            cantidad_subida = altura_inicial_patas*(magnitud/8.0)*(FACTOR_ALTURA_MAXIMA - FACTOR_ALTURA_MINIMA) ;        
        }       
        aux = 1;
    }
    if(cantidad_subida >= 0){
        cm700->refreshAll();
        for(int i = 0; i < n_legs; i++){
            zNivelTable[i] = zNivelTable[i] - VSTEP_UP;
            NewPos[0] = xNivelTable[i];
            NewPos[1] = yNivelTable[i];
            NewPos[2] = zNivelTable[i];
            Legs[i].cambiar_nivel(NewPos,  exclude);
        }
        cm700->moveAll();        
        cantidad_subida = cantidad_subida - VSTEP_UP;
        return 1;
    }else
        return 0;    
}

int Robot::cambiar_inclinacion(int direccion, int magnitud, int * exclude)
{
    float NewPos[3];
    static float dif_ori_x = 0;
    static float dif_ori_y = 0;     
    static float new_ori_x = 0;
    static float new_ori_y = 0;
    static bool out_x = false;
    static bool out_y = false;        
    if(aux == 0){
        obtener_posicion_actual_inclinacion();
        out_x = false;
        out_y = false;
        new_ori_x = (float)MAX_ori_x*(magnitud/4.0)*cos(direccion*GRA);
        new_ori_y = (float)MAX_ori_y*(magnitud/4.0)*sin(direccion*GRA);
        dif_ori_x = new_ori_x - ori_x;
        dif_ori_y = new_ori_y - ori_y;
        aux = 1;
    }
    
    if(dif_ori_x < 0 && ori_x > new_ori_x + 0.00001)
        ori_x = ori_x - abs(dif_ori_x)*STEP_INCLINACION/30.0;
    else
        if(dif_ori_x > 0 && ori_x < new_ori_x - 0.00001)
            ori_x = ori_x + abs(dif_ori_x)*STEP_INCLINACION/30.0;
        else
            out_x = true;
    if(dif_ori_y < 0 && ori_y > new_ori_y + 0.00001)
        ori_y = ori_y - abs(dif_ori_y)*STEP_INCLINACION/30.0;
    else
        if(dif_ori_y > 0 && ori_y < new_ori_y - 0.00001)
            ori_y = ori_y + abs(dif_ori_y)*STEP_INCLINACION/30.0;    
        else
            out_y = true;
    if(out_x && out_y) return 0;
    cm700->refreshAll();
    for(int i = 0; i < n_legs; i++){
        NewPos[0] = posTipx[i];
        NewPos[1] = posTipy[i];
        NewPos[2] = posTipz[i];
        Legs[i].cambiar_nivel_inclinacion(ori_x, ori_y, NewPos,  exclude);
    }
    cm700->moveAll();
    return 1;
    
}


int Robot::orientarse_objeto(float object_position, int * exclude)
{
    if(aux == 0){
        work_area = floor(object_position/(2*M_PI/n_legs));
        aux_position = object_position - work_area*(2*M_PI/n_legs) - (M_PI/n_legs);
        aux = 1;
    }
    
    if(aux_position < -0.00001){
        girar_derecha( exclude);
        if(rot == 1 || rot == 2)
            aux_position = aux_position + 2*rotar_angle;
        
    }else{
        if(aux_position > 0.00001){
            girar_izquierda( exclude);
            if(rot == 1 || rot == 2)
            aux_position = aux_position - 2*rotar_angle;            
        }else
            return 0;
    }
    return 1;
}

int Robot::posicion_trabajo(int posicion_trabajo, int * exclude)
{
    float NewPos[3];
    if(aux == 0){
        phase = 1;
        c = 0;
        r = z = 0;
        if(work_area < 4)
            pata_1 = 2 + work_area;
        else
            pata_1 = 2 + work_area - 6;
        if(work_area < 1)
            pata_2 = 5 + work_area;
        else
            pata_2 = 5 + work_area - 6;
            orientation = work_area*(2*M_PI/n_legs) + M_PI/n_legs;
            aux = 1;
    }
    cm700->refreshAll();
    if(phase == 2){
        r = r + workstep*2;
        z = z - vstep;
        c++;
        NewPos[0] = r;
        NewPos[1] = z;
        NewPos[2] = orientation + (1-posicion_trabajo)*M_PI;
        
        Legs[pata_1].posicion_trabajo( NewPos, -1, posicion_trabajo, exclude);
        Legs[pata_2].posicion_trabajo( NewPos, 1, posicion_trabajo, exclude);
        cm700->moveAll();
        if(c >= cm0){
            c = 0; 
            phase = 1;
            aux = 0;
            return 0;           
        }
    }
    if(phase == 1){
        r = r + workstep*2;
        z = z + vstep;
        c++;
        NewPos[0] = r;
        NewPos[1] = z;
        NewPos[2] = orientation + (1-posicion_trabajo)*M_PI;
        
        Legs[pata_1].posicion_trabajo( NewPos, -1, posicion_trabajo, exclude);
        Legs[pata_2].posicion_trabajo( NewPos, 1, posicion_trabajo, exclude);
        cm700->moveAll();
        if(c >= cm0){
            phase = 2;
            c = 0;
        }
    }
    usleep(5000);
    return 1;    
    
}

int Robot::cambiar_radio( int sentido, int * exclude)
{
    float NewMove[2]; 
    if(aux == 0){
        phase = 1;
        r = 0;
        z = 0;
        c = 0;
        aux = 1;
    }
    if(phase == 3){
        c = c + 1;
    }
    if(phase == 2){
        r = r + step;
        z = z - vstep;
        c = c + 1;
        if(c>=cm0){
            phase = 3;
            c = 0;
        }
    }
    if(phase == 1){
        r = r + step;
        z = z + vstep;
        c = c + 1;
        if(c>=cm0){
            phase = 2;
            c = 0;
        }
    }
    rMovementTable.pop_back();
    zMovementTable.pop_back();
    rMovementTable.insert(rMovementTable.begin(),r);
    zMovementTable.insert(zMovementTable.begin(),z);
    cm700->refreshAll();
    for(int i = 0; i < n_legs; i++){
        NewMove[0] = rMovementTable[modulePos[i]*interModuleDelay];
        NewMove[1] = zMovementTable[modulePos[i]*interModuleDelay];
        Legs[i].cambiar_radio(NewMove, sentido, exclude);
    }
    cm700->moveAll();
    if(c>=tableLength){
        for(int i = 0; i < n_legs; i++){
            Legs[i].cambiar_radio(NULL, sentido, exclude);
        }
        cm700->moveAll();
        return 0;
    }else{
        if(sentido == 0)//aumentar radio
            radio = radio + 8*step;
        else//disminuir radio
            radio = radio - 8*step;
        return 1;
    }
        
}

void Robot::relajar(int * exclude)
{
    detener(exclude);
    while(bajar(true, 9, exclude));
    cm700->setTorque(false);
	relajado = true;
}

void Robot::reincorporar(int * exclude)
{
    if(relajado){
        cm700->moveAll();
        detener(exclude);
        while(levantar(true, 5, exclude));
	    detener(exclude);
	    relajado = false;
	}
}

void Robot::obtener_posicion_actual()
{  
	int angulos_aux[grados_libertad];
	int ids[grados_libertad];
    cm700->refreshAll();
    for(int i=0; i<n_legs;i++){
        for(int j = 0; j < grados_libertad; j++)
            ids[j] = (j+1) + grados_libertad*i;
        cm700->getLegPosition(ids, angulos_aux);
        int exclude_nil[] = EXCLUDE_NIL;
        kinematicPosition(i, altura_inicial_patas, angulos_aux, &posTipx[i], &posTipy[i], &posTipz[i], exclude_nil);
    }
}

void Robot::obtener_posicion_actual_inclinacion()
{  
	int angulos_aux[grados_libertad];
	int ids[grados_libertad];
    cm700->refreshAll();
    for(int i=0; i<n_legs;i++){
        for(int j = 0; j < grados_libertad; j++)
            ids[j] = (j+1) + grados_libertad*i;
        cm700->getLegPosition(ids, angulos_aux);
        int exclude_nil[] = EXCLUDE_NIL;
        kinematicPosition_inclinacion(ori_x, ori_y, i, altura_inicial_patas, angulos_aux, &posTipx[i], &posTipy[i], &posTipz[i], exclude_nil);
    }
}

float Robot::obtener_radio(int * angulos_iniciales)
{
    float posPata[3];
    int exclude_nil[] = EXCLUDE_NIL;
	kinematicPosition(0, 0, angulos_iniciales, &posPata[0], &posPata[1], &posPata[2], exclude_nil);
    return sqrt(posPata[0]*posPata[0] + posPata[1]*posPata[1]);
}

float Robot::altura_inicial_patas_f(int * angulos_iniciales)
{
	float posPata[3];
	int exclude_nil[] = EXCLUDE_NIL;
	kinematicPosition(0, 0, angulos_iniciales, &posPata[0], &posPata[1], &posPata[2], exclude_nil);
    return -1.0*posPata[2];
}

void Robot::sensorizacion()
{
    int * contador = (int *) malloc(sizeof(int));
    cm700->refreshAll();
    *contador = TAMANO_BUFFER_COMUNICACION;
    char buffer[TAMANO_BUFFER_COMUNICACION];
    cm700->obtener_datos_en_arreglo(buffer, contador);
    write(todos_los_tubos->pipe_MR_to_CS[1], buffer, *contador + 1);
    delete(contador);
}

