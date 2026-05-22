import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

CODEOWNERS = ["@filip"]
DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["sensor", "binary_sensor"]
MULTI_CONF = True

murata_pir_ns = cg.esphome_ns.namespace("murata_pir")
MurataPIRComponent = murata_pir_ns.class_(
    "MurataPIRComponent", cg.PollingComponent, i2c.I2CDevice
)

CONF_THRESHOLD_UPPER = "threshold_upper"
CONF_THRESHOLD_LOWER = "threshold_lower"
CONF_THRESHOLD_COUNT = "threshold_count"
CONF_RESET_TIME = "reset_time"
CONF_SAMPLE_RATE = "sample_rate"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(MurataPIRComponent),
            cv.Optional(CONF_THRESHOLD_UPPER, default=20): cv.int_range(min=0, max=255),
            cv.Optional(CONF_THRESHOLD_LOWER, default=20): cv.int_range(min=0, max=255),
            cv.Optional(CONF_THRESHOLD_COUNT, default=2): cv.int_range(min=1, max=7),
            cv.Optional(CONF_RESET_TIME, default="3s"): cv.positive_time_period_milliseconds,
            cv.Optional(CONF_SAMPLE_RATE, default=100): cv.one_of(50, 100, int=True),
        }
    )
    .extend(cv.polling_component_schema("500ms"))
    .extend(i2c.i2c_device_schema(0x48))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    cg.add(var.set_threshold_upper(config[CONF_THRESHOLD_UPPER]))
    cg.add(var.set_threshold_lower(config[CONF_THRESHOLD_LOWER]))
    cg.add(var.set_threshold_count(config[CONF_THRESHOLD_COUNT]))
    cg.add(var.set_reset_time(config[CONF_RESET_TIME].total_milliseconds))
    cg.add(var.set_sample_rate(config[CONF_SAMPLE_RATE]))
