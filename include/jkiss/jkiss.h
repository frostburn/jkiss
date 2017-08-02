#ifndef JKISS_JKISS_H_GUARD
#define JKISS_JKISS_H_GUARD

/* Public domain code for JKISS RNG */
// http://www0.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf
#include <stdio.h>

typedef struct jkiss32
{
    unsigned int x, y, z, c;
} jkiss32;

typedef struct jkiss64
{
    unsigned long long x, y;
    unsigned int z, c;
    unsigned int zl, cl;  // Only used when generating 64 bit results directly.
} jkiss64;


void jkiss32_fprint(FILE *f, jkiss32 gen);
void jkiss64_fprint(FILE *f, jkiss64 gen);

void jkiss32_init(jkiss32 *j);
void jkiss64_init(jkiss64 *j);

// Fastest
unsigned int jkiss32_step(jkiss32 *j);

// Almost as fast, but with a longer period
unsigned int jkiss64_step(jkiss64 *j);

// Generates 64-bit results
unsigned long long jkiss64_step_long(jkiss64 *j);

// Uniform result in the range from 0 to 1
double jkiss64_step_double(jkiss64 *j);

// Global state for easy generation
extern jkiss32 jkiss32_global_gen;
extern jkiss64 jkiss64_global_gen;

// Remember to initialize the global state for non-deterministic results
void jkiss_init();

// Seed helper for easy deterministic results
void jkiss_seed(unsigned int seed);

// The workhorse of the library
unsigned int jrand();

// Long horse is long
unsigned long long jlrand();

// You can also generate floats in the range of 0 to 1
double jdrand();

// Write the global state to a file if you need re-entry
void jkiss_freeze(FILE *f);

// Spawn generators for threaded use
jkiss32 jkiss32_spawn();
jkiss64 jkiss64_spawn();

#endif /* !JKISS_JKISS_H_GUARD */
