#include "src/app_config.h"
#include "src/filters/filters.h"
#include "src/alarm_fsm/alarm_fsm.h"
#include "src/hal_outputs/hal_outputs.h"
#include "src/vigie_common/vigie_time.h"

void setup()
{
    Serial.begin(115200);
    delay(200);
    Serial.println("VIGIE NODE boot " FW_VERSION);
    hal_outputs_init();

    Serial.printf("median3(20, 21, 4000) = %ld\n", (long)median3(20, 21, 4000));
    alarm_fsm_t fsm;
    const alarm_cfg_t cfg = {5000, 60000};
    alarm_fsm_init(&fsm, &cfg);
    Serial.printf("etat initial FSM = %d\n", (int)fsm.state);

    for (int i = 0; i < LED_COUNT; i++) {
        hal_led_set((led_id_t)i, true);
        delay(300);
        hal_led_set((led_id_t)i, false);
    }
    hal_rgb(127, 0, 255);
    hal_buzzer_tone(880);
    delay(200);
    hal_buzzer_tone(0);
    hal_relay_set(true);
    delay(500);
    hal_relay_set(false);
}

void loop()
{
    Serial.printf("uptime : %lu ms\n", (unsigned long)vigie_now_ms());
    delay(2000);
}
