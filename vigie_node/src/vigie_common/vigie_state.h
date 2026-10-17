#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    float temp_c, hum_pct;
    int lux_raw, pot_raw;
    bool dht_ok, pir, obstacle;
    uint8_t alarm_state;
    bool relay_on;
    bool net_up;
    bool mqtt_up;
    bool provisioning;
    char prov_pop[12];
    uint32_t t_ms;
} vigie_state_t;

void vigie_state_init(void);
vigie_state_t *vigie_state_lock(void);
void vigie_state_unlock(void);
void vigie_state_copy(vigie_state_t *out);
const char *alarm_state_name(uint8_t s);
