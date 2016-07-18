unsigned long t_yaw = millis();
boolean faixa = 1; // Variável usada para saber qual faixa o robô está: 0 = esquerda, 1 = central.
//boolean desvio_incompleto = 0; // Variável usada para saber se o robô está em processo de desvio: 0 = não está, 1 = está desviando.
boolean corrige_esq = 0, corrige_dir = 0;


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
  unsigned long tmp;
  tmp = 17*(get_us(qualsensor)>>10);
  Serial.println(tmp);
  delay(1000);
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

void debug_condicao()
{
  if ((SensorUS(USFRENTE)+SensorUS(USTRAS)) < COMP_ARENA - COMP_ROBO)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void debug_pid()
{
  unsigned long t_start = millis();
  setmotoresq(150);
  setmotordir(150);
  
  while(millis() - t_start < 100)
  {
    update_gyro();
  }
  
  do{
     update_sensors();
     update_pid();
     verifica_obstaculo ();
     pseudobang();
  } while (1); //while(millis() - t_start < 2000);
  
  digitalWrite(LED_ESQ, LOW);
  digitalWrite(LED_DIR, LOW);
  setmotoresq(0);
  setmotordir(0);
  while(1);
}
/*
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
*/

void debug_mantem_faixa()
{
  corrige_dir = 0;
  corrige_esq = 0;
  if(faixa)
  {
    if(SensorLDR(LDR_ESQ) || SensorUS(USESQ) <= DIST_FAIXA_1)
    {
      corrige_dir = 1;
    } else if(SensorLDR(LDR_DIR) || SensorUS(USDIR) <= DIST_FAIXA_1) {
      corrige_esq = 1;
    }
  } else {
    if(SensorUS(USESQ) <= DIST_FAIXA_0)
    {
      corrige_dir = 1;
    } else if(SensorLDR(LDR_DIR) || SensorUS(USDIR) <= 55) {
      corrige_esq = 1;
    }
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
