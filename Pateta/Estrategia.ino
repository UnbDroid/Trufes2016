#define coisa 0
#define POW_MORE 100
#define POW_LESS 0
#define TEMPO_PORCO 700
#define MAIS_PORCO 10


//boolean faixa = 1; // Variável usada para saber qual faixa o robô está: 0 = esquerda, 1 = central.
//boolean corrige_esq = 0, corrige_dir = 0;

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
//  verifica_faixa();
}

void esperaobstaculo()
{
  unsigned long t_start_desvio = millis();
  while(/*SensorUS(USDIR) > 20 && */(millis() - t_start_desvio) < 2500)
  {
    update_sensors ();
    update_pid();
    mantem_faixa();
    pseudobang();
  }
  while(/*SensorUS(USDIR) < 20 && */(millis() - t_start_desvio) < 2500)
  {
    update_sensors ();
    update_pid();
    mantem_faixa();
    pseudobang();
  }
}

void desvio()
{
  desvio_esq();
  esperaobstaculo();
  desvio_dir();
  desvio_incompleto = 0;
  faixa = 1;
}

void desvio_esq () {
      setmotordir(POW_MORE);
      setmotoresq(POW_LESS);
      setmotordesviodir (POT_DESVIO);
      update_gyro ();
      while(get_gyro () < 70)
      {
        update_gyro();
      }
      
      setmotoresq(POW_MORE);
      setmotordir(POW_LESS);
      setmotordesvioesq (POT_DESVIO);
      
      while(get_gyro () > 30)
      {
        update_gyro();
      }
      
      offmotoresq();
      offmotordir();
      stopmotordesvio();
      //while (1);
}
void desvio_dir (){      

      setmotoresq(POW_MORE);
      setmotordir(POW_LESS);
      setmotordesvioesq (POT_DESVIO);
      
      while(get_gyro () > -70)
      {
        update_gyro();
      }
      setmotordir(POW_MORE);
      setmotoresq(POW_LESS);
      setmotordesviodir (POT_DESVIO);
      
      while(get_gyro () < -30)
      {
        update_gyro();
      }
      offmotoresq();
      offmotordir();
      stopmotordesvio();
      desvio_incompleto = 0;
}

void verifica_obstaculo()
{
  if(!desvio_incompleto)
  {
 
    /*
    update_pid();
    if(corrige_esq)
    {
      setmotoresq(potesq());
      setmotordir(potdir() + 10);
    } else if(corrige_dir) {
      setmotoresq(potesq() + 10);
      setmotordir(potdir());
    } else {
      setmotoresq(potesq());
      setmotordir(potdir());
    }
    */
    //debug_mantem_faixa (); //[[[DESCOMENTAR AQUI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
    desviar();
    
    //debug_desviar ();

  } else {
    desvio();
    //desviando();//[[[ARRUMAR AQUI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
  }
}

void debug_verifica_obstaculo () {
  desvio_incompleto = 1;
  desvio_sono_porco ();
  faixa = 0;
}

void debug_desviar() {
  unsigned long frente = SensorUSRaw(USFRENTE);
  
  if (frente < 50 && frente != 0){ // Se a soma das distancia da frente e de trás não coincidirem com o tamanho da arena, ele desvia
    faixa = !faixa;
    desvio_incompleto = 1;             // "Avisa" que desvio foi necessario e está em progresso
  }
    
    stopmotordesvio();
   /* 
    offmotoresq();
    offmotordir();
  */
}


void desviar() {
  if ((SensorUS(USFRENTE)+SensorUS(USTRAS)) < COMP_ARENA - COMP_ROBO){ // Se a soma das distancia da frente e de trás não coincidirem com o tamanho da arena, ele desvia
    faixa = !faixa;
    desvio_incompleto = 1;             // "Avisa" que desvio foi necessario e está em progresso
  }
  stopmotordesvio();
}
/*
void desviando() {
  if (faixa  == 0){   // Se o robô está indo para a faixa da esquerda
    if ((SensorUS(USESQ) < DIST_FAIXA_0)){   //Se o robô está proximo da parede esquerda e longe da direita, ele está na faixa
        stopmotordesvio();  // Desliga motor de desvio
        desvio_incompleto = 0; // "Avisa" que desvio foi concluido
        
        setmotoresq(80);
        setmotordir(100);
        solucaoporca ();
        
    }
  }
  else {    // Se o robô está indo para a faixa central
     if (SensorUS(USDIR) < DIST_FAIXA_1){   //Se o robô está proximo da parede direita e longe da esquerda, ele está na faixa
        stopmotordesvio();   // Desliga motor de desvio
        desvio_incompleto = 0;   // "Avisa" que desvio foi concluido
        
        setmotoresq(100);
        setmotordir(80);
        solucaoporca ();
     }
  }
}

void solucaoporca () {
  //Tentariva de solução porca ----------------        
        unsigned long t_start = millis();
        while(millis() - t_start < 100)
        {
          update_gyro();
        }
        setmotoresq(0);
        setmotordir(0);
        t_start = millis();
        while(millis() - t_start < 500)
        {
          update_gyro();
        }
        //---------------------------------------------
}
*/
void desvio_sono_porco () {
  if (desvio_incompleto == 1 && faixa == 0){
    desvio_esq ();
  }
  else if (desvio_incompleto == 1 && faixa == 1){
    desvio_dir ();
  }
}

void pseudobang()
{
  // ==============================================se tá muito pra frente======================================
  if(SensorUS (USTRAS) >= MAX_FUNDO)  // se tá muito pra frente
  {
      setmotoresq(FREIO_DE_MAO);
      setmotordir(FREIO_DE_MAO);
  }

//======================================== se tá muito pra trás================================================ 
  else if(SensorUS (USTRAS) <= MIN_FUNDO) // se tá muito pra trás
      {
      //----------------------------------------------Correção na faixa----------------------------------------------
      if(corrige_esq)
      {
        setmotoresq(potesq());
        setmotordir(potdir() * (N2O-coisa));
      } 
      else if(corrige_dir){
        setmotordir(potdir());
        setmotoresq(potesq() * (N2O+coisa));
      } 
      //----------------------------------------------NITRO------------------------------------------------------
      else {
        unsigned long t_start = millis();
        while((millis() - t_start) < 50)
        {
          update_sensors();
          update_pid();
          setmotoresq(potesq() * TURBO);
          setmotordir(potdir() * TURBO);
        }
        setmotoresq(potesq());
        setmotordir(potdir());
      }
    } 

//=============================================== se tá sussa ===================================================
     else {              // se tá sussa
      //-------------------------------------------Desvio------------------------------------------------
     //desvio_sono_porco ();   
     //----------------------------------------------Correção na faixa---------------------------------------------------
     if(corrige_esq)
      {
        setmotoresq(potesq());
        setmotordir(potdir() * N2O);
      } 
      else if(corrige_dir){
        setmotordir(potdir());
        setmotoresq(potesq() * N2O);
      } 
      //----------------------------------------------nada---------------------------------------------------------
      else {
        setmotoresq(potesq());
        setmotordir(potdir());
      }
    }
}
/*
void pseudobangantigo()
{
// ==============================================se tá muito pra frente======================================
  if(SensorUS (USTRAS) >= MAX_FUNDO)  // se tá muito pra frente
  {
    //-------------------------------------------Desvio-----------------------------------------------------
    //desvio_sono_porco (); 
    //----------------------------------------------Correção na faixa---------------------------------------------
    if(corrige_esq)
    {
      setmotoresq(potesq() * FREIO * FREIO);
      setmotordir(potdir()* (FREIO-coisa));} 
    else if(corrige_dir){
      setmotordir(potdir() * FREIO* FREIO);
      setmotoresq(potesq()* (FREIO+coisa));} 
    //----------------------------------------------Freio-----------------------------------------------------
    else {
      setmotoresq(potesq() * FREIO_DE_MAO);
      setmotordir(potdir() * FREIO_DE_MAO);}
  }

//======================================== se tá muito pra trás================================================ 
  else if(SensorUS (USTRAS) <= MIN_FUNDO) // se tá muito pra trás
      {
      //-------------------------------------------Desvio----------------------------------------------------- 
      //desvio_sono_porco ();
      //----------------------------------------------Correção na faixa----------------------------------------------
      if(corrige_esq)
      {
        setmotoresq(potesq());
        setmotordir(potdir() * (N2O-coisa));
      } 
      else if(corrige_dir){
        setmotordir(potdir());
        setmotoresq(potesq() * (N2O+coisa));
      } 
      //----------------------------------------------NITRO------------------------------------------------------
      else {
        setmotoresq(potesq() * TURBO);
        setmotordir(potdir() * TURBO);
      }
    } 

//=============================================== se tá sussa ===================================================
     else {              // se tá sussa
      //-------------------------------------------Desvio------------------------------------------------
     //desvio_sono_porco ();   
     //----------------------------------------------Correção na faixa---------------------------------------------------
     if(corrige_esq)
      {
        setmotoresq(potesq());
        setmotordir(potdir() * N2O);
      } 
      else if(corrige_dir){
        setmotordir(potdir());
        setmotoresq(potesq() * N2O);
      } 
      //----------------------------------------------nada---------------------------------------------------------
      else {
        setmotoresq(potesq());
        setmotordir(potdir());
      }
    }
}
*/
void mantem_faixa()
{
  corrige_dir = 0;
  corrige_esq = 0;
  if(faixa)
  {
    if((SensorUS(USESQ) < 32 && SensorUS(USDIR) > 32) || SensorLDR(LDR_ESQ)  || get_gyro() > 6)
    {
      corrige_dir = 1;
    } 
    else if((SensorUS(USDIR) < 32 && SensorUS(USESQ) > 32) || SensorLDR(LDR_DIR)  || get_gyro() < -6) 
    {
      corrige_esq = 1;
    }
  } else {
    if(SensorUS(USESQ) <= 6 || get_gyro() > 4)
    {
      corrige_dir = 1;
    } else if(SensorUS(USESQ) >= 8 || SensorLDR(LDR_DIR)  || get_gyro() < -4) {
      corrige_esq = 1;
    }
  }
}

