#define TRIGGER 5
#define TRIG_DELAY 100
#define	FRENTE 4
#define TRAS 7
#define LADO_1 
#define LADO_2 
#define LDR_1 A4
#define LDR_2 A5
#define LDR_3 A6
#define LDR_4 A7
#define ESQUERDA -1
#define DIREITA 1
#define KPROP 1
#define KINTEG 1
#define KDERIV 1
#define KAMORT 1

void iniciaSensorLDR(int analogpin)
{
	pinMode(analogpin, INPUT);
}

void iniciaSensorUS(byte echo)
{
	pinMode(TRIGGER, OUTPUT);
	pinMode(echo, INPUT);
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

void tocaobarco()
{
	int tempo = 0, tempo_ant = 0, dtempo;
	int p, i = 0, d, pidfrente;
	int erro = 0, erro_ant;
	
	while(SensorUS(FRENTE) > DIST_OBSTACULO)
	{		
		erro_ant = erro;
		erro = DIST_FUNDO - SensorUS(TRAS);
		tempo_ant = tempo;
		tempo = millis();
		dtempo = tempo - tempo_ant;
		
		p = erro;
		i = KAMORT*i + erro*dtempo;
		d = (erro - erro_ant)/dtempo;
		pidfrente = KPROP*p + KINTEG*i + KDERIV*d;
		
		//funções que passam pros motores potência BASE+pidfrente
		
		//falta fazer o controle pra manter o robô na linha e uma garantia de que ele não vai sair da faixa que ele tá (usando os LDRs)
		
	}
}


void setup()
{
	iniciaSensorUS();
	iniciaSensorLDR();
}

void loop()
{
	int distancia = SensorUS(FRENTE); // lê a distância no ultrassom da frente
	int ldr_1 = SensorLDR(LDR_1); // lê a distância no LDR_1
}