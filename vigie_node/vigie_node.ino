#include "src/app_config.h"
#include "src/hal_inputs/hal_inputs.h"
#include "src/hal_oled/hal_oled.h"
#include "src/hal_outputs/hal_outputs.h"
#include "src/hal_sensors/hal_sensors.h"

static const char *NAMES[INPUT_COUNT] = {"PIR", "IR", "BOUTON"};

void setup()
{
    Serial.begin(115200);
    delay(200);
    Serial.println("VIGIE NODE boot " FW_VERSION);
    hal_outputs_init();
    hal_inputs_init();
    adc_init();
    if (!hal_oled_init()) {
        Serial.println("OLED non initialisée");
    }
}

void loop()
{
    static uint32_t last = 0;
    static bool prev[INPUT_COUNT] = {false, false, false};
    bool act[INPUT_COUNT];
    hal_inputs_poll(act);
    for (int i = 0; i < INPUT_COUNT; i++) {
        if (act[i] != prev[i]) {
            Serial.printf("entree %s -> %d\n", NAMES[i], (int)act[i]);
            prev[i] = act[i];
        }
    }
    if (millis() - last >= 2000) {
        last = millis();
        float t = 0, h = 0;
        const dht_status_t st = dht11_read(PIN_DHT11, &t, &h);
        const int ldr = adc_read_raw(PIN_LDR), pot = adc_read_raw(PIN_POT);
        char l[24];
        hal_oled_clear();
        snprintf(l, sizeof(l), "T=%.1fC H=%.0f%%", t, h);
        hal_oled_print(0, l);
        snprintf(l, sizeof(l), "LDR=%d POT=%d", ldr, pot);
        hal_oled_print(1, l);
        hal_oled_print(2, st == DHT_OK ? "DHT11: OK" : "DHT11: erreur");
        hal_oled_flush();
    }
    delay(INPUT_POLL_MS);
}
