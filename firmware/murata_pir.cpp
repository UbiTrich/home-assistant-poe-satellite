#include "murata_pir.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace murata_pir {

static const char *const TAG = "murata_pir";

static const uint8_t REG_OP          = 0x00;
static const uint8_t REG_DATA_LO     = 0x02;
static const uint8_t REG_DATA_HI     = 0x03;
static const uint8_t REG_STATUS      = 0x04;
static const uint8_t REG_SAMPLR      = 0x06;
static const uint8_t REG_LPFR_HPFR   = 0x07;
static const uint8_t REG_INTSEL      = 0x09;
static const uint8_t REG_THRESHCOUNT = 0x0A;
static const uint8_t REG_THRESH_HR   = 0x0B;
static const uint8_t REG_THRESH_LR   = 0x0C;

void MurataPIRComponent::setup() {
  ESP_LOGCONFIG(TAG, "Inicializujem Murata PIR...");

  // Zastav meranie
  if (!write_reg_(REG_OP, 0x01)) {
    ESP_LOGE(TAG, "PIR neodpoveda na I2C!");
    this->mark_failed();
    return;
  }
  delay(50);

  // Sample rate
  uint8_t samplr = (sample_rate_ == 50) ? 0x01 : 0x00;
  write_reg_(REG_SAMPLR, samplr);

  // Filter - stredny LPF/HPF
  write_reg_(REG_LPFR_HPFR, 0x22);

  // Threshold nastavenia
  write_reg_(REG_THRESHCOUNT, threshold_count_ & 0x07);
  write_reg_(REG_THRESH_HR, threshold_upper_);
  write_reg_(REG_THRESH_LR, threshold_lower_);

  // INTSEL: 0x08 = OR mode
  write_reg_(REG_INTSEL, 0x08);

  // Clear status
  clear_status_();

  delay(50);

  // Spusti meranie
  write_reg_(REG_OP, 0x00);
  delay(100);

  ESP_LOGCONFIG(TAG, "Murata PIR OK");
}

void MurataPIRComponent::update() {
  uint8_t status = 0;
  if (!read_reg_(REG_STATUS, &status)) {
    ESP_LOGW(TAG, "Chyba pri citani status");
    return;
  }

  if (status & 0x01) {
    int16_t data;
    if (read_data_(&data)) {
      if (raw_sensor_ != nullptr) {
        raw_sensor_->publish_state(data);
      }

      // Softwarova detekcia pohybu - threshold * 128 podla datasheetu
      int16_t threshold_digits = threshold_upper_ * 128;
      bool exceeded = abs(data) > threshold_digits;

      ESP_LOGD(TAG, "data=%d thr=%d exceeded=%d motion_sensor=%d state=%d",
               data, threshold_digits, exceeded,
               motion_sensor_ != nullptr, motion_state_);

      if (exceeded) {
        last_motion_ = millis();
        if (!motion_state_) {
          motion_state_ = true;
          if (motion_sensor_ != nullptr) {
            motion_sensor_->publish_state(true);
            ESP_LOGI(TAG, "POHYB DETEKOVANY! data=%d", data);
          } else {
            ESP_LOGW(TAG, "motion_sensor je NULL!");
          }
        }
      } else if (motion_state_ && (millis() - last_motion_) > reset_time_) {
        motion_state_ = false;
        if (motion_sensor_ != nullptr) {
          motion_sensor_->publish_state(false);
          ESP_LOGI(TAG, "Pohyb skoncil");
        }
      }
    }
    // Vymaz bit 0 (data read flag) - senzor zapise dalsie data
    write_reg_(REG_STATUS, 0x01);
  }
}

void MurataPIRComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Murata PIR IRS-D200ST00R1:");
  LOG_I2C_DEVICE(this);
  ESP_LOGCONFIG(TAG, "  Threshold Upper: %u", threshold_upper_);
  ESP_LOGCONFIG(TAG, "  Threshold Lower: %u", threshold_lower_);
  ESP_LOGCONFIG(TAG, "  Threshold Count: %u", threshold_count_);
  ESP_LOGCONFIG(TAG, "  Reset Time: %u ms", reset_time_);
  ESP_LOGCONFIG(TAG, "  Sample Rate: %u Hz", sample_rate_);
  LOG_UPDATE_INTERVAL(this);
}

bool MurataPIRComponent::write_reg_(uint8_t reg, uint8_t val) {
  return this->write_byte(reg, val);
}

bool MurataPIRComponent::read_reg_(uint8_t reg, uint8_t *val) {
  return this->read_byte(reg, val);
}

bool MurataPIRComponent::read_data_(int16_t *data) {
  uint8_t lo, hi;
  if (!read_reg_(REG_DATA_LO, &lo)) return false;
  if (!read_reg_(REG_DATA_HI, &hi)) return false;
  *data = (int16_t)((hi << 8) | lo);
  return true;
}

void MurataPIRComponent::clear_status_() {
  write_reg_(REG_STATUS, 0x09);  // Clear bit 0 aj bit 3
}

}  // namespace murata_pir
}  // namespace esphome
