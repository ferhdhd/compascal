#ifndef LLVM_H
#define LLVM_H

#include <stdlib.h>
#include <stdio.h>

void emiteGlobal (FILE* fp, nodoID* nodo);
void emiteFunc (FILE* fp, nodoID* nodo);
nodoID* emiteParametrosFunc (FILE* fp, nodoID* nodo);
char *converteTipo (char* tipo);

#endif