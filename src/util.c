#include <stdlib.h>
#include <string.h>

#include "jkiss/jkiss.h"
#include "jkiss/util.h"

void jkiss64_shuffle(jkiss64 *gen, void *array, size_t n, size_t element_size) {
    if (n <= 1) {
        return;
    }
    void *a1;
    void *a2;
    void *temp = malloc(element_size);
    for (size_t i = 0; i < n - 1; ++i) {
        size_t j = i + jkiss64_step(gen) % (n - i);
        a1 = array + i * element_size;
        a2 = array + j * element_size;
        memcpy(temp, a2, element_size);
        memcpy(a2, a1, element_size);
        memcpy(a1, temp, element_size);
    }
    free(temp);
}

void shuffle(void *array, size_t n, size_t element_size) {
    jkiss64_shuffle(&jkiss64_global_gen, array, n, element_size);
}

size_t jkiss64_choose_weighted(jkiss64 *gen, double *weights, size_t n) {
    double selector = jkiss64_step_double(gen);
    size_t i = 0;
    while (selector >= 0 && i < n) {
        selector -= weights[i++];
    }
    return i - 1;
}

size_t choose_weighted(double *weights, size_t n) {
    return jkiss64_choose_weighted(&jkiss64_global_gen, weights, n);
}
