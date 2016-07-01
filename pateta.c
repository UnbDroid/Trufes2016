#define TRIGGER 5
#define TRIG_DELAY 100
#define	USFRENTE 4
#define USTRAS 7
#define LADO_1 
#define LADO_2 
#define LDR_1 A4
#define LDR_2 A5
#define LDR_3 A6
#define LDR_4 A7
#define MOTOR_DESVIOA
#define MOTOR_DESVIOB
#define MOTOR_ESQ
#define MOTOR_DIR

#define ESQUERDA -1
#define DIREITA 1

#define KFPROP 1
#define KFINTEG 1
#define KFDERIV 1
#define KFAMORT 1
#define POTBASE 127


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
	pinMode(MOTOR_DESVIOA, OUTPUT);
	pinMode(MOTOR_DESVIOB, OUTPUT);
	pinMode(MOTOR_ESQ, OUTPUT);
	pinMode(MOTOR_DIR, OUTPUT);
}

int SensorLDR(byte qualsensor) // passar LDR_N sendo N = 1, 2, 3 ou 4
{
	return analogRead(qualsensor);
}

int SensorUS(byte qualsensor) // passar FRENTE ou TRAS como parâmetro
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
	if(lado == ESQUERDA)
	{
		analogWrite(DESVIA_ESQUERDA, POT_DESVIO);
		while(!tanalinha(LDR_1));
		delay(TEMPO_PASSA_LINHA);
		while(!tanalinha(LDR_1));
		digitalWrite(DESVIA_ESQUERDA, LOW);
	} else {
		analogWrite(DESVIA_DIREITA, POT_DESVIO);
		while(!tanalinha(LDR_2));
		delay(TEMPO_PASSA_LINHA);
		while(!tanalinha(LDR_2));
		digitalWrite(DESVIA_DIREITA, LOW);
	}
}

void tocaobarco(int faixa)
{
	int tempo = 0, tempo_ant = 0, dtempo;
	int pfrente, ifrente = 0, dfrente, pidfrente;
	int palinha, ialinha = 0, dalinha, pidalinha;
	int errofrente = DIST_FUNDO - SensorUS(TRAS), errofrente_ant;
	int erroalinha = SensorUS(LADO_1) - SensorUS(LADO_2), erroalinha_ant;
	int potesq, potdir;	
	
	while(SensorUS(USFRENTE) > DIST_OBSTACULO)
	{		
		errofrente_ant = errofrente;
		errofrente = DIST_FUNDO - SensorUS(USTRAS);
		
		erroalinha_ant = erroalinha;
		erroalinha = SensorUS(LADO_1) - SensorUS(LADO_2);
		
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
		
		if(potdir > 255) potdir = 255;
		if(potesq > 255) potesq = 255;
		if(potdir < 0) potdir = 0;
		if(potesq < 0) potesq = 0;
		
		analogWrite(MOTOR_DIR, potdir);
		analogWrite(MOTOR_ESQ, potesq);
		
		//falta garantir que ele não vai escapar da faixa (usar os LDRs)
	}
	
	analogWrite(MOTOR_DIR, POTBASE+pidfrente); //passa pros motores a potência só pra ir pra frente antes de chamar a função de desvio
	analogWrite(MOTOR_ESQ, POTBASE+pidfrente);
}


void setup()
{
	iniciaSensorUS(USFRENTE);
	iniciaSensorUS(USTRAS);
	iniciaSensorUS(LADO_1);
	iniciaSensorUS(LADO_2);
	
	iniciaSensorLDR(LDR_1);
	iniciaSensorLDR(LDR_2);
	
	iniciaMotores();
}

void loop()
{
	int distancia = SensorUS(USFRENTE); // lê a distância no ultrassom da frente
	int ldr_1 = SensorLDR(LDR_1); // lê a distância no LDR_1
}