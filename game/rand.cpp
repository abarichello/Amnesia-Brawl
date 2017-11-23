#include "rand.h"

int GenerateRandom(int max) {
    return (rand() % max) + 1;
}

bool RandomBool() {
    return rand() % 2 == 1;
}
