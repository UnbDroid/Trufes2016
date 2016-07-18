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
  setmotoresq(100);
  setmotordir(100);
  update_gyro();
  delay (50);
  update_gyro();
  do{
     update_sensors();
     update_pid();
     //mantem_faixa ();
     verifica_obstaculo();
     pseudobang();
  } while (1); //while(millis() - t_start < 2000);
  
  digitalWrite(LED_ESQ, LOW);
  digitalWrite(LED_DIR, LOW);
  setmotoresq(0);
  setmotordir(0);
  while(1);
}

void debug_desvio()
{
  
  turn_left ();
  setmotordesviodir (POT_DESVIO);
  //faixa = 0;
  
  if (SensorUS(USDIR) < DIST_FAIXA_1){   //Se o robô está proximo da parede direita e longe da esquerda, ele está na faixa
        turn_right ();
        stopmotordesvio();   // Desliga motor de desvio
        //desvio_incompleto = 0;   // "Avisa" que desvio foi concluido
   }
}

void debug_desviar () {
  if (desvio_incompleto == 0){
    mantem_faixa ();
    desviar ();  
  }
  else {
    desviando ();
  }
}


