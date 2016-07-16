void debug_sensores()
{
   unsigned long t_start = micros();
   for(int i = 0; i < 128; ++i)
   {
     update_us();
     update_ldr();
     update_pid();
     Serial.println("USFRENTE:"+String(SensorUS(USFRENTE))+" USTRAS:"+String(SensorUS(USTRAS))+" USESQ:"+String(SensorUS(USESQ))+" USDIR:"+String(SensorUS(USDIR))+" LDR_ESQ:"+String(SensorLDR(LDR_ESQ))+" LDR_DIR:"+String(SensorLDR(LDR_DIR))+" YAW:"+String(get_gyro()));
     if(SensorLDR(LDR_ESQ) || SensorLDR(LDR_DIR))
     {
      digitalWrite(LED_BUILTIN, HIGH);
     } else {
      digitalWrite(LED_BUILTIN, LOW);
     }
   }
   Serial.println((micros() - t_start)>>7);
}

void debug_gyro()
{
  unsigned long t_start = micros();
  update_gyro();
  Serial.println(micros() - t_start);
}

void debug_pid()
{
  unsigned long t_start = millis();
  do{
     update_pid();
     setmotoresq(potesq());
     setmotordir(potdir());
  } while(millis() - t_start < 4000);
  digitalWrite(LED_ESQ, LOW);
  digitalWrite(LED_DIR, LOW);
  for(int i = MIN_POT_PID; i >= 0; --i)
  {
   setmotoresq(i);
   setmotordir(i);
   delay(15);
  }
  while(1);
}

