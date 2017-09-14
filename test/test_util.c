#include <assert.h>
#include <limits.h>

#include "jkiss/util.h"

#define NUM_ITERATIONS (10000)

void test_shuffle() {
    short int values[] = {0, 1, 2, 3, 4, 5, 6, 7};
    shuffle(values, 8, sizeof(short int));
    unsigned char flags = 0;
    for (int i = 0; i < 8; ++i) {
        printf("%d\n", values[i]);
        flags |= 1 << values[i];
    }
    assert(flags = ~0);
}

void test_choose_weighted() {
    double weights[] = {0, 0, 0.25, 0.5, 0.25, 0};
    int counts[6] = {0};
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        size_t choice = choose_weighted(weights, 6);
        counts[choice]++;
    }
    for (int i = 0; i < 6; ++i) {
        printf("%d\n", counts[i]);
    }
    assert(!counts[0]);
    assert(!counts[1]);
    assert(!counts[5]);
    assert(counts[2] < counts[3]);
    assert(counts[4] < counts[3]);
}

int main() {
    jkiss_init();

    test_shuffle();
    test_choose_weighted();
}
