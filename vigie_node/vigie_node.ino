#include "src/app_config.h"
#include "src/alarm_fsm/alarm_fsm.h"
#include "src/filters/filters.h"
#include "src/hal_inputs/hal_inputs.h"
#include "src/hal_outputs/hal_outputs.h"
#include "src/hal_sensors/hal_sensors.h"
#include "src/vigie_common/vigie_time.h"

static const char *NAMES[INPUT_COUNT] = {"PIR", "IR", "BOUTON"};

void setup()
{
    Serial.begin(115200);
    delay(200);
    Serial.println("VIGIE NODE boot " FW_VERSION);
    hal_outputs_init();
    hal_inputs_init();
    adc_init();
    Serial.printf("median3(20, 21, 4000) = %ld\n", (long)median3(20, 21, 4000));
}

void loop()
{
    static uint32_t last_sensor = 0;
    static bool prev[INPUT_COUNT] = {false, false, false};
    bool act[INPUT_COUNT];

    hal_inputs_poll(act);
    for (int i = 0; i < INPUT_COUNT; i++) {
        if (act[i] != prev[i]) {
            Serial.printf("entree %s -> %d\n", NAMES[i], (int)act[i]);
            prev[i] = act[i];
        }
    }

    if (millis() - last_sensor >= 2000) {
        last_sensor = millis();
        float t = 0, h = 0;
        const dht_status_t st = dht11_read(PIN_DHT11, &t, &h);
        if (st == DHT_OK) {
            Serial.printf("DHT11 T=%.1fC H=%.0f%%\n", t, h);
        } else {
            Serial.printf("DHT11 erreur %d\n", (int)st);
        }
        Serial.printf("LDR=%d POT=%d\n", adc_read_raw(PIN_LDR), adc_read_raw(PIN_POT));
    }
    delay(INPUT_POLL_MS);
}
