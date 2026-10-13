#pragma once
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ALARM_DISARMED,
    ALARM_ARMED,
    ALARM_PRE_ALERT,
    ALARM_ALERT,
    ALARM_MUTED,
    ALARM_FAULT
} alarm_state_t;

typedef struct {
    bool pir, obstacle;         /* entrées déjà anti-rebondies */
    bool btn_short, btn_long;
    bool cmd_arm, cmd_disarm;   /* commandes déjà authentifiées */
    bool sensor_fault;
} alarm_input_t;

typedef struct {
    alarm_state_t state;
    bool led_green, led_yellow, led_red;
    bool buzzer_on, relay_on;
    bool publish_event;         /* vrai quand l'état vient de changer */
} alarm_output_t;

typedef struct {
    uint32_t t_confirm_ms;      /* fenêtre de confirmation d'une intrusion */
    uint32_t t_mute_ms;         /* durée de mise en sourdine */
} alarm_cfg_t;

typedef struct {
    alarm_state_t state;
    alarm_cfg_t cfg;
    uint32_t t_enter_ms;        /* instant d'entrée dans l'état courant */
    bool released;              /* en PRE_ALERT : la détection est retombée au moins une fois */
} alarm_fsm_t;

void alarm_fsm_init(alarm_fsm_t *f, const alarm_cfg_t *cfg);

/* Fonction de transition : aucune E/S, aucun accès à l'horloge. `now_ms` est fourni par l'appelant. */
alarm_output_t alarm_fsm_step(alarm_fsm_t *f, const alarm_input_t *in, uint32_t now_ms);

#ifdef __cplusplus
}
#endif
