#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace murata_pir {

class MurataPIRComponent : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_threshold_upper(uint8_t v) { threshold_upper_ = v; }
  void set_threshold_lower(uint8_t v) { threshold_lower_ = v; }
  void set_threshold_count(uint8_t v) { threshold_count_ = v; }
  void set_reset_time(uint32_t v) { reset_time_ = v; }
  void set_sample_rate(uint16_t v) { sample_rate_ = v; }

  void set_raw_sensor(sensor::Sensor *s) { raw_sensor_ = s; }
  void set_motion_sensor(binary_sensor::BinarySensor *s) { motion_sensor_ = s; }

 protected:
  uint8_t threshold_upper_{20};
  uint8_t threshold_lower_{20};
  uint8_t threshold_count_{2};
  uint32_t reset_time_{3000};
  uint16_t sample_rate_{100};

  sensor::Sensor *raw_sensor_{nullptr};
  binary_sensor::BinarySensor *motion_sensor_{nullptr};

  uint32_t last_motion_{0};
  bool motion_state_{false};

  bool write_reg_(uint8_t reg, uint8_t val);
  bool read_reg_(uint8_t reg, uint8_t *val);
  bool read_data_(int16_t *data);
  void clear_status_();
};

}  // namespace murata_pir
}  // namespace esphome
