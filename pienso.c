#include "mVTipos.h"

typedef void t_funcion(TMV * mv,char tOp1,char tOp2,int ipAct);

void mov(TMV * mv,char tOp1,char tOp2,int ipAct){

    unsigned int registro,registroOp2;
    char sectorDeRegistro;
    unsigned int valor,valor2;
    char offset,offset2;
    ipAct++;

    if(tOp1 == 0x00){ // Operando 1 de tipo Memoria
        registro = (unsigned int) mv->memoria[ipAct];
        ipAct +=2;

        if(tOp2 == 0x00){ //El operando 2 es de tipo Memoria
            registroOp2 = mv->memoria[ipAct];

            mv->memoria[mv->TDD[0] + registro] = mv->memoria[mv->TDD[0] + registroOp2];
            mv->memoria[mv->TDD[0] + registro + 1] = mv->memoria[mv->TDD[0] + registroOp2 + 1];

        }else{
            if(tOp2 == 0x01){ // El operando 2 es de tipo Inmediato

                mv->memoria[mv->TDD[0] + registro] = mv->memoria[ipAct++];
                mv->memoria[mv->TDD[0] + registro + 1] = mv->memoria[ipAct];

            }else{ // El operando 2 es de tipo Registro

                //Hay que aplicar mascaras para saber que cantidad de bits se extraen del registro
                registroOp2 = mv->memoria[ipAct];
                sectorDeRegistro = (registroOp2 & 0x30) >> 6;
                registroOp2 = registroOp2 & 0x0F;

                if(sectorDeRegistro == 0x01) //4to byte del registro

                    mv->memoria[mv->TDD[0] + registro] = mv->registros[registroOp2] & 0x000000FF;
                else{
                    if(sectorDeRegistro == 0x10) //3er byte del registro
                        mv->memoria[mv->TDD[0] + registro] = mv->registros[registroOp2] & 0x0000FF00;
                    else{
                        if(sectorDeRegistro == 0x11){ // registro de 2 bytes

                            mv->memoria[mv->TDD[0] + registro] = mv->registros[registroOp2] & 0x0000FF00;
                            mv->memoria[mv->TDD[0] + registro + 1] = mv->registros[registroOp2] & 0x000000FF;
                        }else{ //registro de 4 bytes

                            mv->memoria[mv->TDD[0] + registro] = mv->registros[registroOp2] & 0xFF000000;
                            mv->memoria[mv->TDD[0] + registro + 1] = mv->registros[registroOp2] & 0x00FF0000;
                            mv->memoria[mv->TDD[0] + registro + 2] = mv->registros[registroOp2] & 0x0000FF00;
                            mv->memoria[mv->TDD[0] + registro + 3] = mv->registros[registroOp2] & 0x000000FF;
                        }
                    }
                }
            }
        }
    }else{
        if(tOp1 == 0x10){ // Operando 1 es de tipo Registro

            registro = mv->memoria[ipAct];
            sectorDeRegistro = (registro & 0x30) >> 6; //Hay que aplicar mascaras para saber que cantidad de bits se extraen del registro
            registro = registro & 0x0F; //saco cual es el registro que voy a pisar

            if(sectorDeRegistro == 0x01){ //4to byte del registro

                if(tOp2 == 0x00){ //El operando 2 es de tipo Memoria

                    registroOp2 = ++ipAct;
                    valor = (int) mv->memoria[mv->TDD[0] + registroOp2];
                    valor = (offset | 0x00000000) >> 24; //convierto el byte a 4 bytes y hago el corrimiento a los 8 bits menos significantes
                    mv->registros[registro] = (unsigned int) valor;

                }else{

                    if(tOp2 == 0x01){ // El operando 2 es de tipo Inmediato

                        registroOp2 = ++ipAct;
                        valor = (int) mv->memoria[registroOp2];
                        valor = (valor | 0x00000000) >> 24; //convierto el byte a 4 bytes y hago el corrimiento a los 8 bits menos significantes
                        registroOp2 = ++ipAct;
                        valor2 = (int) mv->memoria[registroOp2];//convierto el byte a 4 bytes y hago el corrimiento de 2 bytes
                        valor2 = (valor2 | 0x00000000) >> 16;
                        valor = (valor | valor2); //uno los 2 bytes que leí para formar el inmediato
                        mv->registros[registro] = (unsigned int) valor;

                    }else{ // El operando 2 es de tipo registro

                        registroOp2 = mv->memoria[++ipAct]; //veo el registro que voy a tomar
                        sectorDeRegistro = (registroOp2 & 0x30) >> 6; //leo el sector del registro que voy a usar
                        registroOp2 = registroOp2 & 0x0F;

                        if(sectorDeRegistro == 0x01) //4to byte del registro

                            mv->registros[registro] = mv->registros[registroOp2] & 0x000000FF;
                        else
                            if(sectorDeRegistro == 0x10 || sectorDeRegistro == 0x11) //3er byte del registro o registro de 2 bytes

                                mv->registros[registro] = (mv->registros[registroOp2] & 0x0000FF00) >> 8;

                            else //registro de 4 bytes

                                mv->registros[registro] = (mv->registros[registroOp2] & 0xFF000000) >> 24;
                    }

                }
            }else{
                if(sectorDeRegistro == 0x10){ //3er byte del registro

                    if(tOp2 == 0x00){ //El operando 2 es de tipo Memoria

                        registroOp2 = ++ipAct;
                        valor = (int) mv->memoria[mv->TDD[0] + registroOp2];
                        valor = (offset | 0x00000000) >> 16; //convierto el byte a 4 bytes y hago el corrimiento a los 8 bits menos significantes
                        mv->registros[registro] = (unsigned int) valor;

                    }else{

                        if(tOp2 == 0x01){ // El operando 2 es de tipo Inmediato

                            registroOp2 = ipAct + 2;
                            valor = (int) mv->memoria[registroOp2];
                            valor = (valor | 0x00000000) >> 16; //convierto el byte a 4 bytes y hago el corrimiento a los 8 bits menos significantes
                            mv->registros[registro] = (unsigned int) valor;

                        }else{ // El operando 2 es de tipo Registro

                            registroOp2 = mv->memoria[++ipAct]; //veo el registro que voy a tomar
                            sectorDeRegistro = (registroOp2 & 0x30) >> 6; //leo el sector del registro que voy a usar
                            registroOp2 = registroOp2 & 0x0F ;

                            if(sectorDeRegistro == 0x01 || sectorDeRegistro == 0x11) //4to byte del registro o registro de 2 bytes

                                mv->registros[registro] = (mv->registros[registroOp2] & 0x000000FF) << 8;
                            else
                                if(sectorDeRegistro == 0x10 ) //3er byte del registro

                                    mv->registros[registro] = mv->registros[registroOp2] & 0x0000FF00;

                                else //registro de 4 bytes

                                    mv->registros[registro] = (mv->registros[registroOp2] & 0xFF000000) >> 24;
                        }
                    }
                }else{
                    if(sectorDeRegistro == 0x11){//registro de 2 bytes

                        if(tOp2 == 0x00){ //El operando 2 es de tipo Memoria

                            registroOp2 = ++ipAct;
                            offset = mv->memoria[mv->TDD[0] + registroOp2];
                            offset2 = mv->memoria[mv->TDD[0] + registroOp2 + 1];

                            //tomo dos bytes del DS y los uno poniendo el 2do byte en el ultimo byte

                            valor = (int) ((offset2 | 0x00000000) >> 24) | ((offset | 0x00000000)>> 16);
                            mv->registros[registro] = (unsigned int) valor;

                        }else{

                            if(tOp2 == 0x01){ // El operando 2 es de tipo Inmediato

                                registroOp2 = ++ipAct;
                                offset = mv->memoria[registroOp2];
                                offset2 = mv->memoria[registroOp2 + 1];

                                //tomo dos bytes de la memoria y los uno poniendo el 2do byte en el ultimo byte

                                valor = (int) ((offset2 | 0x00000000) >> 24) | ((offset | 0x00000000)>> 16);
                                mv->registros[registro] = (unsigned int) valor;

                            }else{ // El operando 2 es de tipo Registro

                                registroOp2 = mv->memoria[++ipAct]; //veo el registro que voy a tomar
                                sectorDeRegistro = (registroOp2 & 0x30) >> 6; //leo el sector del registro que voy a usar
                                registroOp2 = registroOp2 & 0x0F ;

                                if(sectorDeRegistro == 0x01 || sectorDeRegistro == 0x11) //4to byte del registro o registro de 2 bytes

                                    mv->registros[registro] = (mv->registros[registroOp2] & 0x0000FFFF);
                                else
                                    if(sectorDeRegistro == 0x10 ) //3er byte del registro

                                        mv->registros[registro] = mv->registros[registroOp2] & 0x0000FF00;

                                    else //registro de 4 bytes

                                        mv->registros[registro] = (mv->registros[registroOp2] & 0xFF000000) >> 24;
                            }
                        }

                    }else{//registro de 4 bytes

                        if(tOp2 == 0x00){ //El operando 2 es de tipo Memoria
                            char offset3,offset4;
                            registroOp2 = ++ipAct;
                            offset = mv->memoria[mv->TDD[0] + registroOp2];
                            offset2 = mv->memoria[mv->TDD[0] + registroOp2 + 1];
                            offset3 = mv->memoria[mv->TDD[0] + registroOp2 + 2];
                            offset4 = mv->memoria[mv->TDD[0] + registroOp2 + 3];

                            //tomo dos bytes del DS y los uno poniendo el 2do byte en el ultimo byte

                            valor = (int) ((offset4 | 0x00000000) >> 24) | ((offset3 | 0x00000000)>> 16) |
                                ((offset2 | 0x00000000) >> 8) | (offset | 0x00000000);

                            mv->registros[registro] = (unsigned int) valor;

                        }else{

                            if(tOp2 == 0x01){ // El operando 2 es de tipo Inmediato

                                registroOp2 = ++ipAct;
                                offset = mv->memoria[registroOp2];
                                offset2 = mv->memoria[registroOp2 + 1];

                                //tomo dos bytes de la memoria y los uno poniendo el 2do byte en el ultimo byte

                                valor = (int) ((offset2 | 0x00000000) >> 24) | ((offset | 0x00000000)>> 16);
                                mv->registros[registro] = (unsigned int) valor & 0x0000FFFF;

                            }else{ // El operando 2 es de tipo Registro

                                registroOp2 = mv->memoria[++ipAct]; //veo el registro que voy a tomar
                                mv->registros[registro] = mv->registros[registroOp2];
                            }
                        }
                    }
                }

void add(TMV * mv,char tOp1,char tOp2,int ipAct){
    if(tOp1 == 0x00){

    }



}
void sub(TMV * mv,char tOp1,char tOp2,int ipAct);
void intercambia(TMV * mv,char tOp1,char tOp2,int ipAct);
void mul(TMV * mv,char tOp1,char tOp2,int ipAct);
void div(TMV * mv,char tOp1,char tOp2,int ipAct);
void cmp(TMV * mv,char tOp1,char tOp2,int ipAct);
void shl(TMV * mv,char tOp1,char tOp2,int ipAct);
void shr(TMV * mv,char tOp1,char tOp2,int ipAct);
void and(TMV * mv,char tOp1,char tOp2,int ipAct);
void or(TMV * mv,char tOp1,char tOp2,int ipAct);
void xor(TMV * mv,char tOp1,char tOp2,int ipAct);

//operaciones de 1 operando
void sys(TMV * mv,char tOp1,char tOp2,int ipAct);
void jmp(TMV * mv,char tOp1,char tOp2,int ipAct){
    int valor;
    char aux1,aux2;

    if(tOp1 == 0x00){

    }else{
        if(tOp1 == 0x01){

        }else{

        }
    }
    aux2;



}
void jz(TMV * mv,char tOp1,char tOp2,int ipAct);
void jp(TMV * mv,char tOp1,char tOp2,int ipAct);
void jn(TMV * mv,char tOp1,char tOp2,int ipAct);
void jnz(TMV * mv,char tOp1,char tOp2,int ipAct);
void jnp(TMV * mv,char tOp1,char tOp2,int ipAct);
void jnn(TMV * mv,char tOp1,char tOp2,int ipAct);
void ldl(TMV * mv,char tOp1,char tOp2,int ipAct);
void ldh(TMV * mv,char tOp1,char tOp2,int ipAct);
void rnd(TMV * mv,char tOp1,char tOp2,int ipAct);
void not(TMV * mv,char tOp1,char tOp2,int ipAct);

//t_funcion *vecFuncionesMV[]={
//                                mov,
//                                add,
//                                sub,
//                                intercambia,
//                                mul,
//                                div,
//                                cmp,
//                                shl,
//                                shr,
//                                and,
//                                or,
//                                xor,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                0,
//                                sys,
//                                jmp,
//                                jz,
//                                jp,
//                                jn,
//                                jnz,
//                                jnp,
//                                jnn,
//                                ldl,
//                                ldh,
//                                rnd,
//                                not
//                                };
