/*
int setor()
{
  update_gyro();
  double gyro = get_gyro();
  if(gyro < 90 && gyro > 75) return 1;
  if(gyro < 75 && gyro > 60) return 2;
  if(gyro < 60 && gyro > 45) return 3;
  if(gyro < 45 && gyro > 30) return 4;
  if(gyro < 30 && gyro > 15) return 5;
  if(gyro < 15 && gyro > 5) return 6;
  if(gyro < 5 && gyro > -5) return 7;
  if(gyro < -5 && gyro > -15) return 8;
  if(gyro < -15 && gyro > -30) return 9;
  if(gyro < -30 && gyro > -45) return 10;
  if(gyro < -45 && gyro > -60) return 11;
  if(gyro < -60 && gyro > -75) return 12;
  if(gyro < -75 && gyro > -90) return 13;
}

byte potesq()
{
  int setor = setor();
  switch(setor)
  {
    case 1:
      return 255;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7: //potencia pra frente certinho
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
      return 0;    
  }
}

byte potdir()
{
  int setor = setor();
  switch(setor)
  {
    case 1:
      return 0;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      return //potencia pra frente certinho
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
      return 255;   
  }
}
*/
