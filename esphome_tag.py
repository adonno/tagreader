from homeassistant.const import MAJOR_VERSION, MINOR_VERSION
from homeassistant.helpers import device_registry

DOMAIN = "esphome_tag"


async def async_setup(hass, config):
    if MAJOR_VERSION > 0 or MINOR_VERSION >= 115:

        async def scan_tag(tag_id, device_id):
            await hass.components.tag.async_scan_tag(tag_id, device_id)

    else:
        # Mimick tag integration in HA 0.115+
        async def scan_tag(tag_id, device_id):
            hass.bus.async_fire(
                "tag_scanned",
                {
                    "tag_id": tag_id,
                    "device_id": device_id,
                },
            )

    device_ids = {}

    async def handle_event(ev):
        # Find ESPHome Device ID
        esphome_id = ev.data["device_id"]

        device_id = device_ids.get(esphome_id)

        if device_id is not None:
            await scan_tag(ev.data["tag_id"], device_id)
            return

        esphome_entry = None

        # Find ESPHome entry based on esphome_id
        for entry in hass.config_entries.async_entries("esphome"):
            if entry.unique_id:
                esphome_entry = entry
                break

        if esphome_entry is None:
            await scan_tag(ev.data["tag_id"], device_id)
            return

        # Find ESPHome device in registry
        dev_reg = await device_registry.async_get_registry(hass)
        devices = device_registry.async_entries_for_config_entry(
            dev_reg, esphome_entry.entry_id
        )

        if devices:
            device_id = devices[0].id
            device_ids[esphome_id] = device_id

        await scan_tag(ev.data["tag_id"], device_id)

    hass.bus.async_listen("esphome.rfid_read", handle_event)

    return True