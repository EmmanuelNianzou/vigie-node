#include "filters.h"

void hysteresis_init(hysteresis_t *h, float low, float high)
{
    h->low = low;
    h->high = high;
    h->state = false;
}

bool hysteresis_update(hysteresis_t *h, float value)
{
    if (!h->state && value >= h->high) {
        h->state = true;
    } else if (h->state && value <= h->low) {
        h->state = false;
    }
    return h->state;
}
