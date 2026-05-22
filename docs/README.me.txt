# Home Assistant PoE Voice & Presence Satellite

[![ESPHome](https://img.shields.io/badge/ESPHome-Ready-blue.svg)](https://esphome.io/)
[![Hardware](https://img.shields.io/badge/Hardware-ESP32--S3-green.svg)]()
[![Status](https://img.shields.io/badge/Status-Prototype_Rev1-orange.svg)]()

*(Slovak translation below)*

## About the Project
This repository contains the hardware and software design files for a smart home satellite node developed as a Bachelor's thesis at BUT FME (VUT FSI). The device is built around the ESP32-S3 microcontroller and combines local voice assistant capabilities with reliable presence detection. It is powered entirely via Power over Ethernet (PoE) and integrates seamlessly with Home Assistant and the Loxone ecosystem.

## O projekte
Tento repozitár obsahuje hardvérový návrh a firmvér pre inteligentný satelitný uzol, ktorý vznikol ako bakalárska práca na FSI VUT v Brne. Zariadenie kombinuje lokálneho hlasového asistenta (microWake Word) a spoľahlivú detekciu prítomnosti pomocou fúzie PIR senzora a mmWave radaru. Celé zariadenie je napájané a pripojené do siete pomocou jediného PoE (Power over Ethernet) kábla.

## 🌟 Key Features / Kľúčové vlastnosti
* **Single-cable operation:** Power and data transmission via Ethernet (IEEE 802.3af PoE).
* **Presence Sensor Fusion:** Combines a PIR sensor (motion) and a 24GHz mmWave radar (static presence) for zero false-negatives.
* **Local Voice Assistant:** On-device wake word detection (`microWakeWord`) via an I2S MEMS microphone, completely cloud-free.
* **Smart Home Integration:** Native ESPHome API for Home Assistant and WebSocket proxy for Loxone Miniserver.
* **Custom ESPHome Component:** Includes a custom C++ driver (`murata_pir`) for the Murata IRS-D200ST00R1 I2C sensor.

## 🛠️ Hardware Specifications
* **MCU:** ESP32-S3-WROOM-1U-N16R8 (16MB Flash, 8MB PSRAM for audio buffers)
* **Ethernet:** WIZnet W5500 SPI controller
* **PoE Module:** Silvertel Ag9905M (48V to 5V DC/DC)
* **Radar:** HLK-LD2410 (24 GHz mmWave)
* **Audio:** INMP441 (I2S Digital MEMS Microphone)
* **Form Factor:** Custom 80mm circular 4-layer PCB, designed to fit standard ceiling mounts.

## 📂 Repository Structure
* `/hardware/` - KiCad 9 project files (Schematics, PCB layout, Gerber files, BOM).
* `/firmware/` - ESPHome YAML configuration files.
* `/firmware/custom_components/murata_pir/` - Source code for the custom I2C PIR sensor component.
* `/enclosure/` - 3D models (STL/STEP) for the ceiling-mount case.
* `/docs/` - Setup instructions, wiring diagrams, and testing data.

## ⚠️ Known Limitations (Revision 1)
* **PoE Compatibility:** Current PCB revision implements only Mode B for PoE. Endspan switches using Mode A will not power the device without a midspan injector.
* **PIR Sensor:** The LGA package of the Murata sensor proved difficult to hand-solder and was prone to thermal damage. The current prototype falls back to a standard THT PIR module, though the custom I2C component remains available in the codebase.

## License / Licencia
This project is open-source. Hardware files and software code are provided as-is. 
Author: Filip Lettrich (Bachelor's Thesis, 2026)