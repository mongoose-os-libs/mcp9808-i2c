/*
 * Copyright 2018 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mgos.h"
#include "mgos_mcp9808_internal.h"
#include "mgos_i2c.h"

// Datasheet:
// https://cdn-shop.adafruit.com/datasheets/MCP9808.pdf

// Private functions follow
static bool is_mcp9808(struct mgos_i2c *i2c, uint8_t i2caddr) {
  uint16_t manufacturer_id, device_id;

  manufacturer_id=mgos_i2c_read_reg_w(i2c, i2caddr, MGOS_MCP9808_REG_MANUF_ID);
  device_id=mgos_i2c_read_reg_w(i2c, i2caddr, MGOS_MCP9808_REG_DEVICE_ID);

  return ((manufacturer_id == 0x0054) && (device_id == 0x0400));
}

static void mgos_mcp9808_reset(struct mgos_mcp9808 *sensor) {
  if (!sensor) return;
  mgos_i2c_write_reg_w(sensor->i2c, sensor->i2caddr, MGOS_MCP9808_REG_CONFIG, 0x0);
}
// Private functions end

// Public functions follow
struct mgos_mcp9808 *mgos_mcp9808_create(struct mgos_i2c *i2c, uint8_t i2caddr) {
  struct mgos_mcp9808 *sensor;

  if (!i2c) return NULL;

  if (!is_mcp9808(i2c, i2caddr)) {
    LOG(LL_ERROR, ("Failed to create MCP9808 at I2C 0x%02x", i2caddr));
    return NULL;
  }

  sensor=calloc(1, sizeof(struct mgos_mcp9808));
  if (!sensor) return NULL;
  memset(sensor, 0, sizeof(struct mgos_mcp9808));
  sensor->i2caddr=i2caddr;
  sensor->i2c=i2c;

  mgos_mcp9808_reset(sensor);

  LOG(LL_INFO, ("MCP9808 created at I2C 0x%02x", i2caddr));
  return sensor;
}

void mgos_mcp9808_destroy(struct mgos_mcp9808 **sensor) {
  if (!*sensor) return;
  free (*sensor);
  *sensor=NULL;
  return;
}

bool mgos_mcp9808_read(struct mgos_mcp9808 *sensor) {
  double start = mg_time();

  if (!sensor || !sensor->i2c)
    return false;
  sensor->stats.read++;

  if (start - sensor->stats.last_read_time < MGOS_MCP9808_READ_DELAY) {
    sensor->stats.read_success_cached++;
    return true;
  }
  // Read out sensor data here 
  //
  uint16_t temp_reg = mgos_i2c_read_reg_w(sensor->i2c, sensor->i2caddr, MGOS_MCP9808_REG_AMBIENT_TEMP);

  float temperature = temp_reg & 0x0FFF;
  temperature /= 16.0;
  if (temp_reg & 0x1000) temperature -= 256;
  sensor->temperature=temperature;

  LOG(LL_DEBUG, ("temperature=%.2fC", sensor->temperature));
  sensor->stats.read_success++;
  sensor->stats.read_success_usecs+=1000000*(mg_time()-start);
  sensor->stats.last_read_time=start;
  return true;
}

float mgos_mcp9808_getTemperature(struct mgos_mcp9808 *sensor) {
  if (!mgos_mcp9808_read(sensor)) return NAN;

  return sensor->temperature;
}

void mgos_mcp9808_enable(struct mgos_mcp9808 *sensor, bool enable) {
  uint16_t conf_reg = mgos_i2c_read_reg_w(sensor->i2c, sensor->i2caddr, MGOS_MCP9808_REG_CONFIG);
  if (enable)
    conf_reg &= ~MGOS_MCP9808_REG_CONFIG_SHUTDOWN;
  else
    conf_reg |= MGOS_MCP9808_REG_CONFIG_SHUTDOWN;
  mgos_i2c_write_reg_w(sensor->i2c, sensor->i2caddr, MGOS_MCP9808_REG_CONFIG, conf_reg);
}

bool mgos_mcp9808_getStats(struct mgos_mcp9808 *sensor, struct mgos_mcp9808_stats *stats) {
  if (!sensor || !stats)
    return false;

  memcpy((void *)stats, (const void *)&sensor->stats, sizeof(struct mgos_mcp9808_stats));
  return true;
}

bool mgos_mcp9808_i2c_init(void) {
  return true;
}
// Public functions end
