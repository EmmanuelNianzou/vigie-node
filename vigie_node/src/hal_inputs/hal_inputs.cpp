#include "hal_inputs.h"
#include <Arduino.h>
#include "../app_config.h"
#include "../filters/filters.h"

static const uint8_t PINS[INPUT_COUNT] = {PIN_PIR, PIN_IR_OBSTACLE, PIN_BUTTON};
static const bool ACTIVE_LOW[INPUT_COUNT] = {false, true, true};
static debounce_t s_deb[INPUT_COUNT];

void hal_inputs_init(void)
{
    for (int i = 0; i < INPUT_COUNT; i++) {
        pinMode(PINS[i], ACTIVE_LOW[i] ? INPUT_PULLUP : INPUT);
        debounce_init(&s_deb[i], false, INPUT_DEBOUNCE_SAMPLES);
    }
}

void hal_inputs_poll(bool active[INPUT_COUNT])
{
    for (int i = 0; i < INPUT_COUNT; i++) {
        const bool level = digitalRead(PINS[i]) == HIGH;
        active[i] = debounce_update(&s_deb[i], ACTIVE_LOW[i] ? !level : level);
    }
}
