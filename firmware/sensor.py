import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import STATE_CLASS_MEASUREMENT

from . import MurataPIRComponent, murata_pir_ns

AUTO_LOAD = ["murata_pir"]

CONF_MURATA_PIR_ID = "murata_pir_id"
CONF_RAW = "raw"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_MURATA_PIR_ID): cv.use_id(MurataPIRComponent),
        cv.Optional(CONF_RAW): sensor.sensor_schema(
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_MURATA_PIR_ID])
    if CONF_RAW in config:
        s = await sensor.new_sensor(config[CONF_RAW])
        cg.add(parent.set_raw_sensor(s))
