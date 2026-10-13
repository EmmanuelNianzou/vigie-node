#include "filters.h"

int32_t median3(int32_t a, int32_t b, int32_t c)
{
    if ((a >= b && a <= c) || (a <= b && a >= c)) {
        return a;
    }
    if ((b >= a && b <= c) || (b <= a && b >= c)) {
        return b;
    }
    return c;
}
