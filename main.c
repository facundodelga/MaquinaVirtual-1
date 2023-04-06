#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE archBinario;
    unsigned short int flagStop;
    char tipoOperando1,tipoOperando2,operacion;
    unsigned int ipActual,i;

    archBinario=fopen("traduccion.vmx","rb");
    fgets(mv.memoria[numInstrucciones],6*sizeof(char),archBinario);
    fgets(mv.TDD[0],2*sizeof(char),archBinario);
    //se lee el archivo binario para cargarlo en la memoria
    while(!feof(archBinario)){
        numInstrucciones++;
        fgets(mv.memoria[numInstrucciones],sizeof(char),archBinario);
    }
    fclose(archBinario);
    mv.registros[5]=0;
    mascarasAInstruccuion(mv.memoria[i],&operando1,&operando2,&operancion,flagStop);
    while(mv.TDD[1]!=mv.registros[5] && !flagStop){
        ipActual=mv.registro[5];
        sumaIP(&mv.registros[5],tipoOperando1,tipoOperando2); para que? igual acordate que los operando ya los tenes
        for(ipActual;ipActual<mv.registros[5];ipactual++){

        }
        funcionesMV[operancion](operando1,operando2);
        mascarasAInstruccuion(mv.memoria[mv.registros[5]],&tipoOperando1,&tipoOperando2,&operancion,flagStop);
    }
    //disassebler
    return 0;
}
