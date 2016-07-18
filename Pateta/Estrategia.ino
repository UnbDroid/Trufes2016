boolean faixa = 1; // Variável usada para saber qual faixa o robô está: 0 = esquerda, 1 = central.
boolean corrige_esq = 0, corrige_dir = 0;
#define coisa 0.07

void atualiza_tudo()
{
  update_sensors();
  update_pid();
}

void gogogo()
{
  atualiza_tudo();
  pseudobang();
  verifica_obstaculo();
  //verifica_faixa();
}
/*
void verifica_faixa()
{
  if(!desvio_incompleto)                // Só executa se o robô não está desviando
  {
    if(faixa)                           // Se está na faixa do meio.
    {
      if(SensorLDR(LDR_ESQ) || SensorUS(USESQ) < 31)            // Se está escapando pra a esquerda.
      {
        setmotordesvioesq(POT_FAIXA);
        //setmotoresq(POT_COMPENSA);
        //setmotordir(0);
      } else if(SensorLDR(LDR_DIR) || SensorUS(USDIR) < 31) {   // Se está escpanado pra a direita.
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
void verifica_obstaculo()
{
  if(!desvio_incompleto)
  {
    mantem_faixa ();
    desviar();
  } else {
    desviando();
  }
}

void desviar() {
  if ((SensorUS(USFRENTE)+SensorUS(USTRAS)) < COMP_ARENA - COMP_ROBO){ // Se a soma das distancia da frente e de trás não coincidirem com o tamanho da arena, ele desvia
    if (faixa == 1){                   // Se o robô está na faixa central
      setmotordesviodir (POT_DESVIO);
      faixa = 0;
    }
    else {                             // Se o robô está na faixa da esquerda
      turn_right ();  
      faixa = 1;
      setmotordesvioesq (POT_DESVIO);
    }
    desvio_incompleto = 1;             // "Avisa" que desvio foi necessario e está em progresso
  }
}

void desviando() {
  if (faixa  == 0){   // Se o robô está indo para a faixa da esquerda
    if ((SensorUS(USESQ) < DIST_FAIXA_0)){   //Se o robô está proximo da parede esquerda e longe da direita, ele está na faixa
        //turn_left ();       
        stopmotordesvio();  // Desliga motor de desvio
        desvio_incompleto = 0;                // "Avisa" que desvio foi concluido
    }
  }
  else {    // Se o robô está indo para a faixa central
     if (SensorUS(USDIR) < DIST_FAIXA_1){   //Se o robô está proximo da parede direita e longe da esquerda, ele está na faixa
        //turn_right ();
        stopmotordesvio();   // Desliga motor de desvio
        desvio_incompleto = 0;   // "Avisa" que desvio foi concluido
     }
  }
}

void pseudobang()
{
  int tras = SensorUS(USTRAS);
  int frente = SensorUS(USFRENTE);

  if(tras >= MAX_FUNDO) //|| frente < MAX_FRENTE)   // se tá muito pra frente
  {
    if(corrige_esq)
    {
      setmotoresq(potesq() * FREIO * FREIO);
      setmotordir(potdir()* (FREIO-coisa));} 
    else if(corrige_dir){
      setmotordir(potdir() * FREIO* FREIO);
      setmotoresq(potesq()* (FREIO+coisa));} 
    else {
      setmotoresq(potesq() * FREIO* FREIO);
      setmotordir(potdir() * FREIO* FREIO);}
  } 
  else if(tras <= MIN_FUNDO) // se tá muito pra trás
    {
      if(corrige_esq)
      {
        setmotoresq(potesq());
        setmotordir(potdir() * (N2O-coisa));
      } 
      else if(corrige_dir){
        setmotordir(potdir());
        setmotoresq(potesq() * (N2O+coisa));
      } 
      else {
        setmotoresq(potesq() * N2O);
        setmotordir(potdir() * N2O);
      }
    } 
    else {              // se tá sussa
      if(corrige_esq)
      {
        setmotoresq(potesq());
        setmotordir(potdir() * N2O);
      } 
      else if(corrige_dir){
        setmotordir(potdir());
        setmotoresq(potesq() * N2O);
      } 
      else {
        setmotoresq(potesq());
        setmotordir(potdir());
      }
    }
}

void mantem_faixa()
{
  corrige_dir = 0;
  corrige_esq = 0;
  if(faixa)
  {
    if(SensorUS(USESQ) < 34 && SensorUS(USDIR) > 35)
    {
      corrige_dir = 1;
    } 
    else if(SensorUS(USDIR) < 34 && SensorUS(USESQ) > 35) 
    {
      corrige_esq = 1;
    }
  } else {
    if(SensorUS(USESQ) <= 5)
    {
      corrige_dir = 1;
    } else if(SensorUS(USESQ) >= 8 ) {
      corrige_esq = 1;
    }
  }
}

