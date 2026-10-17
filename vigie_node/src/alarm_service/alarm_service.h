#pragma once
#include <stdbool.h>
void alarm_service_start(void);
void alarm_service_cmd_arm(void);
void alarm_service_cmd_disarm(void);
void alarm_service_relay_manual(bool on); // commande distante du relais
