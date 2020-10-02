#pragma once

#define tag_reader_i2c 1

#include "esphome.h"

#include "NfcAdapter.h"
#include "PN532/PN532.h"

#if tag_reader_i2c
#include "PN532_I2C/PN532_I2C.h"
#else
#include "PN532_SPI/PN532_SPI.h"
#endif

std::string replace_all(std::string str, const std::string& from, const std::string& to) {
  size_t start_pos = 0;
  while((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
  }
  return str;
}

static const char *TAG = "ndef_tag_reader";

static const std::string TAG_PREFIX = "home-assistant.io/tag/";

class NDEFTagReader : public PollingComponent, public TextSensor {
 public:
  NDEFTagReader() : PollingComponent(1000) {}
  void setup() override {
    this->publish_state("");
#if tag_reader_i2c
    this->pn532_ = new PN532_I2C(Wire);
#else
    this->pn532_ = new PN532_SPI(SPI, 0);
#endif
    this->nfc_ = new NfcAdapter(*this->pn532_);
    this->reader_setup_ = this->nfc_->begin(true);
    if (!this->reader_setup_) {
      ESP_LOGE(TAG, "PN532 could not be found or setup, please check connections.");
      this->mark_failed();
      return;
    }
  }

  void update() override {
    if (!this->reader_setup_) {
      return;
    }

    if (!this->nfc_->tagPresent(1)) {
      this->nfc_->powerOff();
      return;
    }

    ESP_LOGD(TAG, "Tag Found!");
    NfcTag tag = this->nfc_->read();

    if (!tag.hasNdefMessage()) {
      this->publish_uid(&tag);
      return;
    }

    ESP_LOGD(TAG, "Tag has NDEF");
    NdefMessage message = tag.getNdefMessage();
    int recordCount = message.getRecordCount();
    ESP_LOGD(TAG, "Tag has %d records", recordCount);

    bool got_url = false;

    for (int i = 0; i < recordCount; i++) {
      NdefRecord record = message.getRecord(i);

      ESP_LOGV(TAG, "Record %d", (i+1));
      ESP_LOGV(TAG, "Type: %s", record.getType().c_str());

      int payloadLength = record.getPayloadLength();
      byte payload[payloadLength];
      record.getPayload(payload);

      std::string payloadAsString;
      for (int c = 0; c < payloadLength; c++) {
        payloadAsString += (char)payload[c];
      }
      ESP_LOGV(TAG, "Payload: %s", payloadAsString.c_str());
      size_t pos = payloadAsString.find(TAG_PREFIX);
      if (pos != std::string::npos) {
        this->publish_state(payloadAsString.substr(pos + TAG_PREFIX.length()));
        this->set_timeout("tag_clear", 1000, [this]() { this->publish_state(""); });
        got_url = true;
      }
    }

    if (!got_url) {
      this->publish_uid(&tag);
    }
    this->nfc_->powerOff();
  }
 protected:

  void publish_uid(NfcTag *tag) {
    ESP_LOGD(TAG, "No NDEF");
    std::string uid(tag->getUidString().c_str());
    uid = replace_all(uid, " ", "-");
    ESP_LOGD(TAG, "Tag UID: %s", uid.c_str());
    this->publish_state(uid);
    this->set_timeout("tag_clear", 1000, [this]() { this->publish_state(""); });
  }

  PN532Interface *pn532_;
  NfcAdapter *nfc_;
  boolean reader_setup_{false};
};
