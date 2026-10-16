#pragma once
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float alpha;
    float value;
    bool primed;
} ema_t;

void ema_init(ema_t *f, float alpha);
float ema_update(ema_t *f, float sample);

//passe à true au-dessus de `high`, repasse à false sous `low`
typedef struct {
    float low;
    float high;
    bool state;
} hysteresis_t;

void hysteresis_init(hysteresis_t *h, float low, float high);
bool hysteresis_update(hysteresis_t *h, float value);

// Médiane de trois valeurs : rejette une mesure aberrante isolée
int32_t median3(int32_t a, int32_t b, int32_t c);

// Anti-rebond : l'état stable ne change qu'après `threshold` échantillons consécutifs identiques
typedef struct {
    bool stable;
    bool candidate;
    uint8_t count;
    uint8_t threshold;
} debounce_t;

void debounce_init(debounce_t *d, bool initial, uint8_t threshold);
bool debounce_update(debounce_t *d, bool raw); // retourne l'état stable

#ifdef __cplusplus
}
#endif
