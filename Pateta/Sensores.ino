int ldr_esq, ldr_dir;
unsigned long usfrente, ustras, usesq, usdir;

void iniciaLDR() // inicializa o LDR passando o pino de leitura dele
{
  pinMode(LDR_ESQ, INPUT);
  pinMode(LDR_DIR, INPUT);
  pinMode(LED_ESQ, OUTPUT);
  pinMode(LED_DIR, OUTPUT);
  digitalWrite(LED_ESQ, HIGH);
  digitalWrite(LED_DIR, HIGH);
  ldr_esq = analogRead(LDR_ESQ);
  ldr_dir = analogRead(LDR_DIR);
}

void iniciaUS() // inicializa o ultrassom passando o pino do echo
{
  pinMode(TRIGGER, OUTPUT);
  pinMode(USFRENTE, INPUT);
  pinMode(USTRAS, INPUT);
  pinMode(USESQ, INPUT);
  pinMode(USDIR, INPUT);
  ustras = get_us(USTRAS);
  usfrente = get_us(USFRENTE);
  usesq = get_us(USESQ);
  usdir = get_us(USDIR);
}

void update_ldr()
{
  ldr_esq = filtro(ldr_esq, analogRead(LDR_ESQ));
  ldr_dir = filtro(ldr_dir, analogRead(LDR_DIR));
}

void update_us()
{
  usfrente = filtro(usfrente, get_us(USFRENTE));
  ustras = filtro(ustras, get_us(USTRAS));
  usesq = filtro(usesq, get_us(USESQ));
  usdir = filtro(usdir, get_us(USDIR));
}

boolean SensorLDR(byte qualsensor) // lê o valor no LDR // passar LDR_ESQ ou LDR_DIR
{
  switch(qualsensor)
  {
    case LDR_ESQ:
      if(ldr_esq > COR_FITA) return HIGH;
      break;
    case LDR_DIR:
      if(ldr_dir > COR_FITA) return HIGH;
  }
  return LOW;
}

unsigned long SensorUS(byte qualsensor)
{
  switch(qualsensor)
  {
    case USTRAS:
      return (int)(ustras>>10);
    case USFRENTE:
      return (int)(usfrente>>10);
    case USESQ:
      return (int)(usesq>>10);
    case USDIR:
      return (int)(usdir>>10);
  }
}

unsigned long get_us(byte qualsensor) // passar USFRENTE ou USTRAS como parâmetro
{
  update_gyro();
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH); // ativa o sensor HC-SR04 com um pulso de 5 microssegundos no pino Trigger
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);
  
  unsigned long duration = 17*pulseIn(qualsensor, HIGH, 7000); // calcula o tempo necessário para o retorno do pulso sonoro
  //int distanceCentimeters = (duration * 17); // calcula a distância percorrida pelo pulso sonoro

  update_gyro();
  update_gyro();
  update_gyro();
  update_gyro();
  update_gyro();
  return duration; // retorna o valor encontrado
}
