#include "llvm.h"
#include <stdio.h>
#include <string.h>

void emiteGlobal (FILE* fp, char *var, char *tipo) {
    
    if (!strcmp(tipo, "INTEIRO"))
        fprintf(fp, "@%s = global i32 0");
    else if (!strcmp(tipo, "REAL"))
        fprintf(fp, "@%s = global float 0");
}
