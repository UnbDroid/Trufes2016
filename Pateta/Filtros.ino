int filtro(int media, int leitura) {
  if(leitura == 0)
  {
    return media;
  }
  
  int tmpdiff = leitura-media;

  if(abs(tmpdiff)>(media>>1))
  {
     media = media - (media>>2) + (leitura>>2);
  } else {
     media = (media>>2) + leitura - (leitura>>2);
  }
  return media;
}
