#pragma once
#define FW_VERSION "0.1.0"
#define VIGIE_CONSOLE 1

#define PIN_I2C_SDA 21
#define PIN_I2C_SCL 22
#define PIN_DHT11 4
#define PIN_PIR 27
#define PIN_IR_OBSTACLE 26
#define PIN_BUTTON 25
#define PIN_LED_GREEN 16
#define PIN_LED_YELLOW 17
#define PIN_LED_RED 18
#define PIN_RGB_R 19
#define PIN_RGB_G 23
#define PIN_RGB_B 32
#define PIN_BUZZER_PASSIVE 33
#define PIN_BUZZER_ACTIVE 14
#define PIN_RELAY_IN1 13
#define PIN_LDR 34
#define PIN_POT 35
#define RELAY_ACTIVE_LOW true

#define INPUT_POLL_MS 10
#define INPUT_DEBOUNCE_SAMPLES 4
#define LONG_PRESS_MS 1500
#define ALARM_T_CONFIRM_MS 5000
#define ALARM_T_MUTE_MS 60000

#define NTP_SERVER "pool.ntp.org"
#define MQTT_BROKER_HOST "broker.lan"
#define MQTT_BROKER_PORT 8883
#define TELEMETRY_PERIOD_S 10
