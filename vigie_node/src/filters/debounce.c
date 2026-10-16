#include "filters.h"

void debounce_init(debounce_t *d, bool initial, uint8_t threshold)
{
    d->stable = initial;
    d->candidate = initial;
    d->count = 0;
    d->threshold = threshold ? threshold : 1;
}

bool debounce_update(debounce_t *d, bool raw)
{
    if (raw == d->stable) {
        d->candidate = d->stable;
        d->count = 0;
    } else {
        if (raw == d->candidate) {
            d->count++;
        } else {
            d->candidate = raw;
            d->count = 1;
        }
        if (d->count >= d->threshold) {
            d->stable = raw;
            d->count = 0;
        }
    }
    return d->stable;
}
