# VIGIE NODE

> Nœud IoT ESP32 sécurisé : capteurs, alarme locale, MQTT/TLS, OTA signée. Développé avec l'**Arduino IDE** (cœur Arduino-ESP32). (En construction.)

![CI](https://github.com/EmmanuelNianzou/vigie-node/actions/workflows/ci.yml/badge.svg)

## Environnement de développement : Arduino IDE

| Élément | Valeur |
|---------|--------|
| IDE | Arduino IDE 2.x |
| Cœur de carte | « esp32 » par Espressif Systems (Boards Manager) |
| Carte | ESP32 Dev Module |
| Partitions | schéma par défaut (deux emplacements d'application, nécessaires à l'OTA) |
| Moniteur série | 115200 bauds |
| Testé avec | TODO : versions exactes de l'IDE et du cœur esp32 |

## Démarrage rapide

1. Installe l'Arduino IDE 2.x.
2. File > Preferences : dans « Additional boards manager URLs », ajoute
   `https://espressif.github.io/arduino-esp32/package_esp32_index.json`.
3. Tools > Board > Boards Manager : installe **esp32** (Espressif Systems).
4. Ouvre `vigie_node/vigie_node.ino`, choisis la carte **ESP32 Dev Module** et le port, puis Upload.
5. Ouvre le moniteur série à 115200 : le nœud affiche `VIGIE NODE boot`.

## Tests sur PC

Le cœur en C pur (`vigie_node/src/filters`, puis la machine à états d'alarme, le codec et le cache de nonces)
est compilé sur PC avec CMake et testé avec Unity, sans carte ni IDE :

    make test

## Structure

    vigie_node/            sketch Arduino (ouvre vigie_node.ino)
      src/                 code du firmware, compilé par l'IDE
        filters/           cœur pur : EMA, hystérésis, médiane
        vigie_common/      horloge
    test_apps/host_unit/   tests Unity exécutés sur PC
    contracts/             contrats JSON (télémétrie, commandes)
    tools/                 outils (issues, signature du firmware, certificats)
    docs/                  documentation

## Pourquoi Arduino

Prise en main rapide et large choix de bibliothèques. La logique métier reste en C standard, indépendante
du framework, donc testée sur PC et réutilisable (par exemple sous Zephyr dans vigie-edge).

## État d'avancement

Voir le jalon « v0.1.0 MVP » dans les issues.
