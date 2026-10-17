#include "vigie_state.h"
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

static vigie_state_t s_state;
static SemaphoreHandle_t s_mutex;

void vigie_state_init(void) { s_mutex = xSemaphoreCreateMutex(); }
vigie_state_t *vigie_state_lock(void)
{
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    return &s_state;
}
void vigie_state_unlock(void) { xSemaphoreGive(s_mutex); }
void vigie_state_copy(vigie_state_t *out)
{
    xSemaphoreTake(s_mutex, portMAX_DELAY);
    *out = s_state;
    xSemaphoreGive(s_mutex);
}
const char *alarm_state_name(uint8_t s)
{
    static const char *N[] = {"DISARMED", "ARMED", "PRE_ALERT", "ALERT", "MUTED", "FAULT"};
    return s < 6 ? N[s] : "FAULT";
}
