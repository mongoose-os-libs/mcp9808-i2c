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

#pragma once

#include "mgos.h"
#include "mgos_i2c.h"
#include "mgos_mcp9808.h"
#include <math.h>

#define MGOS_MCP9808_DEFAULT_I2CADDR       (0x18)

#define MGOS_MCP9808_REG_CONFIG            (0x01)
#define MGOS_MCP9808_REG_CONFIG_SHUTDOWN   (0x0100)
#define MGOS_MCP9808_REG_CONFIG_CRITLOCKED (0x0080)
#define MGOS_MCP9808_REG_CONFIG_WINLOCKED  (0x0040)
#define MGOS_MCP9808_REG_CONFIG_INTCLR     (0x0020)
#define MGOS_MCP9808_REG_CONFIG_ALERTSTAT  (0x0010)
#define MGOS_MCP9808_REG_CONFIG_ALERTCTRL  (0x0008)
#define MGOS_MCP9808_REG_CONFIG_ALERTSEL   (0x0004)
#define MGOS_MCP9808_REG_CONFIG_ALERTPOL   (0x0002)
#define MGOS_MCP9808_REG_CONFIG_ALERTMODE  (0x0001)

#define MGOS_MCP9808_REG_UPPER_TEMP        (0x02)
#define MGOS_MCP9808_REG_LOWER_TEMP        (0x03)
#define MGOS_MCP9808_REG_CRIT_TEMP         (0x04)
#define MGOS_MCP9808_REG_AMBIENT_TEMP      (0x05)
#define MGOS_MCP9808_REG_MANUF_ID          (0x06)
#define MGOS_MCP9808_REG_DEVICE_ID         (0x07)

#ifdef __cplusplus
extern "C" {
#endif

struct mgos_mcp9808 {
  struct mgos_i2c *i2c;
  uint8_t i2caddr;
  struct mgos_mcp9808_stats stats;

  float temperature;
};

#ifdef __cplusplus
}
#endif
