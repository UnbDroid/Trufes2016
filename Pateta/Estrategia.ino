#define DIST_ARENA 130

#define DIST_FAIXA_0 5
#define DIST_FAIXA_1 35

boolean faixa = 1;              // Variável usada para saber qual faixa o robô está: 0 = esquerda, 1 = central.
boolean desvio_incompleto = 0; // Variável usada para saber se o robô está em processo de desvio: 0 = não está, 1 = está desviando.

void desviar () {
  if ((usfrente+ustras) < DIST_ARENA){        // Se a soma das distancia da frente e de trás não coincidirem com o tamanho da arena, ele desvia
    if (faixa == 1){                          // Se o robô está na faixa central
      faixa = 0;
    }
    else {                                    // Se o robô está na faixa da esquerda
      faixa = 1;
    } 
    desvio_incompleto = 1;                   // "Avisa" que desvio foi necessario e está em progresso
  }
}

void desviando () {
  if (faixa  == 0){   // Se o robô está indo para a faixa da esquerda
    if ((usesq < DIST_FAIXA_0)){   //Se o robô está proximo da parede esquerda e longe da direita, ele está na faixa
        digitalWrite (MOTOR_DESVIO_ESQ, LOW);  // Desliga motor de desvio
        desvio_incompleto = 0;                // "Avisa" que desvio foi concluido
    }
  }
  else {    // Se o robô está indo para a faixa central
     if (usdir < DIST_FAIXA_1){   //Se o robô está proximo da parede direita e longe da esquerda, ele está na faixa
        digitalWrite (MOTOR_DESVIO_DIR, LOW);   // Desliga motor de desvio
        desvio_incompleto = 0;   // "Avisa" que desvio foi concluido
     }
  }
}
