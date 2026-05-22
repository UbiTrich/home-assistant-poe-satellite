import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import DEVICE_CLASS_MOTION

from . import MurataPIRComponent

AUTO_LOAD = ["murata_pir"]

CONF_MURATA_PIR_ID = "murata_pir_id"
CONF_MOTION = "motion"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_MURATA_PIR_ID): cv.use_id(MurataPIRComponent),
        cv.Optional(CONF_MOTION): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_MOTION,
        ),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_MURATA_PIR_ID])
    if CONF_MOTION in config:
        bs = await binary_sensor.new_binary_sensor(config[CONF_MOTION])
        cg.add(parent.set_motion_sensor(bs))
