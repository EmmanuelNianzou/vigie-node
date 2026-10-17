#include "ui_service.h"
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../hal_oled/hal_oled.h"
#include "../hal_outputs/hal_outputs.h"
#include "../vigie_common/vigie_state.h"

static void ui_task(void *)
{
    char l[24];
    for (;;) {
        vigie_state_t s;
        vigie_state_copy(&s);
        const uint32_t now = millis();
        const uint32_t page = (now / 3000) % 3;
        hal_oled_clear();
        if (page == 0) {
            snprintf(l, sizeof(l), "T=%.1fC H=%.0f%%", s.temp_c, s.hum_pct);
            hal_oled_print(0, l);
            snprintf(l, sizeof(l), "LUX=%d POT=%d", s.lux_raw, s.pot_raw);
            hal_oled_print(1, l);
        } else if (page == 1) {
            hal_oled_print(0, s.net_up ? "WIFI: OK" : "WIFI: --");
            hal_oled_print(1, s.mqtt_up ? "MQTT: OK" : "MQTT: --");
            if (s.provisioning) {
                snprintf(l, sizeof(l), "PoP: %s", s.prov_pop);
                hal_oled_print(2, l);
            }
        } else {
            snprintf(l, sizeof(l), "ETAT: %s", alarm_state_name(s.alarm_state));
            hal_oled_print(0, l);
        }
        hal_oled_flush();
        if (s.provisioning) hal_rgb(80, 0, 80);
        else if (!s.net_up) hal_rgb(0, 0, ((now / 500) % 2) ? 80 : 0);
        else hal_rgb(0, s.mqtt_up ? 80 : 30, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void ui_service_start(void) { xTaskCreate(ui_task, "ui", 4096, NULL, 2, NULL); }
