#include <gk/random.hpp>

#include <math.h>
#include <time.h>
#include <stdlib.h>

void Random::Seed()
{
    srand(static_cast<unsigned int>(time(NULL)));
}

bool Random::Boolean()
{
    return (rand() % 2) == 0;
}

int Random::Integer(int max)
{
    return (rand() % (max + 1));
}

const int MAX_PRECISION = (log(RAND_MAX) / log(10)) * 10;
float Random::Decimal()
{
    return (static_cast<float>(Random::Integer(MAX_PRECISION)) / static_cast<float>(MAX_PRECISION));
}

