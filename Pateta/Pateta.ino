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

/*LDRs*/

#define COR_FITA 600

/*CONSTANTES*/

#define FREIO 10
#define N20 10
#define POT_DESVIO 127

#define MIN_FUNDO 70
#define MAX_FUNDO 100 
#define COMP_ARENA 130
#define COMP_ROBO 25

#define DIST_FAIXA_0 5
#define DIST_FAIXA_1 35

boolean desvio_incompleto = 0; // Variável usada para saber se o robô está em processo de desvio: 0 = não está, 1 = está desviando.

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
   for(int i = 0; i < 128; ++i)
   {
    debug_sensores();
   }
   Serial.println((micros() - t_start)>>7);
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
