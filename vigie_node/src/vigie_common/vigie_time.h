#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Horloge monotone en millisecondes (millis() sur Arduino). Dans les tests PC : valeur fournie. */
uint32_t vigie_now_ms(void);

#ifdef __cplusplus
}
#endif
