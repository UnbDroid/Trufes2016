/*ULTRASSONS*/

#define TRIGGER 5
#define TRIG_DELAY 100
#define	USFRENTE 4
#define USTRAS 7
#define LADO_FRENTE 
#define LADO_TRAS 

/*LDRs*/

#define LDR_ESQ A4
#define LDR_DIR A5

/*MOTORES*/

#define ENABLE 
#define MOTOR_DESVIO_ESQ
#define MOTOR_DESVIO_DIR
#define MOTOR_ESQ
#define MOTOR_DIR

/*DESVIO*/

#define TEMPO_PASSA_LINHA 
#define POT_DESVIO
#define ESQUERDA -1
#define DIREITA 1

/*PID*/

#define POTBASE 127

	/*PID FRENTE*/
	
	#define KFPROP 1
	#define KFINTEG 1
	#define KFDERIV 1
	#define KFAMORT 1
	
	/*PID ALINHA*/
	
	#define KAPROP1
	#define KAINTEG1
	#define KADERIV1
	#define KAPROP2
	#define KAINTEG2
	#define KADERIV2
	#define KAPROP3
	#define KAINTEG3
	#define KADERIV3
	#define KAAMORT

/*FIM DO CABEÇALHO*/

void iniciaSensorLDR(byte analogpin)
{
	pinMode(analogpin, INPUT);
}

void iniciaSensorUS(byte echo)
{
	pinMode(TRIGGER, OUTPUT);
	pinMode(echo, INPUT);
}

void iniciaMotores()
{
	pinMode(ENABLE, OUTPUT);
	pinMode(MOTOR_DESVIO_ESQ, OUTPUT);
	pinMode(MOTOR_DESVIO_DIR, OUTPUT);
	pinMode(MOTOR_ESQ, OUTPUT);
	pinMode(MOTOR_DIR, OUTPUT);
}

int SensorLDR(byte qualsensor) // passar LDR_ESQ ou LDR_DIR
{
	return analogRead(qualsensor);
}

int SensorUS(byte qualsensor) // passar USFRENTE ou USTRAS como parâmetro
{
	digitalWrite(TRIGGER, LOW);
	delayMicroseconds(2);
	digitalWrite(TRIGGER, HIGH); // ativa o sensor HC-SR04 com um pulso de 5 microssegundos no pino Trigger
	delayMicroseconds(5);
	digitalWrite(TRIGGER, LOW);
  
	unsigned long duration = pulseIn(qualsensor, HIGH); // calcula o tempo necessário para o retorno do pulso sonoro
	int distanceCentimeters = (duration * 17)/1000; // calcula a distância percorrida pelo pulso sonoro
	
	delay(TRIG_DELAY);
	return distanceCentimeters; // retorna o valor encontrado
}

int tanalinha(byte qual_ldr)
{
	if(SensorLDR(qual_ldr) < COR_LINHA)
	{
		return 1;
	} else {
		return 0;
	}
}

void desvia(byte lado)
{
	digitalWrite(ENABLE, HIGH);
	digitalWrite(MOTOR_DESVIO_ESQ, LOW);
	digitalWrite(MOTOR_DESVIO_DIR, LOW);
	
	if(lado == ESQUERDA)
	{
		analogWrite(MOTOR_DESVIO_ESQ, POT_DESVIO);
		while(!tanalinha(LDR_ESQ));
		delay(TEMPO_PASSA_LINHA);
		while(!tanalinha(LDR_DIR));
		delay(TEMPO_PASSA_LINHA);
		digitalWrite(MOTOR_DESVIO_ESQ, LOW);
	} else {
		analogWrite(MOTOR_DESVIO_DIR, POT_DESVIO);
		while(!tanalinha(LDR_DIR));
		delay(TEMPO_PASSA_LINHA);
		while(!tanalinha(LDR_ESQ));
		delay(TEMPO_PASSA_LINHA);
		digitalWrite(MOTOR_DESVIO_DIR, LOW);
	}
}

void tocaobarco(int faixa)
{
	int tempo = 0, tempo_ant = 0, dtempo;
	int pfrente, ifrente = 0, dfrente, pidfrente;
	int palinha, ialinha = 0, dalinha, pidalinha;
	int errofrente = DIST_FUNDO - SensorUS(USTRAS), errofrente_ant;
	int erroalinha = SensorUS(LADO_FRENTE) - SensorUS(LADO_TRAS), erroalinha_ant;
	int potesq, potdir;	
	
	while(SensorUS(USFRENTE) > DIST_OBSTACULO)
	{		
		errofrente_ant = errofrente;
		errofrente = DIST_FUNDO - SensorUS(USTRAS);
		
		erroalinha_ant = erroalinha;
		erroalinha = SensorUS(LADO_FRENTE) - SensorUS(LADO_TRAS);
		
		tempo_ant = tempo;
		tempo = millis();
		dtempo = tempo - tempo_ant;
		
		pfrente = errofrente;
		ifrente = KFAMORT*ifrente + errofrente*dtempo;
		dfrente = (errofrente - errofrente_ant)/dtempo;
		pidfrente = KFPROP*pfrente + KFINTEG*ifrente + KFDERIV*dfrente;
		
		palinha = erroalinha;
		ialinha = KAAMORT*ialinha + erroalinha*dtempo;
		dalinha = (erroalinha - erroalinha_ant)/dtempo;
		
		switch(faixa)
		{
		case 1:
			pidalinha = KAPROP1*palinha + KAINTEG1*ialinha + KADERIV1*dalinha;
			break;
		case 2:
			pidalinha = KAPROP2*palinha + KAINTEG2*ialinha + KADERIV2*dalinha;
			break;
		case 3:
			pidalinha = KAPROP3*palinha + KAINTEG3*ialinha + KADERIV3*dalinha;
			break;
		}
		
		potdir = POTBASE+pidfrente+pidalinha;
		potesq = POTBASE+pidfrente-pidalinha;
		
		if(potdir > MAX_POWER) potdir = MAX_POWER;
		if(potesq > MAX_POWER) potesq = MAX_POWER;
		if(potdir < MIN_POWER) potdir = MIN_POWER;
		if(potesq < MIN_POWER) potesq = MIN_POWER;
		
		analogWrite(MOTOR_DIR, potdir);
		analogWrite(MOTOR_ESQ, potesq);
		
		//não vai escapar da faixa (usar os LDRs)\/\/\/\/\/\/\/\/\/\/\/\/\/
		analogWrite(MOTOR_DESVIO_DIR, LOW);
		analogWrite(MOTOR_DESVIO_ESQ, LOW);
		
		switch(faixa)
		{
			case 1:
				if(!tanalinha(LDR_DIR))//achou linha direita
					analogWrite(MOTOR_DESVIO_DIR, POTBASE);
				break;
			case 2:
				if(!tanalinha(LDR_DIR))//achou linha direita
					analogWrite(MOTOR_DESVIO_DIR, POTBASE);
				if(!tanalinha(LDR_DIR))//achou linha esquerda
					analogWrite(MOTOR_DESVIO_ESQ, POTBASE);
				break;
			case 3:
				if(!tanalinha(LDR_DIR))//achou linha esquerda
					analogWrite(MOTOR_DESVIO_ESQ, POTBASE);
				break;
		}
		//Testar se essa parte do código não permite que o robô perca a linha /\/\/\/\/\/\/\/
	}
	analogWrite(MOTOR_DIR, POTBASE+pidfrente); //passa pros motores a potência só pra ir pra frente antes de chamar a função de desvio
	analogWrite(MOTOR_ESQ, POTBASE+pidfrente);

	//FUNCAO DESVIA

	switch(faixa)//tende a ficar na faixa 1(parede à esquerda)
	{
		case 1:
			desvio(DIREITA);
			break;
		case 2:
			desvio(ESQUERDA);
			break;
		case 3:
			desvio(ESQUERDA);
			break;
	}
	//END DESVIA
}


void setup()
{
	iniciaSensorUS(USFRENTE);
	iniciaSensorUS(USTRAS);
	iniciaSensorUS(LADO_FRENTE);
	iniciaSensorUS(LADO_TRAS);
	
	iniciaSensorLDR(LDR_ESQ);
	iniciaSensorLDR(LDR_DIR);
	
	iniciaMotores();
}


void fazOObvioEVaiEmbora()//So vai pra frente
{
	int t = 0;

	analogWrite(MOTOR_DIR, POTBASE);
	analogWrite(MOTOR_ESQ, POTBASE);
	
	while(t < 2000)
	{
		t = millis();
	}
	
	analogWrite(MOTOR_DIR, LOW);
	analogWrite(MOTOR_ESQ, LOW);
	

}

void loop()
{
	int distancia = SensorUS(USFRENTE); // lê a distância no ultrassom da frente
	int ldr_esq = SensorLDR(LDR_ESQ); // lê a distância no LDR_ESQ
	fazOObvioEVaiEmbora();//so vai pra frente
}