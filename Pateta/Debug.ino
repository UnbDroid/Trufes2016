void debug_sensores()
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

void debug_gyro()
{
  unsigned long t_start = micros();
  update_gyro();
  Serial.println(micros() - t_start);
}
