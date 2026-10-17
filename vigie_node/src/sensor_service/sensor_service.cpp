#include "sensor_service.h"
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../app_config.h"
#include "../filters/filters.h"
#include "../hal_sensors/hal_sensors.h"
#include "../vigie_common/vigie_state.h"
#include "../vigie_common/vigie_time.h"

static void sensor_task(void *)
{
    ema_t t_f, h_f;
    ema_init(&t_f, 0.3f);
    ema_init(&h_f, 0.3f);
    int hist[3] = {0, 0, 0};
    uint8_t hi = 0;
    for (;;) {
        float t = 0, h = 0;
        const dht_status_t st = dht11_read(PIN_DHT11, &t, &h);
        hist[hi] = adc_read_raw(PIN_LDR);
        hi = (hi + 1) % 3;
        const int pot = adc_read_raw(PIN_POT);
        vigie_state_t *s = vigie_state_lock();
        s->dht_ok = (st == DHT_OK);
        if (st == DHT_OK) {
            s->temp_c = ema_update(&t_f, t);
            s->hum_pct = ema_update(&h_f, h);
        }
        s->lux_raw = (int)median3(hist[0], hist[1], hist[2]);
        s->pot_raw = pot;
        s->t_ms = vigie_now_ms();
        vigie_state_unlock();
        if (st != DHT_OK) {
            Serial.printf("DHT11 erreur %d\n", (int)st);
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void sensor_service_start(void)
{
    adc_init();
    xTaskCreate(sensor_task, "sensor", 4096, NULL, 5, NULL);
}
