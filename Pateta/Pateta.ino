#include <Wire.h>

/*TABELA DE PINOS*/

#define ENABLE 6
#define MOTOR_DESVIO_ESQ 10
#define MOTOR_DESVIO_DIR 9

#define MOTOR_ESQ 11
#define MOTOR_DIR 3

#define USFRENTE 4
#define USTRAS 8
#define USESQ 2
#define USDIR 7
#define TRIGGER 5

#define LDR_ESQ A6
#define LDR_DIR A7

#define LED_ESQ A0
#define LED_DIR A3

/*MOTORES*/

#define MIN_POWER 0
#define MAX_POWER 240

/*ULTRASSONS*/

#define TRIG_DELAY 20

/*LDRs*/

#define COR_FITA 600

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  setupL3G4200D(2000); // Configura o L3G4200 para 200, 500 ou 2000 graus/seg
  iniciaMotores();
  
  iniciaLDR();
  iniciaUS();

  Setup_PID();
}

void loop()
{
   unsigned long t_start = micros();
   for(int i; 
   /*
   unsigned long t_start = millis();
   do{
     update_pid();
     setmotoresq(potesq());
     setmotordir(potdir());
   } while(millis() - t_start < 2500);
   for(int i = 80; i >= 0; --i)
   {
    setmotoresq(i);
    setmotordir(i);
    delay(15);
   }
   while(1);
   */
}
