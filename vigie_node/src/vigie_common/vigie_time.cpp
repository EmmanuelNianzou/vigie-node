#include "vigie_time.h"
#include <Arduino.h>

uint32_t vigie_now_ms(void)
{
    return (uint32_t)millis();
}
