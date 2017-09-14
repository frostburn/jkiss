#ifndef JKISS_UTIL_H_GUARD
#define JKISS_UTIL_H_GUARD

#include <stddef.h>

#include "jkiss/jkiss.h"

// Arrange the n elements of the array in random order.
void jkiss64_shuffle(jkiss64 *gen, void *array, size_t n, size_t element_size);

void shuffle(void *array, size_t n, size_t element_size);

// Return an index according to a distribution of weights that sum up to one.
size_t jkiss64_choose_weighted(jkiss64 *gen, double *weights, size_t n);

size_t choose_weighted(double *weights, size_t n);

#endif
