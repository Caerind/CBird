#include "../include/Random.hpp"

void initRandomGenerator()
{
    srand(time(0));
}

int getRandom(int a, int b)
{
    return a + rand()%(b-a);
}
