#ifndef MVTIPOS_H_INCLUDED
#define MVTIPOS_H_INCLUDED

typedef char instruccion[];

typedef struct{
    instruccion memoria[];
    int TDD[8];
    unsigned int registros[16];}mv;

#endif // MVTIPOS_H_INCLUDED
