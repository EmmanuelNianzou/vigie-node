# Matériel et câblage (VIGIE NODE)

## Carte
ESP32 DevKit (WROOM-32). Vérifie le pinout de la carte avant de câbler.

## Plan des broches (source de vérité : `vigie_node/src/app_config.h`)
| GPIO | Fonction | Remarque |
|------|----------|----------|
| 21 / 22 | OLED SDA / SCL (I2C) | |
| 4 | DHT11 DATA | |
| 27 | PIR OUT | alimenté en 5 V, sortie en 3,3 V |
| 26 | IR obstacle OUT | actif bas |
| 34 / 35 | Photorésistance / potentiomètre | ADC1, entrées seules |
| 25 | Bouton vers GND | pull-up interne |
| 16 / 17 / 18 | LED verte / jaune / rouge | 220 Ω en série |
| 19 / 23 / 32 | LED RGB R / G / B | 220 Ω par canal |
| 33 | Buzzer passif (PWM) | une fréquence = une note |
| 14 | Buzzer actif | |
| 13 | Relais IN1 | actif bas ; état inactif forcé au démarrage |

## Précautions
- L'ESP32 est en 3,3 V et n'est pas tolérant au 5 V.
- ADC1 uniquement (GPIO 32 à 39) : l'ADC2 est inutilisable quand le Wi-Fi est actif.
- Évitez les GPIO 0, 2, 12, 15 (au démarrage) et 6 à 11 (pendant le flash).
- Relais : charges basse tension uniquement (LED, petit ventilateur 5/12 V). On a pas eu besoin de mettre du 230 V.
