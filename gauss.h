#ifndef GAUSS_H
#define GAUSS_H

#include <stddef.h>

extern float modulo(float x);
extern float *gauss(size_t ordem, float matriz[ordem][ordem], float b[ordem]);

#endif