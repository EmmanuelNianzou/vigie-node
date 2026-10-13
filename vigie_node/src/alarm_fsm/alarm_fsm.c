#include "alarm_fsm.h"

static void enter(alarm_fsm_t *f, alarm_state_t s, uint32_t now_ms)
{
    f->state = s;
    f->t_enter_ms = now_ms;
    f->released = false;
}

void alarm_fsm_init(alarm_fsm_t *f, const alarm_cfg_t *cfg)
{
    f->cfg = *cfg;
    f->state = ALARM_DISARMED;
    f->t_enter_ms = 0;
    f->released = false;
}

alarm_output_t alarm_fsm_step(alarm_fsm_t *f, const alarm_input_t *in, uint32_t now_ms)
{
    const alarm_state_t previous = f->state;
    const bool detect = in->pir || in->obstacle;
    const uint32_t elapsed = now_ms - f->t_enter_ms; /* non signé : correct même au débordement */

    if (in->sensor_fault && f->state != ALARM_FAULT) {
        enter(f, ALARM_FAULT, now_ms); /* 9 */
    } else {
        switch (f->state) {
        case ALARM_DISARMED:
            if (in->btn_short || in->cmd_arm) {
                enter(f, ALARM_ARMED, now_ms); /* 1 */
            }
            break;
        case ALARM_ARMED:
            if (in->btn_short || in->cmd_disarm) {
                enter(f, ALARM_DISARMED, now_ms); /* 2 */
            } else if (detect) {
                enter(f, ALARM_PRE_ALERT, now_ms); /* 3 */
            }
            break;
        case ALARM_PRE_ALERT:
            if (in->btn_short || in->cmd_disarm) {
                enter(f, ALARM_DISARMED, now_ms);
            } else {
                if (!detect) {
                    f->released = true;
                }
                if (detect && f->released) {
                    enter(f, ALARM_ALERT, now_ms); /* 5 : seconde détection */
                } else if (elapsed >= f->cfg.t_confirm_ms) {
                    enter(f, ALARM_ARMED, now_ms); /* 4 : pas de confirmation */
                }
            }
            break;
        case ALARM_ALERT:
            if (in->cmd_disarm) {
                enter(f, ALARM_DISARMED, now_ms); /* 8 */
            } else if (in->btn_short) {
                enter(f, ALARM_MUTED, now_ms); /* 6 */
            }
            break;
        case ALARM_MUTED:
            if (in->cmd_disarm) {
                enter(f, ALARM_DISARMED, now_ms);
            } else if (elapsed >= f->cfg.t_mute_ms) {
                enter(f, ALARM_ARMED, now_ms); /* 7 */
            }
            break;
        case ALARM_FAULT:
            if (!in->sensor_fault && (in->btn_long || in->cmd_disarm)) {
                enter(f, ALARM_DISARMED, now_ms); /* 10 */
            }
            break;
        }
    }

    alarm_output_t out = {0};
    out.state = f->state;
    out.led_green = (f->state == ALARM_ARMED);
    out.led_yellow = (f->state == ALARM_PRE_ALERT);
    out.led_red = (f->state == ALARM_ALERT || f->state == ALARM_MUTED || f->state == ALARM_FAULT);
    out.buzzer_on = (f->state == ALARM_ALERT);
    out.relay_on = (f->state == ALARM_ALERT);
    out.publish_event = (f->state != previous);
    return out;
}
