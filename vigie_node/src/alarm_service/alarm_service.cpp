#include "alarm_service.h"
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../app_config.h"
#include "../alarm_fsm/alarm_fsm.h"
#include "../hal_inputs/hal_inputs.h"
#include "../hal_outputs/hal_outputs.h"
#include "../vigie_common/vigie_state.h"
#include "../vigie_common/vigie_time.h"

static volatile bool s_cmd_arm, s_cmd_disarm, s_relay_manual;
void alarm_service_cmd_arm(void) { s_cmd_arm = true; }
void alarm_service_cmd_disarm(void) { s_cmd_disarm = true; }
void alarm_service_relay_manual(bool on) { s_relay_manual = on; }

static void alarm_task(void *)
{
    alarm_fsm_t fsm;
    const alarm_cfg_t cfg = {ALARM_T_CONFIRM_MS, ALARM_T_MUTE_MS};
    alarm_fsm_init(&fsm, &cfg);
    bool button_was_down = false;
    uint32_t t_press = 0;
    for (;;) {
        bool act[INPUT_COUNT];
        hal_inputs_poll(act);
        const uint32_t now = vigie_now_ms();
        alarm_input_t in = {};
        if (act[INPUT_BUTTON] && !button_was_down) {
            t_press = now;
        }
        if (!act[INPUT_BUTTON] && button_was_down) {
            if (now - t_press >= LONG_PRESS_MS) in.btn_long = true;
            else in.btn_short = true;
        }
        button_was_down = act[INPUT_BUTTON];
        in.pir = act[INPUT_PIR];
        in.obstacle = act[INPUT_OBSTACLE];
        in.cmd_arm = s_cmd_arm;
        in.cmd_disarm = s_cmd_disarm;
        s_cmd_arm = s_cmd_disarm = false;

        const alarm_output_t out = alarm_fsm_step(&fsm, &in, now);
        hal_led_set(LED_GREEN, out.led_green);
        hal_led_set(LED_YELLOW, out.led_yellow);
        hal_led_set(LED_RED, out.led_red);
        hal_buzzer_active(out.buzzer_on);
        hal_relay_set(out.relay_on || s_relay_manual); // une commande distante ne doit pas être écrasée par la FSM

        vigie_state_t *s = vigie_state_lock();
        s->pir = act[INPUT_PIR];
        s->obstacle = act[INPUT_OBSTACLE];
        s->alarm_state = (uint8_t)out.state;
        s->relay_on = out.relay_on || s_relay_manual;
        vigie_state_unlock();
        if (out.publish_event) {
            Serial.printf("alarme -> etat %s\n", alarm_state_name((uint8_t)out.state));
        }
        vTaskDelay(pdMS_TO_TICKS(INPUT_POLL_MS));
    }
}

void alarm_service_start(void) { xTaskCreate(alarm_task, "alarm", 3072, NULL, 7, NULL); }
