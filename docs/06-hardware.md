# Matériel et câblage (VIGIE NODE)

## Carte
ESP32 DevKit 30 broches, module ESP-WROOM-32. Les numéros GPIO de `vigie_node/src/app_config.h`
font foi. GPIO16 et GPIO17 sont étiquetés **RX2** et **TX2** sur la carte.

## Plan des broches
| Signal | GPIO | Étiquette carte | Remarque |
|--------|------|-----------------|----------|
| LED verte | 16 | RX2 | 220 Ω en série |
| LED jaune | 17 | TX2 | 220 Ω en série |
| LED rouge | 18 | D18 | 220 Ω en série |
| LED RGB R / G / B | 19 / 23 / 32 | D19 / D23 / D32 | 220 Ω par canal |
| Buzzer passif | 33 | D33 | PWM |
| Buzzer actif | 14 | D14 | bref signal au démarrage, normal |
| Relais IN1 | 13 | D13 | actif bas, état inactif forcé au démarrage |
| DHT11 | 4 | D4 | 10 kΩ vers 3V3 si capteur nu |
| PIR OUT | 27 | D27 | alimenté en 5 V (VIN), sortie en 3,3 V |
| IR obstacle OUT | 26 | D26 | actif bas |
| Bouton | 25 | D25 | vers GND, pull-up interne |
| Photorésistance | 34 | D34 | ADC1, entrée seule |
| Potentiomètre | 35 | D35 | ADC1, entrée seule |
| OLED SDA / SCL | 21 / 22 | D21 / D22 | adresse I2C à confirmer par scan |

## Alimentation
3V3 (bas droite) pour les capteurs 3,3 V ; VIN (bas gauche) = 5 V pour le relais et le PIR ; GND commun.

## Précautions
- L'ESP32 est en 3,3 V et n'est pas tolérant au 5 V.
- ADC1 uniquement pour l'analogique (GPIO 32 à 39) : l'ADC2 est inutilisable avec le Wi-Fi.
- À éviter : D12, D15, D2, D5 (démarrage), TX0/RX0 (USB), GPIO 6 à 11 (flash).
- Module WROVER : GPIO16 et 17 sont pris par la PSRAM (utiliser les GPIO 36/39 pour les entrées).
- Relais : charges basse tension uniquement. Jamais de 230 V.
