#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef enum { LED_GREEN, LED_YELLOW, LED_RED, LED_COUNT } led_id_t;

void hal_outputs_init(void);
void hal_led_set(led_id_t id, bool on);
void hal_rgb(uint8_t r, uint8_t g, uint8_t b);
void hal_buzzer_tone(uint32_t freq_hz);
void hal_buzzer_active(bool on);
void hal_relay_set(bool on);
