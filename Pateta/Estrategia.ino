boolean faixa = 1; // Variável usada para saber qual faixa o robô está: 0 = esquerda, 1 = central.
boolean desvio_incompleto = 0; // Variável usada para saber se o robô está em processo de desvio: 0 = não está, 1 = está desviando.
boolean corrige_esq = 0, corrige_dir = 0;

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
  verifica_faixa();
}

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

void verifica_obstaculo()
{
  if(!desvio_incompleto)
  {
    desviar();
  } else {
    desviando();
  }
}

void desviar() {
  if ((SensorUS(USFRENTE)+SensorUS(USTRAS)) < COMP_ARENA - COMP_ROBO){ // Se a soma das distancia da frente e de trás não coincidirem com o tamanho da arena, ele desvia
    if (faixa == 1){                   // Se o robô está na faixa central
      setmotordesvioesq(POT_DESVIO);
      faixa = 0;
    }
    else {                             // Se o robô está na faixa da esquerda
      faixa = 1;
      setmotordesviodir(POT_DESVIO);
    }
    desvio_incompleto = 1;             // "Avisa" que desvio foi necessario e está em progresso
  }
}

void desviando() {
  if (faixa  == 0){   // Se o robô está indo para a faixa da esquerda
    if ((SensorUS(USESQ) < DIST_FAIXA_0)){   //Se o robô está proximo da parede esquerda e longe da direita, ele está na faixa
        stopmotordesvio();  // Desliga motor de desvio
        desvio_incompleto = 0;                // "Avisa" que desvio foi concluido
    }
  }
  else {    // Se o robô está indo para a faixa central
     if (SensorUS(USDIR) < DIST_FAIXA_1){   //Se o robô está proximo da parede direita e longe da esquerda, ele está na faixa
        stopmotordesvio();   // Desliga motor de desvio
        desvio_incompleto = 0;   // "Avisa" que desvio foi concluido
     }
  }
}

void pseudobang()
{
  int tras = SensorUS(USTRAS);
  int frente = SensorUS(USFRENTE);

  if(tras >= MAX_FUNDO || frente < MAX_FRENTE)   // se tá muito pra frente
  {
    if(corrige_esq)
    {
      setmotoresq(potesq() * FREIO);
      setmotordir(potdir());
    } else if(corrige_dir){
      setmotordir(potdir() * FREIO);
      setmotoresq(potesq());
    } else {
      setmotoresq(potesq() * FREIO);
      setmotordir(potdir() * FREIO);
    }
  } else {
    if(tras <= MIN_FUNDO) // se tá muito pra trás
    {
      if(corrige_esq)
      {
        setmotoresq(potesq());
        setmotordir(potdir() * N2O);
      } else if(corrige_dir){
        setmotordir(potdir());
        setmotoresq(potesq() * N2O);
      } else {
        setmotoresq(potesq() * N2O);
        setmotordir(potdir() * N2O);
      }
    } else {              // se tá sussa
      if(corrige_esq)
      {
        setmotoresq(potesq());
        setmotordir(potdir() * N2O);
      } else if(corrige_dir){
        setmotordir(potdir());
        setmotoresq(potesq() * N2O);
      } else {
        setmotoresq(potesq());
        setmotordir(potdir());
      }
    }
  }
}

void mantem_faixa()
{
  corrige_dir = 0;
  corrige_esq = 0;
  if(faixa)
  {
    if(SensorLDR(LDR_ESQ) || SensorUS(USESQ) <= DIST_FAIXA_1)
    {
      corrige_dir = 1;
    } else if(SensorLDR(LDR_DIR)) {
      corrige_esq = 1;
    }
  } else {
    if(SensorUS(USESQ) <= DIST_FAIXA_0)
    {
      corrige_dir = 1;
    } else if(SensorLDR(LDR_DIR)) {
      corrige_esq = 1;
    }
  }
}

