# Tag Reader for Home Assistant

The tag reader is a simple to build/use NFC tag reader, specially created for [Home Assistant](https://www.home-assistant.io). It is using a Wemos D1 and the PN532 NFC module. The firmware is built using [ESPhome](https://www.esphome.io).

![Photos of the final product](https://raw.githubusercontent.com/adonno/tagreader/master/docs/cases.jpg)

## Configuring for use with Home Assistant

If the tag reader is unable to connect to a wifi network, it will start a WiFi access point with a captive portal to allow you to enter your WiFi credentials.

The tag reader will be automatically found by Home Assistant once the tag reader is connected to the same network. You can follow the instructions in the UI to set it up.

A custom component is required for Home Assistant at this time to link the tag reader to the tag integration in Home Assistant. Copy [this file](https://raw.githubusercontent.com/adonno/tagreader/master/custom_components/esphome_tag.py) to `<config>/custom_components/esphome_tag.py` and add `esphome_tag:` to your `configuration.yaml` file.

## Usage

Each tag that is scanned will result in a tag scan event in Home Assistant. You can manage your tags and automate them in the Home Assistant UI under config -> tags.

![Screenshot of the Home Assistant tag UI](https://raw.githubusercontent.com/adonno/tagreader/master/docs/tag-ui.gif)

## Options

The tag reader is available in three different configurations:

- Plug-and-play. A pre-built, pre-configured tag reader, ready to use.
- Pre-configured components. All required components and the case for you to enjoy building yourself.
- Case only. Order a case to use with your own components.

## Additional component

In order to be able to use the tag reader with the new tag integration added in Home Assistant v 0.115, a special custom component is required

## Contributing

TODO

## License

TODO
