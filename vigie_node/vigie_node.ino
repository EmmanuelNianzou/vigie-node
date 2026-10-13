// VIGIE NODE : firmware Arduino (ESP32).
// Ouvre ce fichier avec l'Arduino IDE 2.x ; carte : ESP32 Dev Module ; moniteur série : 115200.
#include "src/filters/filters.h"
#include "src/alarm_fsm/alarm_fsm.h"
#include "src/vigie_common/vigie_time.h"

void setup()
{
    Serial.begin(115200);
    delay(200);
    Serial.println("VIGIE NODE boot");
    // Vérifie que le cœur en C pur est bien lié au firmware : doit afficher 21, puis 0.
    Serial.printf("median3(20, 21, 4000) = %ld\n", (long)median3(20, 21, 4000));
    alarm_fsm_t fsm;
    const alarm_cfg_t cfg = {5000, 60000};
    alarm_fsm_init(&fsm, &cfg);
    Serial.printf("etat initial FSM = %d\n", (int)fsm.state); // 0 = DISARMED
}

void loop()
{
    Serial.printf("uptime : %lu ms\n", (unsigned long)vigie_now_ms());
    delay(2000);
}
