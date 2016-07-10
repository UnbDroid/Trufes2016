/*ULTRASSONS*/

#define TRIGGER 5
#define TRIG_DELAY 100
#define USFRENTE 4
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
#define MAX_POWER 255
#define MIN_POWER 0

#define MOTOR_ESQ 11
#define MOTOR_DIR 3

/*DESVIO*/

#define TEMPO_PASSA_LINHA 
#define POT_DESVIO 
#define ESQUERDA -1
#define DIREITA 1

/*PID*/

#define POTBASE 127
#define TEMPO_PID 0

  /*PID FRENTE*/
  
  #define KFPROP 0  //constante da parte proporcional do PID
  #define KFINTEG 0 //constante da parte integral do PID
  #define KFDERIV 0 //constante da parte diferencial do PID
  #define KFAMORT 1 //amortecimento da parte integral do PID
  
  /*PID ALINHA*/
  
  #define KAPROP1 0
  #define KAINTEG1 0
  #define KADERIV1 0
  #define KAPROP2 0
  #define KAINTEG2 0
  #define KADERIV2 0
  #define KAPROP3 0
  #define KAINTEG3 0
  #define KADERIV3 0
  #define KAAMORT 1

/*FIM DO CABEÇALHO*/

void iniciaSensorLDR(byte analogpin) // inicializa o LDR passando o pino de leitura dele
{
  pinMode(analogpin, INPUT);
}

void iniciaSensorUS(byte echo) // inicializa o ultrassom passando o pino do echo
{
  pinMode(TRIGGER, OUTPUT);
  pinMode(echo, INPUT);
}

void iniciaMotores() // inicializa os pinos dos motores
{
  pinMode(ENABLE, OUTPUT);
  pinMode(MOTOR_DESVIO_ESQ, OUTPUT);
  pinMode(MOTOR_DESVIO_DIR, OUTPUT);
  pinMode(MOTOR_ESQ, OUTPUT);
  pinMode(MOTOR_DIR, OUTPUT);
}

int SensorLDR(byte qualsensor) // lê o valor no LDR // passar LDR_ESQ ou LDR_DIR
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

int tanalinha(byte qual_ldr) // retorna 1 se o LDR vê a linha e 0 caso contrário
{
  if(SensorLDR(qual_ldr) < COR_LINHA)
  {
    return 1;
  } else {
    return 0;
  }
}

void mudafaixa(byte lado) // falta ajustar a função pra usar as novas funções desvia e stopdesvia
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

void desvia(byte lado) // liga o motor de desvio pra ESQUERDA ou DIREITA
{
  if(lado == ESQUERDA)
  {
    digitalWrite(ENABLE, HIGH);
    digitalWrite(MOTOR_DESVIO_DIR, LOW);
    analogWrite(MOTOR_DESVIO_ESQ, POT_DESVIO);
  } else {
    digitalWrite(ENABLE, HIGH);
    digitalWrite(MOTOR_DESVIO_ESQ, LOW);
    analogWrite(MOTOR_DESVIO_DIR, POT_DESVIO);
  }
}

void stopdesvia() // desliga o motor de desvio
{
  digitalWrite(ENABLE, LOW);
  digitalWrite(MOTOR_DESVIO_DIR, LOW);
  digitalWrite(MOTOR_DESVIO_ESQ, LOW);
}

void tocaobarco(int faixa)
{
  int tempo = 0, tempo_ant = 0, dtempo;
  int pfrente, ifrente = 0, dfrente, pidfrente;
  int palinha, ialinha = 0, dalinha, pidalinha;
  int errofrente = DIST_FUNDO - SensorUS(USTRAS), errofrente_ant;
  int erroalinha = SensorUS(LADO_FRENTE) - SensorUS(LADO_TRAS), erroalinha_ant;
  int potesq, potdir; 
  
  digitalWrite(ENABLE, LOW);
  
  while(SensorUS(USFRENTE) > DIST_OBSTACULO)
  {   
    /*trecho que calcula e passa pros motores o controle pid*/
    
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

    /*fim do pid*/


    /*trecho que garante que o robô não vai escapar da faixa*/
    
      //falta escrever a função para usar o ultrassom para garantir que o robô permaneça na faixa
    
    switch(faixa)
    {
      case 1:
        if(tanalinha(LDR_DIR)) //achou linha direita
        {
          desvia(ESQUERDA);
        } else {
          stopdesvia();
        }
        break;
      case 2:
        if(tanalinha(LDR_ESQ) || tanalinha(LDR_DIR))
        {
          if(tanalinha(LDR_DIR)) desvia(ESQUERDA);
          if(tanalinha(LDR_ESQ)) desvia(DIREITA);
        } else {
          stopdesvia();
        }
        break;
      case 3:
        if(tanalinha(LDR_ESQ)) //achou linha esquerda
        {
          desvia(DIREITA);
        } else {
          stopdesvia();
        }
        break;
    }
    
    delay(TEMPO_PID);
    
    /*fim do trecho p não escapar da faixa*/
  }

  analogWrite(MOTOR_DIR, POTBASE+pidfrente); //passa pros motores a potência só pra ir pra frente antes de chamar a função de desvio
  analogWrite(MOTOR_ESQ, POTBASE+pidfrente);
  stopdesvia(); // desliga a ponte H
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

void loop()
{
  int distancia = SensorUS(USFRENTE); // lê a distância no ultrassom da frente
  int ldr_esq = SensorLDR(LDR_ESQ); // lê a distância no LDR_ESQ



}
