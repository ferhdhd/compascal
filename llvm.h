#ifndef LLVM_H
#define LLVM_H

#include <stdlib.h>
#include <stdio.h>
#include "listaID.h"

void emiteGlobal (FILE* fp, nodoID* nodo);
void emiteFunc (FILE* fp, nodoID* nodo);
nodoID* emiteParametrosFunc (FILE* fp, nodoID* nodo);
char *converteTipo (char* tipo);
void emiteNumero (FILE *fp, exp_t *novo_exp, int id_atual);
void emiteVar (FILE *fp, exp_t *novo_exp, int id_atual);
void emiteSoma (FILE *fp, exp_t *exp_esq, exp_t *exp_dir, int id_atual);
void emiteSubtracao (FILE *fp, exp_t *exp_esq, exp_t *exp_dir, int id_atual);
void emiteOpMult (FILE *fp, exp_t *exp_esq, exp_t *exp_dir, char *op, int id_atual);
void emiteOr (FILE *fp, exp_t *exp_esq, exp_t *exp_dir, int id_atual);

void armazenaVar (FILE *fp, char *var, exp_t *exp, nodoID *ts);

#endif