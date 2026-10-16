#include "hal_sensors.h"
#include <Arduino.h>
#include "../app_config.h"

static portMUX_TYPE s_mux = portMUX_INITIALIZER_UNLOCKED;

void adc_init(void)
{
    analogReadResolution(12);
    analogSetPinAttenuation(PIN_LDR, ADC_11db);
    analogSetPinAttenuation(PIN_POT, ADC_11db);
}

int adc_read_raw(uint8_t pin) { return analogRead(pin); }

// Attend tant que la ligne est à `level`. Retourne la durée ou -1 au dépassement.
static int wait_while(uint8_t pin, int level, uint32_t timeout_us)
{
    const uint32_t t0 = micros();
    while (digitalRead(pin) == level) {
        if ((uint32_t)(micros() - t0) > timeout_us) {
            return -1;
        }
    }
    return (int)(micros() - t0);
}

static bool read_frame(uint8_t pin, uint8_t d[5])
{
    if (wait_while(pin, HIGH, 100) < 0) return false; // le capteur tire la ligne à 0 après 20 à 40 us
    if (wait_while(pin, LOW, 100) < 0) return false;  // réponse basse ≈ 80 us
    if (wait_while(pin, HIGH, 100) < 0) return false; // réponse haute ≈ 80 us
    for (int i = 0; i < 40; i++) {
        if (wait_while(pin, LOW, 80) < 0) return false; // début de bit : 50 us à 0
        const int high = wait_while(pin, HIGH, 100);    // entre 26 et 28 us = 0 ; ≈ 70 us = 1
        if (high < 0) return false;
        d[i / 8] = (uint8_t)(d[i / 8] << 1);
        if (high > 40) d[i / 8] |= 1;
    }
    return true;
}

dht_status_t dht11_read(uint8_t pin, float *temp_c, float *hum_pct)
{
    static uint32_t last_ms = 0;
    if (last_ms != 0 && millis() - last_ms < 1000) return DHT_TOO_SOON;
    last_ms = millis();

    uint8_t d[5] = {0};
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(20);
    digitalWrite(pin, HIGH);
    delayMicroseconds(30);
    pinMode(pin, INPUT_PULLUP);

    portENTER_CRITICAL(&s_mux);
    const bool ok = read_frame(pin, d);
    portEXIT_CRITICAL(&s_mux);

    if (!ok) return DHT_TIMEOUT;
    if ((uint8_t)(d[0] + d[1] + d[2] + d[3]) != d[4]) return DHT_BAD_CRC;
    *hum_pct = d[0] + d[1] / 10.0f;
    *temp_c = d[2] + d[3] / 10.0f;
    return DHT_OK;
}
