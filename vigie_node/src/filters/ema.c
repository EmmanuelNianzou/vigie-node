#include "filters.h"

void ema_init(ema_t *f, float alpha)
{
    f->alpha = alpha;
    f->value = 0.0f;
    f->primed = false;
}

float ema_update(ema_t *f, float sample)
{
    if (!f->primed) {
        f->value = sample;
        f->primed = true;
    } else {
        f->value = f->alpha * sample + (1.0f - f->alpha) * f->value;
    }
    return f->value;
}
