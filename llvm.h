#ifndef LLVM_H
#define LLVM_H

#include <stdlib.h>
#include <stdio.h>
#include "listaID.h"

void emiteReadWrite(FILE *fp);

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
void emiteMudancaSinal (FILE *fp, exp_t *exp, int id_atual);


void emiteComparacao(FILE *fp, exp_t *esq, char *comparacao, exp_t *dir, int id_atual);
void emiteComecoIf(FILE *fp, exp_t *exp, int cont_if);
void emiteFimThen(FILE *fp, int cont_if);
void emiteFimElse(FILE *fp, int cont_if);

void emiteComecoWhile(FILE *fp, int cont_while);
void emiteDoWhile(FILE *fp, exp_t *exp, int cont_while);
void emiteFimWhile(FILE *fp, int cont_while);

void emiteMain (FILE *fp);
void emiteProcSemPar (FILE *fp, char *proc, nodoID *ts);
void emiteProcComPar (FILE *fp, char *proc, exp_t *parametros, nodoID *ts);
int emiteWrite(FILE *fp, exp_t *parametros, nodoID *ts, int id_atual);
int emiteRead(FILE *fp, exp_t *parametros, nodoID *ts, int id_atual);
void emiteRetornoFuncao(FILE *fp, exp_t *parametros, exp_t *funcao, nodoID *ts, int id_atual);

void emiteErroRetorno(nodoID *ts);

int armazenaVar (FILE *fp, char *var, exp_t *exp, nodoID *ts);

#endif