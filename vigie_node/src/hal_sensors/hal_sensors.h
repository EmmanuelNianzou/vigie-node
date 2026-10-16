#pragma once
#include <stdint.h>
typedef enum { DHT_OK, DHT_TOO_SOON, DHT_TIMEOUT, DHT_BAD_CRC } dht_status_t;
void adc_init(void);
int adc_read_raw(uint8_t pin);
dht_status_t dht11_read(uint8_t pin, float *temp_c, float *hum_pct);
