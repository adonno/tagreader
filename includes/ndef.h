#pragma once

#include "esphome.h"

#include "NfcAdapter.h"
#include "PN532/PN532.h"
#include "PN532_SPI/PN532_SPI.h"

static const char *TAG = "ndef_tag_reader";

static const std::string TAG_PREFIX = "home-assistant.io/tag/";

class NDEFTagReader : public PollingComponent, public TextSensor {
 public:
  NDEFTagReader() : PollingComponent(1000) {}
  void setup() override {
    this->pn532spi_ = new PN532_SPI(SPI, 0);
    this->nfc_ = new NfcAdapter(*this->pn532spi_);
    this->nfc_->begin();
  }

  void update() override {
    if (this->nfc_->tagPresent()) {
      ESP_LOGD(TAG, "Tag Found!");
      NfcTag tag = this->nfc_->read();

      if (tag.hasNdefMessage()) {
        ESP_LOGD(TAG, "Tag has NDEF");
        NdefMessage message = tag.getNdefMessage();
        int recordCount = message.getRecordCount();
        ESP_LOGD(TAG, "Tag has %d records", recordCount);
        for (int i = 0; i < recordCount; i++) {
          NdefRecord record = message.getRecord(i);

          ESP_LOGD(TAG, "Record %d", (i+1));
          ESP_LOGD(TAG, "Type: %s", record.getType().c_str());

          int payloadLength = record.getPayloadLength();
          byte payload[payloadLength];
          record.getPayload(payload);

          std::string payloadAsString;
          for (int c = 0; c < payloadLength; c++) {
            payloadAsString += (char)payload[c];
          }
          ESP_LOGD(TAG, "Payload: %s", payloadAsString.c_str());
          size_t pos = payloadAsString.find(TAG_PREFIX);
          if (pos != std::string::npos) {
            this->publish_state(payloadAsString.substr(pos + TAG_PREFIX.length()));
            this->set_timeout("tag_clear", 1000, [this]() { this->publish_state(""); });
          }
        }
      } else {
        ESP_LOGD(TAG, "No NDEF");
      }
    }
  }
 protected:
  PN532_SPI *pn532spi_;
  NfcAdapter *nfc_;
};
