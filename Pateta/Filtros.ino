int filtro(int media, int leitura) {
  int tmpdiff = leitura-media;

  if(abs(tmpdiff)>(media>>1)) {
     media = media - (media>>3) + (leitura>>3);
     return media;
  }
  media = (media>>2) + leitura - (leitura>>2);
  return media;
}
