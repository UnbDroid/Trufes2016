boolean faixa = 1; // Variável usada para saber qual faixa o robô está: 0 = esquerda, 1 = central.

void desviar () {
  if ((SensorUS(USFRENTE)+SensorUS(USTRAS)) < COMP_ARENA - COMP_ROBO){ // Se a soma das distancia da frente e de trás não coincidirem com o tamanho da arena, ele desvia
    if (faixa == 1){                   // Se o robô está na faixa central
      setmotordesvioesq();
      faixa = 0;
    }
    else {                             // Se o robô está na faixa da esquerda
      faixa = 1;
      setmotordesviodir();
    } 
    desvio_incompleto = 1;             // "Avisa" que desvio foi necessario e está em progresso
  }
}

void desviando () {
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
  if(tras <= MIN_FUNDO)   // se tá muito pra trás
  {
    setmotoresq(potesq() + N20);
    setmotordir(potdir() + N20);
  } else {
    if(tras >= MAX_FUNDO) // se tá muito pra frente
    {
      setmotoresq(potesq() - FREIO);
      setmotordir(potdir() - FREIO);
    } else {              // se tá sussa
      setmotoresq(potesq());
      setmotordir(potdir());
    }
  }
}

