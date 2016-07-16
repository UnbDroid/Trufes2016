void iniciaMotores() // inicializa os pinos dos motores
{
  
  pinMode(ENABLE, OUTPUT);
  pinMode(MOTOR_DESVIO_ESQ, OUTPUT);
  pinMode(MOTOR_DESVIO_DIR, OUTPUT);
  pinMode(MOTOR_ESQ, OUTPUT);
  pinMode(MOTOR_DIR, OUTPUT);
}

void setmotoresq(int potencia)
{
  if(potencia > MAX_POWER) potencia = MAX_POWER;
  if(potencia < MIN_POWER) potencia = MIN_POWER;
  analogWrite(MOTOR_ESQ, potencia);
}

void setmotordir(int potencia)
{
  if(potencia > MAX_POWER) potencia = MAX_POWER;
  if(potencia < MIN_POWER) potencia = MIN_POWER;
  analogWrite(MOTOR_DIR, potencia);
}

void setmotordesvioesq(){
  digitalWrite (MOTOR_DESVIO_ESQ, LOW);
  analogWrite (MOTOR_DESVIO_DIR, POT_DESVIO);
}
void setmotordesviodir(){
  digitalWrite (MOTOR_DESVIO_DIR, LOW);
  analogWrite (MOTOR_DESVIO_ESQ, POT_DESVIO);
}

void stopmotordesvio(){
  digitalWrite(MOTOR_DESVIO_DIR, LOW);
  digitalWrite(MOTOR_DESVIO_ESQ, LOW);
}

/*
void setmotordesvio(int potencia)
{
  if(potencia >= 0)
  {
    //if(potencia > MAX_POWER) potencia = MAX_POWER;
    //if(potencia < MIN_POWER) potencia = MIN_POWER;
    digitalWrite(MOTOR_DESVIO_ESQ, LOW);
    analogWrite(MOTOR_DESVIO_DIR, potencia);
  } else {
    potencia = -potencia;
    //if(potencia > MAX_POWER) potencia = MAX_POWER;
    //if(potencia < MIN_POWER) potencia = MIN_POWER;
    digitalWrite(MOTOR_DESVIO_DIR, LOW);
    analogWrite(MOTOR_DESVIO_ESQ, potencia);
  }
}
*/

