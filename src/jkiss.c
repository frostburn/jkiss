/* Public domain code for JKISS RNG */
// http://www0.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "jkiss/jkiss.h"

jkiss32 jkiss32_global_gen = {123456789, 987654321, 43219876, 6543217};
jkiss64 jkiss64_global_gen = {123456789123ULL, 987654321987ULL, 43219876, 6543217, 21987643, 1732654};

void jkiss32_fprint(FILE *f, jkiss32 gen) {
    fprintf(f, "(jkiss32){%u, %u, %u, %u};\n", gen.x, gen.y, gen.z, gen.c);
}

void jkiss64_fprint(FILE *f, jkiss64 gen) {
    fprintf(f, "(jkiss64){%lluULL, %lluULL, %u, %u, %u, %u};\n", gen.x, gen.y, gen.z, gen.c, gen.zl, gen.cl);
}

unsigned int devrand(void)
{
    int fn;
    unsigned int r;
    fn = open("/dev/urandom", O_RDONLY);
    if (fn == -1)
        exit(-1); /* Failed! */
    if (read(fn, &r, 4) != 4)
        exit(-1); /* Failed! */
    close(fn);
    return r;
}

/* Initialise KISS generator using /dev/urandom */
void jkiss32_init(jkiss32 *j)
{
    j->x = devrand();
    while (!(j->y = devrand())); /* y must not be zero! */
    j->z = devrand();
    /* We don’t really need to set c as well but let's anyway… */
    /* NOTE: offset c by 1 to avoid z=c=0 */
    j->c = devrand() % 698769068 + 1; /* Should be less than 698769069 */
}

void jkiss64_init(jkiss64 *j)
{
    j->x = devrand();
    j->x = (j->x << 32) | devrand();
    while (!(j->y = devrand())); /* y must not be zero! */
    j->y = (j->y << 32) | devrand();
    j->z = devrand();
    j->zl = devrand();
    /* We don’t really need to set c as well but let's anyway… */
    /* NOTE: offset c by 1 to avoid z=c=0 */
    j->c = devrand() % 698769068 + 1; /* Should be less than 698769069 */
    j->cl = devrand() % 698769068 + 1;
}

// Fastest
unsigned int jkiss32_step(jkiss32 *j)
{
    unsigned long long t;
    j->x = 314527869 * j->x + 1234567;
    j->y ^= j->y << 5; j->y ^= j->y >> 7; j->y ^= j->y << 22;
    t = 4294584393ULL * j->z + j->c;
    j->c = t >> 32;
    j->z = t;
    return j->x + j->y + j->z;
}

// Almost as fast, but with a longer period
unsigned int jkiss64_step(jkiss64 *j)
{
    unsigned long long t;
    j->x = 1490024343005336237ULL * j->x + 123456789;
    j->y ^= j->y << 21; j->y ^= j->y >> 17; j->y ^= j->y << 30;
    t = 4294584393ULL * j->z + j->c; j->c = t >> 32; j->z = t;
    return (unsigned int)(j->x >> 32) + (unsigned int)j->y + j->z; /* Return 32-bit result */
}

// Generates 64-bit results
unsigned long long jkiss64_step_long(jkiss64 *j)
{
    unsigned long long t;
    j->x = 1490024343005336237ULL * j->x + 123456789;
    j->y ^= j->y << 21; j->y ^= j->y >> 17; j->y ^= j->y << 30;
    t = 4294584393ULL * j->z + j->c; j->c = t >> 32; j->z = t;
    t = 4246477509ULL * j->zl + j->cl; j->cl = t >> 32; j->zl = t;
    return j->x + j->y + j->z + ((unsigned long long)j->zl << 32); /* Return 64-bit result */
}

// Uniform result in the range from 0 to 1
double jkiss64_step_double(jkiss64 *j) {
    return jkiss64_step_long(j) / 18446744073709551615.0;
}

void jkiss_init() {
    jkiss32_init(&jkiss32_global_gen);
    jkiss64_init(&jkiss64_global_gen);
}

void jkiss_seed(unsigned int seed) {
    unsigned int y = seed * seed + 7;
    unsigned int c = (seed ^ (seed >> 11)) % 698769068 + 1;
    if (y == 0) {
        y = 11;
    }
    jkiss32_global_gen = (jkiss32){seed, y, seed*seed*seed, c};
    for (int i = 0; i < 100; ++i) {
        jkiss32_step(&jkiss32_global_gen);
    }
    jkiss64_global_gen = (jkiss64){
        jkiss32_step(&jkiss32_global_gen),
        jkiss32_step(&jkiss32_global_gen),
        jkiss32_step(&jkiss32_global_gen),
        jkiss32_step(&jkiss32_global_gen),
        jkiss32_step(&jkiss32_global_gen),
        jkiss32_step(&jkiss32_global_gen)
    };
    jkiss64_global_gen.x = jkiss64_global_gen.x << 32 | jkiss32_step(&jkiss32_global_gen);
    jkiss64_global_gen.y = jkiss64_global_gen.y << 32 | jkiss32_step(&jkiss32_global_gen);
    if (jkiss64_global_gen.y == 0) {
        jkiss64_global_gen.y = 13;
    }
    if (jkiss64_global_gen.c > 698769068) {
        jkiss64_global_gen.c = 17;
    }
    if (jkiss64_global_gen.cl > 698769068) {
        jkiss64_global_gen.cl = 19;
    }
}

unsigned int jrand() {
    return jkiss64_step(&jkiss64_global_gen);
}

unsigned long long jlrand() {
    return jkiss64_step_long(&jkiss64_global_gen);
}

double jdrand() {
    return jkiss64_step_double(&jkiss64_global_gen);
}

void jkiss_freeze(FILE *f) {
    fprintf(
        f,
        "jkiss32_global_gen = (jkiss32){%u, %u, %u, %u};\n",
        jkiss32_global_gen.x, jkiss32_global_gen.y, jkiss32_global_gen.z, jkiss32_global_gen.c
    );
    fprintf(
        f,
        "jkiss64_global_gen = (jkiss64){%lluULL, %lluULL, %u, %u, %u, %u};\n",
        jkiss64_global_gen.x, jkiss64_global_gen.y, jkiss64_global_gen.z, jkiss64_global_gen.c, jkiss64_global_gen.zl, jkiss64_global_gen.cl
    );
}

jkiss32 jkiss32_spawn() {
    unsigned int y;
    while(!(y = jrand()));
    return (jkiss32) {
        jrand(),
        y,
        jrand(),
        jrand() % 698769068 + 1,
    };
}

jkiss64 jkiss64_spawn() {
    unsigned long long y;
    while(!(y = jlrand()));
    return (jkiss64) {
        jlrand(),
        y,
        jrand(),
        jrand() % 698769068 + 1,
        jrand(),
        jrand() % 698769068 + 1,
    };
}
