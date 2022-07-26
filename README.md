<a href="https://www.buymeacoffee.com/adonno" target="_blank"><img src="https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png" alt="Buy Me A Coffee" style="height: 41px !important;width: 174px !important;box-shadow: 0px 3px 2px 0px rgba(190, 190, 190, 0.5) !important;-webkit-box-shadow: 0px 3px 2px 0px rgba(190, 190, 190, 0.5) !important;" ></a>
[![Discord](https://img.shields.io/discord/755394229944975380)](https://discord.gg/4SDcaRS)
[![GitHub release](https://img.shields.io/github/release/adonno/tagreader.svg)](https://GitHub.com/adonno/tagreader/releases/)

# Tag Reader for Home Assistant

The tag reader is a simple to build/use NFC tag reader, specially created for [Home Assistant](https://www.home-assistant.io). It is using a D1 mini ESP 8266 and the PN532 NFC module. The firmware is built using [ESPhome](https://www.esphome.io).

> I am selling a pre-built version, a DIY version (assembly required), or just the case (use with own components). Check it out on [my website](https://adonno-crafts.myshopify.com/).

![Photos of the final product](docs/cases.jpg)

## Building the tag reader

To build your own tag reader, you need the following components:

 - [ESP8266 D1 Mini](https://s.click.aliexpress.com/e/_d8l72oB)  
 - [PN532 NFC Reader](https://s.click.aliexpress.com/e/_dZNORIJ)
 - [WS2812](https://s.click.aliexpress.com/e/_d82GRqr)
 - [Buzzer](https://s.click.aliexpress.com/e/_dZ5F5yj)

The 3D models for the case are [here](STLs).

~~**WARNING** regarding AZdelivery D1. We have had several users contacting us with different issues, we don't recommend using these boards and won't provide support for them.~~  
Recent reports suggest more clones of the D1 are now working as expected. This has been verified on an AZdelivery D1.

### Connecting the components

![Photo of schematics](Schematics/tag_reader_schematics_v3.2.png)

There are not too many components to connect, but it does require soldering. You will need the following:

- [Solder](https://s.click.aliexpress.com/e/_dT3S62j)
- [Soldering iron with a fairly thin tip](https://s.click.aliexpress.com/e/_dXaI6nz)
- [About 40cm of thin wire (at least 5 different colors)](https://s.click.aliexpress.com/e/_dZvoYoB)


Also, make sure that you have set the switches on the PN532 to the following:
- Switch 1: On (up)
- Switch 2: Off (down)

This enables the PN532 module to communicate with the D1 over I2C, and is required for the modules to work together!

To flash the reader firmware to your D1 Mini you point ESPHome at [tagreader.yaml](tagreader.yaml).  
> :warning: The tag reader requires ESPHome `1.16.0`.

If you're new to ESPHome, we recommend that you use the [ESPHome Home Assistant add-on](https://esphome.io/guides/getting_started_hassio.html).

![Open Case](docs/inside-case-completed.jpg)

## Configuring for use with Home Assistant

The tag reader requires [Home Assistant](https://www.home-assistant.io) 0.115 or later.

If the tag reader is unable to connect to a wifi network, it will start a WiFi access point with a captive portal to allow you to enter your WiFi credentials.

The tag reader will be automatically discovered by Home Assistant once the tag reader is connected to the same network. You can follow the instructions in the UI to set it up.

## Usage

Scanned tags can be managed from the tags interface in Home Assistant. You can find it under config -> tags.

![Screenshot of the Home Assistant tag UI](docs/tag-ui.gif)

## Disclamer

We use aliexpress affiliate links for the components and the tools. Some Ad-blockers might block these links and thus they seem to appear broken. You will have to temporarily disable the ad-blocker to open these links. 
