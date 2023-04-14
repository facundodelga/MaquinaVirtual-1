
#include "mVTipos.h"

typedef struct{
    char tipo;
    char registro; //eax ds
    char segmentoReg; //AH AL ó cantidad de bytes que vamos a leer
    char memoria[4];
    int desplazamiento; //desplazamiento de memoria y tipo inmediato
}TOperando;

int getOp(TMV *,TOperando );
void setOp(TMV *,TOperando ,int );
int getReg(TMV *,TOperando );
int getMem(TMV *,TOperando );
void recuperaOperandos(TMV *,TOperando *,int ); //mv, vector de operandos, ip
