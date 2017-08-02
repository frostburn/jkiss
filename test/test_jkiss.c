#include <assert.h>
#include <limits.h>

#include "jkiss/jkiss.h"
#define NUM_ITERATIONS (10000)

int main() {
    jkiss_init();

    unsigned long long accum_32 = 0;
    unsigned long long accum_64 = 0;
    double accum_long = 0;
    double accum_double = 0;

    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        accum_32 += jkiss32_step(&jkiss32_global_gen);
        accum_64 += jrand();
        accum_long += jlrand();
        accum_double += jdrand();
    }

    assert(accum_32 > 0.4 * UINT_MAX * NUM_ITERATIONS);
    assert(accum_64 > 0.4 * UINT_MAX * NUM_ITERATIONS);
    assert(accum_long > 0.4 * ULONG_MAX * NUM_ITERATIONS);
    assert(accum_double > 0.4 * NUM_ITERATIONS);

    assert(accum_32 < 0.6 * UINT_MAX * NUM_ITERATIONS);
    assert(accum_64 < 0.6 * UINT_MAX * NUM_ITERATIONS);
    assert(accum_long < 0.6 * ULONG_MAX * NUM_ITERATIONS);
    assert(accum_double < 0.6 * NUM_ITERATIONS);

    jkiss64 gen1 = jkiss64_spawn();
    jkiss64 gen2 = jkiss64_spawn();

    double correlation = 0;
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        correlation += jkiss64_step_double(&gen1) * jkiss64_step_double(&gen2);
    }
    assert(correlation > 0.2 * NUM_ITERATIONS);
    assert(correlation < 0.3 * NUM_ITERATIONS);

    jkiss32_spawn();

    jkiss_seed(12345);
    unsigned int r1 = jrand();
    jkiss_seed(12345);
    unsigned int r2 = jrand();

    assert(r1 == r2);
}
