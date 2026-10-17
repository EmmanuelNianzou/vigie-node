#include "src/app_config.h"
#include "src/alarm_service/alarm_service.h"
#include "src/hal_inputs/hal_inputs.h"
#include "src/hal_oled/hal_oled.h"
#include "src/hal_outputs/hal_outputs.h"
#include "src/sensor_service/sensor_service.h"
#include "src/ui_service/ui_service.h"
#include "src/vigie_common/vigie_state.h"

static void log_stacks(void) // marge de pile de chaque tâche, pour les essais
{
    static const char *names[] = {"sensor", "alarm", "ui"};
    for (const char *n : names) {
        TaskHandle_t h = xTaskGetHandle(n);
        if (h) Serial.printf("pile %-6s : %u octets libres (min)\n", n, (unsigned)uxTaskGetStackHighWaterMark(h));
    }
}

void setup()
{
    Serial.begin(115200);
    delay(200);
    Serial.println("VIGIE NODE boot " FW_VERSION);
    vigie_state_init();
    hal_outputs_init();
    hal_inputs_init();
    hal_oled_init();
    sensor_service_start();
    alarm_service_start();
    ui_service_start();
}

void loop()
{
    static uint32_t last = 0;
    if (millis() - last > 10000) {
        last = millis();
        log_stacks();
    }
    vTaskDelay(pdMS_TO_TICKS(100));
}
