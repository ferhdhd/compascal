#ifndef LLVM_H
#define LLVM_H

#include <stdlib.h>
#include <stdio.h>
#include "listaID.h"

void emiteGlobal (FILE* fp, nodoID* nodo);
void emiteFunc (FILE* fp, nodoID* nodo);
nodoID* emiteParametrosFunc (FILE* fp, nodoID* nodo);
char *converteTipo (char* tipo);
void emiteNumero(FILE *fp, exp_t *novo_exp, int id_atual);
void emiteVar(FILE *fp, exp_t *novo_exp, int id_atual);

#endif