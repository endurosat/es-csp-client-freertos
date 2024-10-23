# Getting Started

```{contents}
:depth: 3
```

## Setup

The setup consists of RPi4 acting as a server for the inbound commands issued by the
STM32H7555 board. The RPi4 can be used also for development & flashing the STM32H7555 board
since it is running Ubuntu. The steps from this page can be executed on the RPi4 board.
The steps marked with ![Raspberry Pi](https://img.shields.io/badge/-RaspberryPi-C51A4A?style=for-the-badge&logo=Raspberry-Pi)
can be executed on the Raspberry from the test setup.

### Hardware

* [RPi4](https://www.amazon.com/Raspberry-Model-2019-Quad-Bluetooth/dp/B07TC2BK1X/ref=sr_1_3?crid=3V6EUSV9956IA&dib=eyJ2IjoiMSJ9.l-BqsrQVZQ1hEJh6W55CEH9BPs8tHiIfDAEfKZ5lNNtfUFFY_T7T-tCG2A5BWlDdTsLk1xutjDpss9zKS5GRc6b2Uxz6MdgmkTscY1fhg-7W8hubsmLcNDiD4eXCsEo13f4Onx2ozdOUJWCO7CaeZ_B8nC0ZzvaddIRMPmRxj_9cwYGsMMQNKOVVoWVY9SRqkHCsQrYxUgC2AfPCO-0YThUdg1FwzKnH6nsPGp67cAk.nPtgTasTqbjg4gTZGG6l0LNMZTwuTmM_qgmcTi9iDuw&dib_tag=se&keywords=raspberry%2Bpi&qid=1729673886&sprefix=raspber%2Caps%2C194&sr=8-3&th=1) (x1)
* [FTDI 232](https://www.amazon.com/HiLetgo-FT232RL-Converter-Adapter-Breakout/dp/B00IJXZQ7C/ref=sr_1_1_sspa?dib=eyJ2IjoiMSJ9.K2aU-dlTd45oLwa2bR7pJ1po1kY_2XEjzv3VnO7YVgHRxSunJPSORA3fmdJpc_Oa-zNBNwKYG9m6t0gbtZS_sWqo896WkBQMm8lghT9NcHUzaHyYxNnzqj5162EB7sqJXagYY7IfA0quD8HpIiSJNcy0-BDLn4DJdW93VEmW4XyMYv3QPmMyiJThh8AXnyTez4gGuzvMprDuhQ_dZIgipiOuKujuD60SOx5CXJfbhgY.kJZ5jFHgT_yRatCZklLa6kWHjQ3g7moW5XRRD0ShIEg&dib_tag=se&keywords=ft232rl&qid=1729673788&sr=8-1-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1) (x1)
* [NUCLEO H755](https://www.amazon.com/XFCZMG-NUCLEO-H755ZI-Q-Nucleo-144-Development-STM32H755ZI/dp/B0CKVZ2X7Z/ref=sr_1_1?crid=39PV2CYAK688F&dib=eyJ2IjoiMSJ9.vTBYp3GDS7fdalb3bym8GV968xxXVHqu3Hya2FMk9FoUqz4XuxTXbEWuwShAY3AC7CD43MM7LU0lvFj1dZXw_JLWkG17NTFx1dldlvzeVQ9fTh1tLLIZw2rJwNgSwUGDlyMwAfcl-NhvAGzxvbJkGFW1MD_yIXSaNI735PZy6nynAnR-yMc0tHWa3pWRKAqK._fceoe_HtCNZ-M0Q8Kz1ZgqmRJvP4EE1_seyqOtMDvw&dib_tag=se&keywords=nucleo+h755&qid=1729673847&sprefix=nucleoh755%2Caps%2C168&sr=8-1) (x1)
* [SPI SD Card Module](https://www.amazon.de/-/en/ANGEEK-Modules-Compatible-Arduino-Raspberry/dp/B07S6K3RVK/) (x1)
* Mini USB cables (x1) - FTDI
* Micro USB cable (x1) - Debug UART
* Micro HDMI cable (x1) - Monitor
* Keyboard & mouse

<script type="text/javascript" src="./_static/js/custom.js"></script>
<link rel="stylesheet" type="text/css" href="./_static/css/custom.css" media="screen" />

<div id="img-viewer">
    <span class="close" onclick="close_model()">&times;</span>
    <img class="modal-content" id="full-image" >
</div>

<div  class='img-container'>
<img src="./_static/setup.png" class='img-source'>

<!-- Expand Icon -->
<img src=" data:image/png;base64,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"  class='expand-icon' onclick="full_view(this);" >
</div>
<br>
<br>

```{warning}
Please note that the FTDI chips should be configured with jumper on 3.3v
```

## Dependencies

* cmake - Build system
* openocd - Flashing & debug utilities
* gcc-arm-none-eabi - Cross compiler
* gdb-multiarch - Debug utility
* minicom - Debug utility (serial console)

**To install the dependencies use the following command**

```console
$ sudo apt-get -y install cmake openocd gcc-arm-none-eabi build-essential gdb-multiarch libnewlib-arm-none-eabi minicom
```

## Usage

![Raspberry Pi](https://img.shields.io/badge/-RaspberryPi-C51A4A?style=for-the-badge&logo=Raspberry-Pi)

1. Clone the repository

```console
$ git clone https://github.com/endurosat/es-csp-client-freertos
```

2. Build

```console
$ source configure
$ es_build
```

The resulting build artefacts are stored inside the `build` directory. After a succesfull
build the contents of this directory should look like this:

```console
. build
├── CMakeCache.txt
├── CMakeFiles
├── cmake_install.cmake
├── csp
├── esclient-api
├── es_client_lib_CM7.bin
├── es_client_lib_CM7.elf
├── es_client_lib_CM7.hex
├── es_client_lib_CM7.map
├── FATFS
├── FreeRTOS
├── HAL
└── Makefile
```

## Open the Debug UART

![Raspberry Pi](https://img.shields.io/badge/-RaspberryPi-C51A4A?style=for-the-badge&logo=Raspberry-Pi)

```console
$ minicom -b 115200 -D /dev/ttyACM0
```

## Start the ES CSP server

In order to build and start the es csp server for these [instructions](https://endurosat.github.io/es-csp-server/)

## Flash the target (STM32H755 Nucleo Board)

![Raspberry Pi](https://img.shields.io/badge/-RaspberryPi-C51A4A?style=for-the-badge&logo=Raspberry-Pi)

Use the following command to flash the target.

```console
$ source configure
$ es_flash_target
```

At this point the green LED on the development board should be toggling each 1000ms.
The yellow LED is toggled on each byte received over the UART. The red LED is set
if the application enters an error hook.

On the debug UART similar output to the one below should be displayed:

```console
2 main [INFO] (47): ES Client v100
5 es_sd_card [INFO] (576): SD Card Mounted Successfully
5 es_sd_card [INFO] (580): TotalSpace : 7794688 bytes, FreeSpace = 7791264 bytes
5 es_client [TRACE] (54): Initializing ES Client
5 es_tasks [INFO] (108): Starting: Default task
5 es_tasks [INFO] (108): Starting: ES Client task
 .
 .
 .
```
