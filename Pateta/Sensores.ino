int ldr_esq, ldr_dir;
unsigned long usfrente, ustras, usesq, usdir;
boolean ultrassom = 0;

void update_sensors()
{
  update_ldr();
  update_gyro();
  update_us();
}

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
}

void update_ldr()
{
  ldr_esq = analogRead(LDR_ESQ);
  ldr_dir = analogRead(LDR_DIR);
}

void update_us()
{
  if(ultrassom)
  {
    ustras = filtro(ustras, get_us(USTRAS));
    ultrassom = !ultrassom;
    usesq = filtro(usesq, get_us(USESQ));
  } else {
    usfrente = filtro(usfrente, get_us(USFRENTE));
    ultrassom = !ultrassom;
    usdir = filtro(usdir, get_us(USDIR));
  }
}

boolean SensorLDR(byte qualsensor) // lê o valor booleano do LDR // passar LDR_ESQ ou LDR_DIR
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

int SensorLDRRaw(byte qualsensor) //
{
  switch(qualsensor)
  {
    case LDR_ESQ:
      return ldr_esq;
      break;
    case LDR_DIR:
      return ldr_dir;
  }
}

unsigned long SensorUS(byte qualsensor)
{
  switch(qualsensor)
  {
    case USTRAS:
      return (17*ustras)>>10;
      //return ustras;
    case USFRENTE:
      return (17*usfrente)>>10;
      //return usfrente;
    case USESQ:
      return (17*usesq)>>10;
      //return usesq;
    case USDIR:
      return (17*usdir)>>10;
      //return usdir;
  }
}

unsigned long SensorUSRaw(byte qualsensor)
{
  switch(qualsensor)
  {
    case USTRAS:
      return (17*get_us(USTRAS))>>10;
      //return ustras;
    case USFRENTE:
      return (17*get_us(USFRENTE))>>10;
      //return usfrente;
    case USESQ:
      return (17*get_us(USESQ))>>10;
      //return usesq;
    case USDIR:
      return (17*get_us(USDIR))>>10;
      //return usdir;
  }
}

unsigned long get_us(byte qualsensor)
{
  digitalWrite(TRIGGER, LOW);
  //delayMicroseconds(4);
  digitalWrite(TRIGGER, HIGH); // ativa o sensor HC-SR04 com um pulso de 5 microssegundos no pino Trigger
  delayMicroseconds(5);
  digitalWrite(TRIGGER, LOW);
  
  unsigned long duration = pulseIn(qualsensor, HIGH, 25000); // calcula o tempo necessário para o retorno do pulso sonoro
 /*
  if(duration = 0)
  {
    switch(qualsensor)
    {
      case USTRAS:
        return (ustras);
      case USFRENTE:
        return (usfrente);
      case USESQ:
        return (usesq);
      case USDIR:
        return (usdir);
    }
  }
  */
  //int distanceCentimeters = (duration * 17)>>10; // calcula a distância percorrida pelo pulso sonoro
  unsigned long t_start = millis();
  
  while(millis() - t_start < TRIG_DELAY)
  {
    update_gyro();
  } 
   
  return duration; // retorna o valor encontrado
}

