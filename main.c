#include <stdio.h>
#include <stdlib.h>
#include "mVTipos.h"
#include "mVfuncionesTiposProtos.h"

int main(){
    FILE *archBinario;
    unsigned short int flagStop;
    char operacion;
    TOperando operandos[2];
    unsigned int ipActual,i;
    char *header =(char *)malloc(sizeof(char) * 6);
    t_funcion *vecFuncionesMV[]={mov,add,sub,intercambia,mul,div,cmp,shl,shr,and,or,xor,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    sys,jmp,jz,jp,jn,jnz,jnp,jnn,ldl,ldh,rnd,not};
    TMV mv;

    archBinario=fopen("fact.vmx","rb");

    fgets(header,6*sizeof(char),archBinario); //Obtengo el header
    fread(mv.TDD[0],sizeof(int),1,archBinario); //Leo el tama�o del codigo y asigno al DS

    int numInstruccion = 0;

    while(!feof(archBinario)){ //se lee el archivo binario para cargarlo en la memoria
        fread(mv.memoria[numInstrucciones++],sizeof(char),1,archBinario);
    }

    fclose(archBinario);

    mv.registros[5]=0; //IP == 0

    mascarasAInstruccuion(mv.memoria[mv.registros[5]],operando[0].tipo,operando[1].tipo,&operancion);

    while(mv.registros[5] < mv.TDD[1]){ // IP menor al DS
        sumaIP(&mv.registros[5],operando[0].tipo,operando[1].tipo);
        vecFuncionesMV[operancion](&mv,operandos);
        leePrimerByte(mv.memoria[mv.registros[5]],&(operando[0].tipo),&(operando[1].tipo),&operancion);
    }

    //disassebler
    return 0;
}
