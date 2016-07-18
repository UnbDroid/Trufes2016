unsigned long t_yaw = millis();

void debug_sensores()
{
   //unsigned long t_start = micros();
   //for(int i = 0; i < 128; ++i)
   { 
     update_sensors();
     update_pid();
     Serial.println("USFRENTE:"+String(SensorUS(USFRENTE))+" USTRAS:"+String(SensorUS(USTRAS))+" USESQ:"+String(SensorUS(USESQ))+" USDIR:"+String(SensorUS(USDIR))+" LDR_ESQ:"+String(SensorLDR(LDR_ESQ))+" LDR_DIR:"+String(SensorLDR(LDR_DIR))+" YAW:"+String(get_gyro()));
     
     if(SensorLDR(LDR_ESQ) || SensorLDR(LDR_DIR))
     {
      digitalWrite(LED_BUILTIN, HIGH);
     } else {
      digitalWrite(LED_BUILTIN, LOW);
     }
     
     //if (get_gyro() != 0) {
     // Serial.println(String(millis() - t_yaw)+" "+get_gyro()); 
     //}
   }
   
   
}

void debug_us(int qualsensor)
{
  switch(qualsensor)
  {
    case USTRAS:
      Serial.println(17*(get_us(USTRAS))>>10);
    case USFRENTE:
      Serial.println(17*(get_us(USFRENTE))>>10);
    case USESQ:
      Serial.println(17*(get_us(USESQ))>>10);
    case USDIR:
      Serial.println(17*(get_us(USDIR))>>10);
  }
  delay(500);
}

void debug_time_gyro()
{
  unsigned long t_start = micros();
  update_gyro();
  Serial.println(String(micros() - t_start)+" "+String(get_gyro()));
}

void debug_time_pid()
{
  unsigned long t_start = micros();
  update_pid();
  Serial.println(micros() - t_start);
}

void debug_pid()
{
  unsigned long t_start = millis();
  update_gyro();
  setmotoresq(150);
  setmotordir(150);
  delay (100);
  //setmotordesvioesq ();
  do{
     update_sensors();
     update_pid();
     mantem_faixa();
     pseudobang();
     //debug_verifica_faixa();
     //setmotoresq(potesq());
     //setmotordir(potdir());
  } while (1);//while(millis() - t_start < 5000);
  
  digitalWrite(LED_ESQ, LOW);
  digitalWrite(LED_DIR, LOW);
  //stopmotordesvio ();
  setmotoresq(0);
  setmotordir(0);
 /*
  for(int i = MIN_POT_PID; i >= 0; --i)
  {
   setmotoresq(i);
   setmotordir(i);
   delay(40);
  }
  */
   
  //stopmotordesvio();
  while(1);
}

void debug_desvio()
{
  int esq = SensorUSRaw(USESQ);
  int dir = SensorUSRaw(USDIR);
  setmotoresq(20);
  setmotordir(20);
  if((dir < 10 && dir != 0) || (esq < 10 && esq != 0))
  {
    if(dir < 10 && dir != 0) {
      setmotordesvioesq(POT_DESVIO);
      setmotoresq(0);
      setmotordir(POT_COMPENSA);
    } else if (esq < 10 && esq != 0) {
      setmotordesviodir(POT_DESVIO);
      setmotoresq(POT_COMPENSA);
      setmotordir(0);
    }
  } else {
    stopmotordesvio();
    setmotoresq(0);
    setmotordir(0);
  }
}

/*
void debug_verifica_faixa()
{
  update_sensors ();
  if(!desvio_incompleto)                // Só executa se o robô não está desviando
  {
    if(faixa)                           // Se está na faixa do meio.
    {
      if(SensorLDR(LDR_ESQ) || SensorUS(USESQ) < DIST_FAIXA_1)            // Se está escapando pra a esquerda.
      {
        setmotordesvioesq(POT_FAIXA);
        //setmotoresq(POT_COMPENSA);
        //setmotordir(0);
      } else if(SensorLDR(LDR_DIR) || SensorUS(USDIR) < DIST_FAIXA_1) {   // Se está escpanado pra a direita.
        setmotordesviodir(POT_FAIXA);
        //setmotoresq(0);
        //setmotordir(POT_COMPENSA);
      } else {
        stopmotordesvio();
      }
    } else {                            // Se está na faixa da esquerda
      if(SensorUS(USESQ) < DIST_FAIXA_0)
      {
        setmotordesvioesq(POT_FAIXA);
      } else if(SensorLDR(LDR_DIR)) {
        setmotordesviodir(POT_FAIXA);
      } else {
        stopmotordesvio();
      }
    }
  }
} 
*/
