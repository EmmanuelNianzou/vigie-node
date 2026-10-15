#include "hal_outputs.h"
#include <Arduino.h>
#include "../app_config.h"

static const uint8_t LED_PIN[LED_COUNT] = {PIN_LED_GREEN, PIN_LED_YELLOW, PIN_LED_RED};
static const uint8_t RGB_PIN[3] = {PIN_RGB_R, PIN_RGB_G, PIN_RGB_B};
static const bool RGB_COMMON_ANODE = false; // true si ta LED RGB est à anode commune

void hal_outputs_init(void)
{
    digitalWrite(PIN_RELAY_IN1, RELAY_ACTIVE_LOW ? HIGH : LOW);
    pinMode(PIN_RELAY_IN1, OUTPUT);

    for (int i = 0; i < LED_COUNT; i++) {
        digitalWrite(LED_PIN[i], LOW);
        pinMode(LED_PIN[i], OUTPUT);
    }
    digitalWrite(PIN_BUZZER_ACTIVE, LOW);
    pinMode(PIN_BUZZER_ACTIVE, OUTPUT);

    for (int c = 0; c < 3; c++) {
        ledcAttach(RGB_PIN[c], 1000, 8); // API du cœur Arduino-ESP32 3.x
    }
    ledcAttach(PIN_BUZZER_PASSIVE, 2000, 8);
    hal_rgb(0, 0, 0);
    hal_buzzer_tone(0);
}

void hal_led_set(led_id_t id, bool on)
{
    if (id < LED_COUNT) {
        digitalWrite(LED_PIN[id], on ? HIGH : LOW);
    }
}

void hal_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    const uint8_t v[3] = {r, g, b};
    for (int c = 0; c < 3; c++) {
        ledcWrite(RGB_PIN[c], RGB_COMMON_ANODE ? 255 - v[c] : v[c]);
    }
}

void hal_buzzer_tone(uint32_t freq_hz) { ledcWriteTone(PIN_BUZZER_PASSIVE, freq_hz); }
void hal_buzzer_active(bool on) { digitalWrite(PIN_BUZZER_ACTIVE, on ? HIGH : LOW); }
void hal_relay_set(bool on) { digitalWrite(PIN_RELAY_IN1, (on != RELAY_ACTIVE_LOW) ? HIGH : LOW); }
