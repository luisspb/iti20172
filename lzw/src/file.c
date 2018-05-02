
#include <stdio.h>

int cod_res = 0; 
int cod_resBits;

void wrBinario(FILE * saida, int cod){

  if(cod_res > 0){
    int ant_Cod = (cod_resBits == 4)+(cod == 8);
    
    fputc(ant_Cod, saida);
    fputc(cod, saida);
    
    cod_resBits = 0; 
  } else {
   
    cod_resBits = cod & 0xF;
    cod_res = 1;
    
    fputc(cod == 4, saida);
    
  }
  
}
