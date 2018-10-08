
//	+ ------------------------------------ +	id 0 : Visualizador Izquierdo
//	|             Menú principal           |	id 1 : Visualizador Derecho
//	+ ------- + ---------------- + ------- +	id 2 : Especificador (mover, girar, inclinar, ...)
//	|         |                  |         |	id 3 : Estado (relajar, recuperar, detener)
//	|         |                  |         |	id 4 : Direccionador
//	|   id 0  |   Video Stream   |   id 1  |	
//	|         |                  |         |	Grados de libertad:
//	|         |                  |         |	0 : Todos
//	|         |                  |         |	1 : Arriba-Abajo
//	+ ------- + ---------------- + ------- +	2 : Derecha-Izquierda
//	|         |                  |         |
//	|         |                  |         |
//	|   id 2  |      id 3        |   id 4  |	| alto 					
//	|         |                  |         |	- ancho
//	+ ------- + ---------------- + ------- +

// GENERAL =====================================================================

	// Rutas de archivos (posiciones relativas)
	var rutaConfig 			= 'config';
	var rutaLector 			= 'lector.php';
	var rutaEjecutor		= 'ejecutor.php';
	var rutaTemperatura 	= 'temperatura';
	var rutaCorriente		= 'corriente';

	// Archivos para lecturas de datos
	var archivos = new Array();
	archivos[0] = rutaTemperatura;
	archivos[1] = rutaCorriente;

	// Configuración general del robot
	var nombreRobot;
	var autores = new Array();
	var numeroMotores;
	var numeroPatas;
	var numeroArticulaciones;

	// Configuración general de los datos 
	var numeroBotonesEspecificador;
	var botonesEspecificador = new Array();								// {Mover, C, 0} , {Girar, G, 5}, ...
	var idAccionActual;

	// Configuración general de las cantidades medidas
	var titulo 			= new Array();
	var valorMaximo 	= new Array();
	var valorMinimo 	= new Array();
	var rangoValores	= new Array();									// Rango Valores = Valor Máximo - Valor Mínimo
	var unidad 			= new Array();

	// Arreglo que contiene todos los Objetos (id: 0, 1, 2, 3, 4)
	var canvas = new Array();

	// Tiempo de refresco de los diagramas ( por defecto 1000 [ms] )
	var tiempoLoopDiagramaIzquierdo = 1000; 
	var tiempoLoopDiagramaDerecho 	= 1000; 

// VISUALIZADOR ================================================================

	var anchoVisualizador;
	var altoVisualizador;

	function Visualizador () {

		// Propiedades del Canvas
		this.ancho 	= anchoVisualizador;
		this.alto 	= altoVisualizador;

		// Configuración Tipo Barra - Rejillas
		this.paddingLR					= this.ancho * 0.15; 					// Padding Left-Rigth
		this.paddingT 					= this.ancho * 0.25; 					// Padding Top
		this.paddingB 					= this.ancho * 0.1; 					// Padding Bottom
		this.anchoRejlla 				= this.ancho - 2 * this.paddingLR;
		this.altoRejilla 				= this.alto - (this.paddingT + this.paddingB);	
		this.numeroDivisionesRejilla 	= 3; 									// Número divisiones rejilla (>= 1)
		this.anchoDivisionesVertical 	= this.anchoRejlla / this.numeroDivisionesRejilla;

		// Configuración Tipo Barra - Barras
		this.anchoDivisionHorizontal 	= this.altoRejilla / numeroMotores;		// Alto rejilla / Número motores
		this.altoBarras 				= this.anchoDivisionHorizontal / 2;		// Ancho divisiones horizontales / 2
		this.offset 					= this.altoBarras / 2;					// Ancho barras / 2

		// Configuración Tipo Torta - Torta
		this.centro 					= { x : this.ancho / 2, y : this.alto / 2 };
		this.radio 						= (this.ancho - 2 * this.paddingLR) / 2; 

		// Configuración Tipo Torta - Leyenda
		this.anchoLeyenda 				= this.ancho - 2 * this.paddingLR;
		this.altoLeyenda 				= this.paddingB / 2; 					
		this.numeroDivisionesLeyenda 	= 3;
		this.anchoDivisionLeyenda 		= this.anchoLeyenda / this.numeroDivisionesLeyenda;
		this.posicionLeyendaY 			= this.alto - this.paddingB - this.altoLeyenda;
		this.posicionLeyendaX 			= this.paddingLR;

		// Varios ...
		this.dibujarBarra = _dibujarBarra;
		this.dibujarTorta = _dibujarTorta;
		this.dibujarTitulo = _dibujarTitulo;

		//this.actualizarCanvas = _actualizarCanvas;
	}

// VISUALIZADOR IZUIERDO -------------------------------------------------------

	function VisualizadorIzquierdo () {
		Visualizador.apply(this);

		this.canvas = document.getElementById('canvas_0');
		this.ctx = this.canvas.getContext('2d');

		this.tipo = 0;															// Tipo: 0 Barra | 1 Torta | ...
		this.escalar = 0;														// Escalar: 0 Temperatura | 1 Corriente | ...

		this.dibujar = function (datos) {		

			// Limpia el canvas
			this.ctx.clearRect(0, 0, this.ancho, this.alto);	
			this.dibujarTitulo();
			switch (this.tipo) {
				case 0: this.dibujarBarra(datos, 0); break;
				case 1: this.dibujarTorta(datos, 0); break;
				default: console.log('Error: Tipo izquierdo no definido');
			}
		}
	}

// VISUALIZADOR DERECHO --------------------------------------------------------

	function VisualizadorDerecho () {
		Visualizador.apply(this);

		this.canvas = document.getElementById('canvas_1');
		this.ctx = this.canvas.getContext('2d');

		this.tipo = 1; 															// Tipo: 0 Barra | 1 Torta | ...
		this.escalar = 0;														// Escalar: 0 Temperatura | 1 Corriente | ...
		
		this.dibujar = function (datos) { 

			// Limpia el canvas
			this.ctx.clearRect(0, 0, this.ancho, this.alto);	
			this.dibujarTitulo();
			switch (this.tipo) {
				case 0: this.dibujarBarra(datos, 1); break;
				case 1: this.dibujarTorta(datos, 1); break;
				default: console.log('Error: Tipo derecho no definido');
			}
		}
	}

// DIBUJAR TITULO --------------------------------------------------------------

	// Dibuja el título del diagrama, cualquiera sea
	function _dibujarTitulo () {

		this.ctx.save();

		this.ctx.shadowBlur 	= Math.floor(this.alto * 0.05);
		this.ctx.shadowColor 	= 'rgb(0,198,255)';
		this.ctx.fillStyle 		= 'rgb(255,255,255)';
		this.ctx.font 			=  Math.floor(this.alto * 0.05) + 'px Arial';
		this.ctx.textAlign 		= 'center';

		this.ctx.fillText(titulo[this.escalar], this.ancho / 2, this.paddingT / 2);

		this.ctx.restore();
	}

// DIBUJAR BARRA ---------------------------------------------------------------

	// Dibuja el diagrama de barras en el canvas especificado
	function _dibujarBarra (datos, pos) {

		this.ctx.save();

		// Estilo rejilla guía
		this.ctx.strokeStyle 	= 'rgb(70,70,70)';								// Color rejilla
		this.ctx.fillStyle 		= 'rgb(107,107,107)';							// Color fuente 
		this.ctx.textAlign 		= 'center';
		this.ctx.font 			=  this.alto * 0.025 + 'px Arial';

		// Dibujar rejilla guía
		var valorRangoDivisionesRejilla = (valorMaximo[this.escalar] - valorMinimo[this.escalar]) / this.numeroDivisionesRejilla;
		for (var i = 0 ; i < this.numeroDivisionesRejilla ; i++) {

			// Dibuja el valor de referencia de la rejilla
			this.ctx.fillText(Math.floor(valorRangoDivisionesRejilla * ((1 - pos) * this.numeroDivisionesRejilla + i * (2 * pos - 1))) + unidad[this.escalar], this.anchoDivisionesVertical * i + this.paddingLR, this.paddingT - this.altoBarras);

			// Dibuja un cuadrado para formar la rejilla
			this.ctx.strokeRect(this.anchoDivisionesVertical * i + this.paddingLR, this.paddingT, this.anchoDivisionesVertical, this.altoRejilla);
		}
		this.ctx.fillText(valorMaximo[this.escalar] * pos + valorMinimo[this.escalar] * (1 - pos) + unidad[this.escalar], this.anchoRejlla + this.paddingLR, this.paddingT - this.altoBarras);	

		// Estilo barras
		var gradiente = this.ctx.createLinearGradient(this.paddingLR, 0, this.paddingLR + this.anchoRejlla, 0);
		gradiente.addColorStop(0, (pos) ? 'rgb(0,90,255)' : 'rgb(255,0,0)'); 
		gradiente.addColorStop(0.5, 'rgb(0,220,0)');
		gradiente.addColorStop(1, (pos) ? 'rgb(255,0,0)' : 'rgb(0,90,255)');
		this.ctx.fillStyle = gradiente;

		// Estilo fuentes
		this.ctx.font = Math.floor(200 / (numeroMotores + 1)) + 'px Arial';
		this.ctx.textBaseline = 'top';
		this.ctx.textAlign = (pos) ? 'end' : 'start';

		// Dibujar barras + id motores
		var anchoBarra; 
		for (var i = 0 ; i < numeroMotores ; i++) {

			anchoBarra = this.anchoRejlla * (datos[i] - valorMinimo[this.escalar]) / rangoValores[this.escalar];

			// Dibuja el ID del motor actual
			this.ctx.fillText(i + 1, (1 - pos) * (this.anchoRejlla + 20) + this.paddingLR - 10, this.paddingT + i * this.anchoDivisionHorizontal + this.offset / 2);

			// Dibuja la barra del motor actual
			this.ctx.fillRect((1 - pos) * (this.anchoRejlla - anchoBarra) + this.paddingLR, this.paddingT + i * this.anchoDivisionHorizontal + this.offset, anchoBarra, this.altoBarras);
		}

		this.ctx.restore();
	}

// DIBUJAR TORTA ---------------------------------------------------------------

	function _dibujarTorta (datos, pos) {

		this.ctx.save();
		
		this.ctx.strokeStyle 	= 'rgb(32,32,32)';
		this.ctx.textAlign 		= 'center';
		this.ctx.font 			= this.alto * 0.15 + 'px Arial';		

		this.ctx.save();

		// Traslada el origen del canvas a su centro geométrico 
		this.ctx.translate(this.ancho / 2, this.alto / 2);

		var dato;
		var colorRojo;
		var colorVerde;
		var colorAzul;

		// Itera a lo largo de todas las corridas de articulaciones (motores) del robot.
		// Parte desde la corrida con radio mayor hasta la menor.
		for (var j = numeroArticulaciones ; j > 0 ; j--) {

			// Dibuja circunferencias divididas diametralmente en 'numeroPatas' partes isomórficas.
			for (var i = 0 ; i < numeroPatas ; i++) {

				dato = datos[j + i * numeroArticulaciones - 1];

				colorRojo 			= Math.floor(255 * (dato - valorMinimo[this.escalar]) / rangoValores[this.escalar]);
				colorVerde 			= 255 - colorRojo;
				colorAzul 			= 0;

				/*if (dato > valorMaximo[this.escalar] / 2 ) {
					colorRojo 			= Math.floor(255 * (dato - valorMinimo[this.escalar]) / rangoValores[this.escalar]);
					colorVerde 			= 255 - colorRojo;
					colorAzul 			= 0;
				}
				else {
					colorRojo 			= 0;
					colorVerde 			= Math.floor(255 * (dato - valorMinimo[this.escalar]) / rangoValores[this.escalar]);
					colorAzul 			= 255 - colorVerde;
				}*/

				this.ctx.fillStyle 	= 'rgb(' + colorRojo + ',' + colorVerde + ',' + colorAzul + ')'; 

				// Dibuja un trozo de la circunferencia
				this.ctx.beginPath();
				this.ctx.moveTo(0, 0);
				this.ctx.arc(0, 0, this.radio * j / numeroArticulaciones, - Math.PI / 2, - Math.PI / 2 + Math.PI * 2 / numeroPatas, false); 
				this.ctx.closePath();

				// Rellena el trozo le dibuja un borde negro
				this.ctx.fill();
				this.ctx.stroke();

				// Rota el eje de coordenadas
				this.ctx.rotate(Math.PI / numeroPatas);

				// Dibuja el ID de las patas
				if(j == 1) { 
					this.ctx.fillStyle = 'rgba(0,0,0,0.5)';
					this.ctx.fillText(i + 1, 0, - this.radio / 2.5);
				}		

				// Rota el eje de coordenadas nuevamente
				this.ctx.rotate(Math.PI / numeroPatas);
			}
		}

		// El origen del canvas vuelve a ser el de antes
		this.ctx.restore();

		// Dibujo del rectángulo que servirá como leyenda de colores
		var gradiente = this.ctx.createLinearGradient(this.posicionLeyendaX, 0, this.anchoLeyenda, 0);
		gradiente.addColorStop(0, (pos) ? 'rgb(0,255,0)' : 'rgb(255,0,0)'); 
		gradiente.addColorStop(1, (pos) ? 'rgb(255,0,0)' : 'rgb(0,255,0)');
		this.ctx.fillStyle = gradiente;
		this.ctx.fillRect(this.posicionLeyendaX, this.posicionLeyendaY, this.anchoLeyenda, this.altoLeyenda);
		this.ctx.stroke();
		
		// Estilo de la fuente
		this.ctx.fillStyle 	= 'rgb(107,107,107)';
		this.ctx.font 		= this.alto * 0.025 + 'px Arial';

		// Dibujamos los valores de referencia de la leyenda
		var valorRangoDivisionesLeyenda = rangoValores[this.escalar] / this.numeroDivisionesLeyenda;
		for (var i = 0 ; i <= this.numeroDivisionesLeyenda ; i++) {
			this.ctx.fillText(Math.floor(valorRangoDivisionesLeyenda * ((1 - pos) * this.numeroDivisionesLeyenda + i * (2 * pos - 1))) + unidad[this.escalar], this.anchoDivisionLeyenda * i + this.posicionLeyendaX, this.posicionLeyendaY * 0.97);
		}

		this.ctx.restore();
	}

// DIBUJAR DIAGRAMA ------------------------------------------------------------

	// id canvas: 0 izquierdo | 1 derecho
	function dibujarDiagrama(id) {																				

		var xmlhttp = ConstructorXMLHttpRequest();
		xmlhttp.onreadystatechange = function() {
			if(xmlhttp.readyState == 4)
			{
				canvas[id].dibujar(JSON.parse(xmlhttp.responseText));
			}
		}
		xmlhttp.open('GET', rutaLector + '?archivo=' + archivos[canvas[id].escalar], true); // true: Asincrónico
		xmlhttp.send(null);		
	}	

// CONTROLADOR =================================================================

// ESPECIFICADOR ---------------------------------------------------------------

	function Especificador (ancho) {

		// Dimensiones del canvas
		this.ancho 	= ancho;
		this.alto 	= this.ancho;

		// Margen con los bordes
		this.paddingL 	= 10;
		this.paddingR 	= 10;
		this.paddingT 	= 10;
		this.paddingB 	= 10;

		this.anchoContenedorBotones 			= this.ancho - this.paddingL - this.paddingR;
		this.altoContenedorBotones 				= this.alto - this.paddingT - this.paddingB;
		this.altoBotonCentral 					= this.altoContenedorBotones * 0.3;
		this.altoContenedorBotonesIndividual	= (this.altoContenedorBotones - this.altoBotonCentral) / 2;
		this.altoBoton 							= this.altoContenedorBotonesIndividual / Math.ceil((numeroBotonesEspecificador - 1) / 2);

		// Canvas
		this.canvas = document.getElementById('canvas_2');
		this.ctx 	= this.canvas.getContext('2d');

		this.dibujar = _dibujarEspecificador;

		this.click = function (evento) { 
			var posicionMouse = _obtenerPosicion(evento);
			this.dibujar (posicionMouse.x, posicionMouse.y, 1); 
		};
		this.mover = function (evento) { 
			var posicionMouse = _obtenerPosicion(evento);
			this.dibujar (posicionMouse.x, posicionMouse.y, 0); 
		};

		// Para que dibuje la primera vez
		this.dibujar (0, 0, 0); 
	}

// ESTADO ----------------------------------------------------------------------

	function Estado (ancho, alto) {

		// Dimensiones del canvas
		this.ancho 	= ancho;
		this.alto 	= alto;

		// Margen con los bordes
		this.paddingLR 	= 20;
		this.paddingT 	= 20;
		this.paddingB 	= 20;						

		this.altoDivisionBotones = (this.alto - this.paddingB - this.paddingT) / 2;

		// Brecha de separación entre botones
		this.separacionDivisiones = this.altoDivisionBotones * 0.2;

		// Dimensión de los botones generales (que no son 'Detener')
		this.altoBoton = this.altoDivisionBotones - this.separacionDivisiones / 2;
		this.anchoBoton = this.ancho - 2 * this.paddingLR;

		// Conmutar entre relajar e incorporar
		this.estados = ['RELAJAR', 'REINCORPORAR'];
		this.flag = 0;
		this.clickDetener = 0;

		// Canvas
		this.canvas = document.getElementById('canvas_3');
		this.ctx = this.canvas.getContext('2d');

		this.dibujar = _dibujarEstado;

		this.click = function (evento) { 
			var posicionMouse = _obtenerPosicion(evento);
			this.dibujar (posicionMouse.x, posicionMouse.y, 1); 
		};
		this.mover = function (evento) { 
			var posicionMouse = _obtenerPosicion(evento);
			this.dibujar (posicionMouse.x, posicionMouse.y, 0);  
		};

		// Para que dibuje la primera vez
		this.dibujar (0, 0, 0); 
	}

// DIRECCIONADOR ---------------------------------------------------------------

	function Direccionador (ancho, alto) {

		// Dimensiones del canvas
		this.ancho 	= ancho;
		this.alto 	= alto;

		// Radio máximo del control
	    this.radio 			= this.alto * 0.5;
	    this.radioMinimo 	= this.radio * 0.3;									// Radio mínimo para que la magnitud sea cero
	    this.rangoRadio 	= this.radio - this.radioMinimo;
	    this.numeroDivisionesRadio = 4;											// Número de divisiones del radio con magnitud
		
		this.canvas = document.getElementById('canvas_4');
		this.ctx = this.canvas.getContext('2d');

		// Ángulo del último 'Click!'
		this.anguloActual = 0;													

		this.dibujar = _dibujarDireccionador;
		this.obtenerMagnitud = _obtenerMagnitud;

		this.click = function (evento) { 
			var posicionMouse = _obtenerPosicion(evento);
			this.dibujar (posicionMouse.x, posicionMouse.y, 1); 
		};

		this.mover = function (evento) { 
			var posicionMouse = _obtenerPosicion(evento);
			this.dibujar (posicionMouse.x, posicionMouse.y, 0); 
		};

		// Para que dibuje la primera vez
		this.dibujar (this.ancho / 2 + 1, this.alto / 2, 0); 
	}

// OBTENER POSICION ------------------------------------------------------------

	function _obtenerPosicion (evento) {

       	var x, y;

       	// Obtiene la posición del puntero dentro del canvas  
        if (evento.offsetX != undefined && evento.offsetY != undefined) {
        	x = evento.offsetX;
        	y = evento.offsetY;
    	}

    	//  Firefox
    	else {
    		x = evento.clientX;
    		y = evento.clientY;
    	}

    	return {'x' : x, 'y' : y};
	}

// DIBUJAR ESPECIFICADOR -------------------------------------------------------

	function _dibujarEspecificador (x, y, click) {

		this.ctx.save();

  		// Limpia las gráficas previas
		this.ctx.clearRect(0, 0, this.ancho, this.alto);

		// Estilo de los botones
		var gradiente = this.ctx.createLinearGradient(0, 0, 0, this.alto);
		this.ctx.save();
		gradiente.addColorStop(0, 'rgba(0,0,0,0)');
		gradiente.addColorStop(0.3, 'rgba(0,220,255,0.2)');
		gradiente.addColorStop(0.5, 'rgba(0,180,255,0.5)');
		gradiente.addColorStop(0.7, 'rgba(0,220,255,0.2)');
		gradiente.addColorStop(1, 'rgba(0,0,0,0)');

		// Dibuja el fondo
		this.ctx.fillStyle = gradiente;
		this.ctx.fillRect(this.paddingL, this.paddingT, this.anchoContenedorBotones, this.altoContenedorBotones);

		// Estilo del texto
		this.ctx.fillStyle 		= 'rgb(200,200,200)';
		this.ctx.textAlign 		= 'start';
		this.ctx.textBaseline	= 'middle';
		this.ctx.font			= this.alto * 0.1 + 'px Arial';

		this.ctx.translate(this.paddingL, this.paddingT);

		var idBotonCentral = Math.floor(numeroBotonesEspecificador / 2);
    	for (var i = 0 ; i < numeroBotonesEspecificador ; i++) {

    		if (i != idBotonCentral) {
				this.ctx.fillText(botonesEspecificador[i].titulo, this.paddingL, this.altoBoton / 2);
				this.ctx.translate(0, this.altoBoton);
			}
			else {
				this.ctx.clearRect(0, 0, this.anchoContenedorBotones, this.altoBotonCentral);
				this.ctx.save();
				this.ctx.font = this.alto * 0.2 + 'px Arial';
				this.ctx.fillText(botonesEspecificador[i].titulo, this.paddingL, Math.floor(this.altoBotonCentral / 2));
				this.ctx.restore();
				this.ctx.translate(0, this.altoBotonCentral);
			}
    	}

    	this.ctx.restore();

    	// Rotar opciones
    	if (click == 1) {

    		// Arriba
	    	if (y >= this.paddingT && y <= this.paddingT + this.altoContenedorBotonesIndividual) {
	    		botonesEspecificador.unshift(botonesEspecificador.pop());
	    	}

	    	// Abajo
	    	else if (y >= this.alto - this.paddingB - this.altoContenedorBotonesIndividual && y <= this.alto - this.paddingB) {
	    		botonesEspecificador.push(botonesEspecificador.shift());
	    	}

	    	// Actualiza la vista
	    	canvas[2].dibujar(0, 0, 0);
	    	canvas[4].dibujar(canvas[4].ancho / 2, canvas[4].alto / 2, 0);
    	}
	}

// DIBUJAR ESTADO --------------------------------------------------------------

	function _dibujarEstado (x, y, click) {

		this.ctx.save();

		// Estilo del botón detener
		var gradienteBotonDetener = this.ctx.createLinearGradient(0, 0, 0, this.altoBoton);
		gradienteBotonDetener.addColorStop(0, 'rgb(150,30,30)');
		gradienteBotonDetener.addColorStop(0.1, 'rgb(160,0,0)');
		gradienteBotonDetener.addColorStop(0.5, 'rgb(190,10,10)');
		gradienteBotonDetener.addColorStop(0.9, 'rgb(160,0,0)');
		gradienteBotonDetener.addColorStop(1, 'rgb(50,0,0)');

		if (y <= this.paddingT + this.altoBoton && y >= this.paddingT) {

			if (this.flag == 0) {
				realizarAccion(rutaEjecutor + '?flag=relajar');
				this.flag = 1;
			}
			else {
				realizarAccion(rutaEjecutor + '?flag=reincorporar');
				this.flag = 0;
			}			
		}  
		else if (y <= this.alto - this.paddingB && y >= this.alto - this.altoBoton) {
			realizarAccion(rutaEjecutor + '?flag=detener');

			if (this.clickDetener == 1) {
				gradienteBotonDetener.addColorStop(0, 'rgb(190,70,70)');
				gradienteBotonDetener.addColorStop(0.1, 'rgb(2000,0,0)');
				gradienteBotonDetener.addColorStop(0.5, 'rgb(230,10,10)');
				gradienteBotonDetener.addColorStop(0.9, 'rgb(230,0,0)');
				gradienteBotonDetener.addColorStop(1, 'rgb(90,0,0)');
			}
		}

		// Limpia las gráficas previas
		this.ctx.clearRect(0, 0, this.ancho, this.alto);

		// Estilo del botón relajar-reincorporar
		var gradienteBotonGeneral = this.ctx.createLinearGradient(0, 0, 0, this.altoBoton);
		gradienteBotonGeneral.addColorStop(0, 'rgb(100,100,100)');
		gradienteBotonGeneral.addColorStop(0.1, 'rgb(200,200,200)');
		gradienteBotonGeneral.addColorStop(0.5, 'rgb(220,220,220)');
		gradienteBotonGeneral.addColorStop(0.9, 'rgb(160,160,160)');
		gradienteBotonGeneral.addColorStop(1, 'rgb(50,50,50)');

		// Estilo del borde de los botones
		this.ctx.lineJoin		= 'round';
		this.ctx.lineWidth 		= this.altoBoton * 0.04;
		this.ctx.strokeStyle 	= 'rgba(0,0,0,0.5)';

		// Estilo del texto
		this.ctx.shadowBlur 	= this.altoBoton * 0.2;
		this.ctx.shadowColor 	= 'rgba(0,0,0,0.5)';
		this.ctx.textAlign 		= 'center';
		this.ctx.textBaseline	= 'middle';
		this.ctx.font 			=  Math.floor(this.altoBoton * 0.4) + 'px Arial';

		// Primer botón
		this.ctx.translate(this.paddingLR, this.paddingT);
		this.ctx.fillStyle = gradienteBotonGeneral;
		this.ctx.fillRect(0, 0, this.anchoBoton, this.altoBoton);
		this.ctx.strokeRect(0, 0, this.anchoBoton, this.altoBoton);

		// Texto primer botón
		this.ctx.fillStyle = 'rgb(70,70,70)';
		this.ctx.fillText(this.estados[this.flag], this.anchoBoton / 2, this.altoBoton / 2);

		// Segundo botón
		this.ctx.translate(0, this.altoBoton + this.separacionDivisiones);
		this.ctx.fillStyle = gradienteBotonDetener;
		this.ctx.fillRect(0, 0, this.anchoBoton, this.altoBoton);
		this.ctx.strokeRect(0, 0, this.anchoBoton, this.altoBoton);

		// Texto segundo botón
		this.ctx.fillStyle = 'rgb(190,190,190)';
		this.ctx.fillText('DETENER', this.anchoBoton / 2, this.altoBoton / 2);



		this.ctx.restore();
	}

// DIBUJAR DIRECCIONADOR -------------------------------------------------------

	function _dibujarDireccionador (x, y, click) {

		// Traslado de la posición obtenida hacia el centro del canvas,
		// dando vuelta el eje de las ordenadas 
		x -= (this.ancho - this.alto / 2);
		y = this.alto / 2 - y;

	    // Si el puntero está dentro radio del direccionador ...
	    if ( x * x + y * y <= this.radio * this.radio ) {

	    	// DEBUGGER
			//console.log('Direccionador | x: ' + x + ' y: ' + y);

			this.ctx.save();

			// Angulo con respecto al eje positivo de las abscisas
       		var angulo;

	    	// Limpia la gráfica previa
			this.ctx.clearRect(0, 0, this.ancho, this.alto);

		    // Calcula el angulo del vector (x,y) respecto al eje de las abscisas 
		    if (x == 0 && y == 0) 	angulo = this.anguloActual;
		    else {
		    	angulo = Math.atan(y / x);
		    	if (x < 0)			angulo += Math.PI;
		    	else if (y <= 0) 	angulo += 2 * Math.PI;
			}

	       	// Traslada el origen del canvas al centro geométrico del mismo
			this.ctx.translate(this.ancho / 2, this.alto / 2);

			// Gradiente Circulo grande
	       	var gradienteCirculoGrande = this.ctx.createRadialGradient(0, 0, this.radio, 0, 0, 0);
	       	gradienteCirculoGrande.addColorStop(0, 'rgba(0,0,0,0)');
	       	gradienteCirculoGrande.addColorStop(0.2, 'rgba(100,190,220,0.05)');
	       	gradienteCirculoGrande.addColorStop(0.3, 'rgba(30,210,250,0.1)');
	       	gradienteCirculoGrande.addColorStop(0.31, 'rgba(0,160,255,0.7)');
	       	gradienteCirculoGrande.addColorStop(0.35, 'rgba(0,20,100,0.2)');
	       	gradienteCirculoGrande.addColorStop(1, 'rgba(0,0,0,0)');

	       	// Gráfica Circulo grande
	       	this.ctx.fillStyle = gradienteCirculoGrande;
	       	this.ctx.beginPath();
	       	this.ctx.arc(0, 0, this.radio, 0, 2 * Math.PI, false);
	       	this.ctx.closePath();
	       	this.ctx.fill();

	       	// Escritura de los ángulos
			this.ctx.fillStyle 		= 'rgb(150,180,200)';
			this.ctx.font 			=  this.alto * 0.2 + 'px Arial';
			this.ctx.textAlign 		= 'center';
			this.ctx.textBaseline	= 'middle';

			// Todos los grados de libertad
			if (botonesEspecificador[idAccionActual].grados == 0) {
				this.ctx.font = this.alto * 0.2 + 'px Arial';
				this.ctx.fillText(this.anguloActual + 'º', 0, 0);
				this.ctx.font = this.alto * 0.1 + 'px Arial'; 
				this.ctx.fillText(Math.floor(angulo * 180 / Math.PI) + 'º', 0, 40);	
				this.ctx.rotate(- angulo);
				angulo = Math.floor(angulo * 180 / Math.PI);
			}

			// Niveles
			else if (botonesEspecificador[idAccionActual].grados == 1) {
				this.ctx.font = this.alto * 0.1 + 'px Arial';
				if (angulo < 2 * Math.PI && angulo >= Math.PI) {
					this.ctx.fillText('Abajo', 0, 0);
					this.ctx.rotate(Math.PI / 2);	
					angulo = 0; // Ángulo real : 3 * Math.PI / 2
				}
				else {
					this.ctx.fillText('Arriba', 0, 0);
					this.ctx.rotate(- Math.PI / 2);
					angulo = 1; // Ángulo real : Math.PI / 2
				}
			}

			// Lados
			else if (botonesEspecificador[idAccionActual].grados == 2) {
				this.ctx.font = this.alto * 0.1 + 'px Arial';
				if (angulo >= Math.PI / 2 && angulo < 3 * Math.PI / 2) {
					this.ctx.fillText('Izquierda', 0, 0);
					this.ctx.rotate(Math.PI);
					angulo = 0; // Ángulo real : Math.PI
				}
				else {
					this.ctx.fillText('Derecha', 0, 0);
					angulo = 1; // Ángulo real : 0
				}
			}

			var magnitud = -1;													// -1 : no me interesa la magnitud para enviar

			// Especifica la magnitud
			if (botonesEspecificador[idAccionActual].magnitud == 1) {

				magnitud = this.obtenerMagnitud(x,y);

				// Gradiente circulo magnitud
				var gradienteCirculoMagnitud = this.ctx.createRadialGradient(0, 0, this.radioMinimo + this.rangoRadio * magnitud / this.numeroDivisionesRadio, 0, 0, 0);
				gradienteCirculoMagnitud.addColorStop(0, 'rgba(0,0,0,0)');
				gradienteCirculoMagnitud.addColorStop(0.05, 'rgba(100,190,220,' + (magnitud + 1) / (3 * this.numeroDivisionesRadio) + ')');
				gradienteCirculoMagnitud.addColorStop(1, 'rgba(0,0,0,0)');

				// Dibujar circulo magnitud
				this.ctx.fillStyle = gradienteCirculoMagnitud;
				this.ctx.beginPath();
				this.ctx.arc(0, 0, this.radioMinimo + this.rangoRadio * magnitud / this.numeroDivisionesRadio, 0, 2 * Math.PI, false);
				this.ctx.closePath();
				this.ctx.fill();
			}

			// Gráfica Circulo pequeño
			this.ctx.fillStyle = 'rgba(0,160,255,0.5)';
			this.ctx.beginPath();
			this.ctx.arc(this.radio * 0.7, 0, this.radio / 10, 0, 2 * Math.PI, false);
			this.ctx.closePath();
			this.ctx.fill();

			// Envia el ángulo seleccionado al servidor
	       	if (click == 1) {

				var accion = botonesEspecificador[idAccionActual].accion;

	       		// Revisa si importa la magnitud
				magnitud = (magnitud == -1) ? '' : ';' + magnitud; 

	       		this.anguloActual = angulo;

				if (accion == 'C') {
					// TMP - Simula un click en Detener
					realizarAccion(rutaEjecutor + '?flag=detener');
					esperar();
				}
				realizarAccion(rutaEjecutor + '?flag=' + accion + angulo + magnitud);
	       	}

	       	this.ctx.restore();
	    }
	}

	function _obtenerMagnitud (x, y) {

		if (x * x + y * y > this.radioMinimo * this.radioMinimo) {
			return Math.ceil(this.numeroDivisionesRadio * (Math.sqrt(x * x + y * y) - this.radioMinimo) / this.rangoRadio);
		}
		return 0;
	}

// REALIZAR ACCION =============================================================

	function realizarAccion (accion) {

		// DEBUGGER
		console.log('accion: ' + accion);

		var xmlhttp = ConstructorXMLHttpRequest();
		xmlhttp.open('GET', accion, true); // true: Asincrónico
		xmlhttp.send(null);
	}

// ESPERAR =====================================================================

	function esperar () {

		var delay = 100; 
		var now = new Date();
		var desiredTime = new Date().setMilliseconds(now.getMilliseconds() + delay);

		while (now < desiredTime) {
		    now = new Date(); // update the current time
		}
	}

// INIT ========================================================================

	// Configuración inicial de las variables globales.
	function init() {
		// Dirección de la imagen donde se streamea.
		var streamdir = window.location.protocol + "//" + window.location.hostname + ":8080/?action=stream";
		
		// Configuraciones iniciales del robot y de los visualizadores
		var xmlhttp = ConstructorXMLHttpRequest();
				
		xmlhttp.onreadystatechange = function() {
			if(xmlhttp.readyState == 4)
			{
				var config = JSON.parse(xmlhttp.responseText);

				// Datos generales del robot
				nombreRobot 			= config.nombre;
				numeroPatas 			= config.numPatas;
				numeroArticulaciones 	= config.numArticulaciones;
				numeroMotores 			= numeroPatas * numeroArticulaciones;

				for (var i = 0 ; i < config.autores.length ; i++) {
					autores[i] = config.autores[i];
				}

				// Datos de las variables escalares [temperatura, corriente, ...]
				for (var i = 0 ; i < config.grafico.length ; i++) {

					titulo[i] 		= config.grafico[i].titulo;
					valorMaximo[i] 	= config.grafico[i].valorMax;
					valorMinimo[i] 	= config.grafico[i].valorMin;
					rangoValores[i] = valorMaximo[i] - valorMinimo[i];
					unidad[i] 		= config.grafico[i].unidad;
				}

				numeroBotonesEspecificador = config.control.length;
				for (var i = 0 ; i < numeroBotonesEspecificador ; i++) {
					botonesEspecificador[i] = {
						'titulo' 	: config.control[i].titulo, 
						'accion' 	: config.control[i].idAccion, 
						'grados' 	: config.control[i].idGradosLibertad,
						'magnitud' 	: config.control[i].magnitud
					}
				}

				idAccionActual = Math.floor(numeroBotonesEspecificador / 2); 

				var canvas0	= document.getElementById("canvas_0");
				var canvas1	= document.getElementById("canvas_1");
				var canvas2 = document.getElementById("canvas_2");
				var canvas3 = document.getElementById("canvas_3");
				var canvas4 = document.getElementById("canvas_4");

				var viewportWidth = window.innerWidth;

				canvas0.width 	= viewportWidth * 0.22;
				canvas0.height 	= canvas0.width * 4 / 3;
				canvas1.width 	= viewportWidth * 0.22;
				canvas1.height 	= canvas0.width * 4 / 3;
				canvas2.width 	= viewportWidth * 0.18;
				canvas2.height 	= canvas2.width;
				canvas3.width 	= viewportWidth * 0.3;
				canvas3.height 	= canvas3.width * 0.6;
				canvas4.width 	= viewportWidth * 0.18;
				canvas4.height 	= canvas4.width;
				
				anchoVisualizador 	= canvas0.width;
				altoVisualizador 	= canvas0.height

				canvas[0] = new VisualizadorIzquierdo();
				canvas[1] = new VisualizadorDerecho();
				canvas[2] = new Especificador(			canvas2.width, canvas2.height);
				canvas[3] = new Estado(					canvas3.width, canvas3.height);
				canvas[4] = new Direccionador(			canvas4.width, canvas4.height);

				var canvasIzquierdo	= setInterval( function () { dibujarDiagrama(0); }, tiempoLoopDiagramaIzquierdo );
				var canvasDerecho	= setInterval( function () { dibujarDiagrama(1); }, tiempoLoopDiagramaDerecho );

				canvas2.addEventListener("click", function (evento) { canvas[2].click(evento); }, false);
				//canvas3.addEventListener("click", function (evento) { canvas[3].click(evento); }, false);
				canvas4.addEventListener("click", function (evento) { canvas[4].click(evento); }, false);
	
				canvas3.addEventListener("mousedown", function (evento) { canvas[3].clickDetener = 1; canvas[3].click(evento); }, false);
				canvas3.addEventListener("mouseup", function (evento) { canvas[3].clickDetener = 0; canvas[3].dibujar(0,0,0); }, false);

				// Por ahora no es necesario, ya que los botones son estáticos
				//canvas3.addEventListener("mousemove", function (evento) { canvas[3].mover(evento); }, false); 
				//canvas2.addEventListener("mousemove", function (evento) { canvas[2].mover(evento); }, false);

				canvas4.addEventListener("mousemove", function (evento) { canvas[4].mover(evento); }, false);
			}
		}
		xmlhttp.open('GET', rutaLector + '?json=true&archivo=' + rutaConfig, true); // true: Asincrónico 
		xmlhttp.send(null);
		
		// Cambia el puerto de la imagen de streaming.
		document.getElementById("streaming").src = streamdir;
	}


// OTROS =======================================================================

	function cambiarTipo(tipo, posicion) {
		canvas[posicion].tipo = tipo; 

	}

	function cambiarEscalar(escalar, posicion) {
		canvas[posicion].escalar = escalar; 
	}

	function conmutarMenu () {
		if (document.getElementById('menu_configuracion').style.display == 'none') {
			document.getElementById('menu_configuracion').style.display = 'block';
		}
		else {
			document.getElementById('menu_configuracion').style.display = 'none';
		}
	}

	function conmutarVista () {
		if (document.getElementById('streaming').style.display == 'none') {
			document.getElementById('crabot_image').style.display = 'none';
			document.getElementById('streaming').style.display = 'block';
		}
		else {
			document.getElementById('crabot_image').style.display = 'block';
			document.getElementById('streaming').style.display = 'none';
		}
	}