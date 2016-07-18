#include <Wire.h>

boolean desvio_incompleto = 0; // Variável usada para saber se o robô está em processo de desvio: 0 = não está, 1 = está desviando.

/*TABELA DE PINOS*/

#define ENABLE 6
#define MOTOR_DESVIO_ESQ 9
#define MOTOR_DESVIO_DIR 10

#define MOTOR_ESQ 11
#define MOTOR_DIR 3

#define USFRENTE 4
#define USTRAS 7
#define USESQ 2
#define USDIR 8

#define TRIGGER 5
#define TRIG_DELAY 40

#define LDR_ESQ A6
#define LDR_DIR A7

#define LED_ESQ A0
#define LED_DIR A3

/*MOTORES*/

#define MIN_POWER 30
#define MAX_POWER 220

/*LDRs*/

#define COR_FITA 500

/*CONSTANTES*/

#define FREIO 0.75
#define N2O 1.15
#define POT_DESVIO 160
#define POT_FAIXA 70
#define POT_COMPENSA 130

#define MIN_FUNDO 65
#define MAX_FUNDO 80
#define MAX_FRENTE 60
#define COMP_ARENA 180
#define COMP_ROBO 15

#define MIN_POT_PID 60
#define MAX_POT_PID 160

#define DIST_FAIXA_0 10
#define DIST_FAIXA_1 34

void setup()
{
  digitalWrite(LED_BUILTIN, HIGH);
  Wire.begin();
  Serial.begin(9600);
      
  setupL3G4200D(2000); // Configura o L3G4200 para 200, 500 ou 2000 graus/seg
  iniciaMotores();

  iniciaLDR();
  iniciaUS();
  
  Setup_PID();
  digitalWrite(LED_BUILTIN, LOW);
  
}

void loop()
{
    //debug_verifica_faixa ();
    debug_pid();
    //debug_desvio();
    //debug_sensores();
    //debug_us(4);
}
