#define TRIGGER 5
#define	FRENTE 4
#define TRAS 7
#define LDR_1 A4
#define LDR_2 A5
#define LDR_3 A6
#define LDR_4 A7

void iniciaSensorLDR()
{
	pinMode(LDR_1, INPUT);
	pinMode(LDR_2, INPUT);
	pinMode(LDR_3, INPUT);
	pinMode(LDR_4, INPUT);
}

void iniciaSensorUS()
{
	pinMode(TRIGGER, OUTPUT);
	pinMode(FRENTE, INPUT);
	pinMode(TRAS, INPUT);
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
	return distanceCentimeters; // retorna o valor encontrado
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