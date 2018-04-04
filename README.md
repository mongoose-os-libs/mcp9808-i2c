# MCP9808 I2C Driver

A Mongoose library for Microchip's MCP9808 integrated circuit.

## Sensor details

Microchip Technology Inc.’s MCP9808 digital temperature sensor converts
temperatures between -20°C and +100°C to a digital word with
±0.25°C/±0.5°C (typical/maximum) accuracy.

The MCP9808 comes with user-programmable registers that provide flexibility
for temperature sensing applications. The registers allow user-selectable
settings such as Shutdown or Low-Power modes and the specification of
temperature Alert window limits and critical output limits. When the
temperature changes beyond the specified boundary limits, the MCP9808 outputs
an Alert signal. The user has the option of setting the Alert output signal
polarity as an active-low or activehigh comparator output for thermostat
operation, or as a temperature Alert interrupt output for microprocessor based
systems. The Alert output can also be configured as a critical temperature
output only.

This sensor has an industry standard 400 kHz, 2-wire, SMBus/I2C compatible
serial interface, allowing up to eight or sixteen sensors to be controlled with
a single serial bus.

These features make the MCP9808 ideal for sophisticated, multi-zone,
temperature-monitoring applications.

See [datasheet](https://cdn-shop.adafruit.com/datasheets/MCP9808.pdf)
for implementation details.

A great place to buy a ready made and tested unit is at [Adafruit](https://learn.adafruit.com/adafruit-mcp9808-precision-i2c-temperature-sensor-guide).

## Example application

An example program using a timer to read data from the sensor every 5 seconds:

```
#include "mgos.h"
#include "mgos_i2c.h"
#include "mgos_mcp9808.h"

static struct mgos_mcp9808 *s_mcp9808;

static void timer_cb(void *user_data) {
  float temperature;

  temperature=mgos_mcp9808_getTemperature(s_mcp9808);

  LOG(LL_INFO, ("mcp9808 temperature=%.2f", temperature));

  (void) user_data;
}

enum mgos_app_init_result mgos_app_init(void) {
  struct mgos_i2c *i2c;

  i2c=mgos_i2c_get_global();
  if (!i2c) {
    LOG(LL_ERROR, ("I2C bus missing, set i2c.enable=true in mos.yml"));
  } else {
    s_mcp9808=mgos_mcp9808_create(i2c, 0x40); // Default I2C address
    if (s_mcp9808) {
      mgos_set_timer(5000, true, timer_cb, NULL);
    } else {
      LOG(LL_ERROR, ("Could not initialize sensor"));
    }
  }
  return MGOS_APP_INIT_SUCCESS;
}
```

# Disclaimer

This project is not an official Google project. It is not supported by Google
and Google specifically disclaims all warranties as to its quality,
merchantability, or fitness for a particular purpose.
