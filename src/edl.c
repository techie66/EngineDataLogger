/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018-2019 Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * This file was generated by cantools version 39.4.5 Tue Mar  5 18:30:05 2024.
 */

#include <string.h>

#include "edl.h"

static inline uint8_t pack_left_shift_u8(
  uint8_t value,
  uint8_t shift,
  uint8_t mask)
{
  return (uint8_t)((uint8_t)(value << shift) & mask);
}

static inline uint8_t pack_left_shift_u16(
  uint16_t value,
  uint8_t shift,
  uint8_t mask)
{
  return (uint8_t)((uint8_t)(value << shift) & mask);
}

static inline uint8_t pack_left_shift_u32(
  uint32_t value,
  uint8_t shift,
  uint8_t mask)
{
  return (uint8_t)((uint8_t)(value << shift) & mask);
}

static inline uint8_t pack_right_shift_u8(
  uint8_t value,
  uint8_t shift,
  uint8_t mask)
{
  return (uint8_t)((uint8_t)(value >> shift) & mask);
}

static inline uint8_t pack_right_shift_u16(
  uint16_t value,
  uint8_t shift,
  uint8_t mask)
{
  return (uint8_t)((uint8_t)(value >> shift) & mask);
}

static inline uint8_t pack_right_shift_u32(
  uint32_t value,
  uint8_t shift,
  uint8_t mask)
{
  return (uint8_t)((uint8_t)(value >> shift) & mask);
}

static inline uint8_t unpack_left_shift_u8(
  uint8_t value,
  uint8_t shift,
  uint8_t mask)
{
  return (uint8_t)((uint8_t)(value & mask) << shift);
}

static inline uint16_t unpack_left_shift_u16(
  uint8_t value,
  uint8_t shift,
  uint8_t mask)
{
  return (uint16_t)((uint16_t)(value & mask) << shift);
}

static inline uint32_t unpack_left_shift_u32(
  uint8_t value,
  uint8_t shift,
  uint8_t mask)
{
  return (uint32_t)((uint32_t)(value & mask) << shift);
}

static inline uint8_t unpack_right_shift_u8(
  uint8_t value,
  uint8_t shift,
  uint8_t mask)
{
  return (uint8_t)((uint8_t)(value & mask) >> shift);
}

static inline uint16_t unpack_right_shift_u16(
  uint8_t value,
  uint8_t shift,
  uint8_t mask)
{
  return (uint16_t)((uint16_t)(value & mask) >> shift);
}

static inline uint32_t unpack_right_shift_u32(
  uint8_t value,
  uint8_t shift,
  uint8_t mask)
{
  return (uint32_t)((uint32_t)(value & mask) >> shift);
}

int edl_edl_pack(
  uint8_t *dst_p,
  const struct edl_edl_t *src_p,
  size_t size)
{
  if (size < 4u) {
    return (-EINVAL);
  }

  memset(&dst_p[0], 0, 4);

  dst_p[3] |= pack_left_shift_u8(src_p->cmd_a, 0u, 0xffu);

  return (4);
}

int edl_edl_unpack(
  struct edl_edl_t *dst_p,
  const uint8_t *src_p,
  size_t size)
{
  if (size < 4u) {
    return (-EINVAL);
  }

  dst_p->cmd_a = unpack_right_shift_u8(src_p[3], 0u, 0xffu);

  return (0);
}

int edl_edl_init(struct edl_edl_t *msg_p)
{
  if (msg_p == NULL) return -1;

  memset(msg_p, 0, sizeof(struct edl_edl_t));

  return 0;
}

uint8_t edl_edl_cmd_a_encode(double value)
{
  return (uint8_t)(value);
}

double edl_edl_cmd_a_decode(uint8_t value)
{
  return ((double)value);
}

bool edl_edl_cmd_a_is_in_range(uint8_t value)
{
  (void)value;

  return (true);
}

int edl_engine1_pack(
  uint8_t *dst_p,
  const struct edl_engine1_t *src_p,
  size_t size)
{
  if (size < 8u) {
    return (-EINVAL);
  }

  memset(&dst_p[0], 0, 8);

  dst_p[0] |= pack_left_shift_u16(src_p->rpm, 0u, 0xffu);
  dst_p[1] |= pack_right_shift_u16(src_p->rpm, 8u, 0x3fu);
  dst_p[1] |= pack_left_shift_u8(src_p->speed_mph, 6u, 0xc0u);
  dst_p[2] |= pack_right_shift_u8(src_p->speed_mph, 2u, 0x3fu);
  dst_p[2] |= pack_left_shift_u8(src_p->tps, 6u, 0xc0u);
  dst_p[3] |= pack_right_shift_u8(src_p->tps, 2u, 0x1fu);
  dst_p[3] |= pack_left_shift_u16(src_p->oil_temp, 5u, 0xe0u);
  dst_p[4] |= pack_right_shift_u16(src_p->oil_temp, 3u, 0x3fu);
  dst_p[4] |= pack_left_shift_u8(src_p->oil_pres, 6u, 0xc0u);
  dst_p[5] |= pack_right_shift_u8(src_p->oil_pres, 2u, 0x3fu);
  dst_p[5] |= pack_left_shift_u8(src_p->iap, 6u, 0xc0u);
  dst_p[6] |= pack_right_shift_u8(src_p->iap, 2u, 0x3fu);
  dst_p[6] |= pack_left_shift_u8(src_p->voltage, 6u, 0xc0u);
  dst_p[7] |= pack_right_shift_u8(src_p->voltage, 2u, 0x3fu);

  return (8);
}

int edl_engine1_unpack(
  struct edl_engine1_t *dst_p,
  const uint8_t *src_p,
  size_t size)
{
  if (size < 8u) {
    return (-EINVAL);
  }

  dst_p->rpm = unpack_right_shift_u16(src_p[0], 0u, 0xffu);
  dst_p->rpm |= unpack_left_shift_u16(src_p[1], 8u, 0x3fu);
  dst_p->speed_mph = unpack_right_shift_u8(src_p[1], 6u, 0xc0u);
  dst_p->speed_mph |= unpack_left_shift_u8(src_p[2], 2u, 0x3fu);
  dst_p->tps = unpack_right_shift_u8(src_p[2], 6u, 0xc0u);
  dst_p->tps |= unpack_left_shift_u8(src_p[3], 2u, 0x1fu);
  dst_p->oil_temp = unpack_right_shift_u16(src_p[3], 5u, 0xe0u);
  dst_p->oil_temp |= unpack_left_shift_u16(src_p[4], 3u, 0x3fu);
  dst_p->oil_pres = unpack_right_shift_u8(src_p[4], 6u, 0xc0u);
  dst_p->oil_pres |= unpack_left_shift_u8(src_p[5], 2u, 0x3fu);
  dst_p->iap = unpack_right_shift_u8(src_p[5], 6u, 0xc0u);
  dst_p->iap |= unpack_left_shift_u8(src_p[6], 2u, 0x3fu);
  dst_p->voltage = unpack_right_shift_u8(src_p[6], 6u, 0xc0u);
  dst_p->voltage |= unpack_left_shift_u8(src_p[7], 2u, 0x3fu);

  return (0);
}

int edl_engine1_init(struct edl_engine1_t *msg_p)
{
  if (msg_p == NULL) return -1;

  memset(msg_p, 0, sizeof(struct edl_engine1_t));

  return 0;
}

uint16_t edl_engine1_rpm_encode(double value)
{
  return (uint16_t)(value);
}

double edl_engine1_rpm_decode(uint16_t value)
{
  return ((double)value);
}

bool edl_engine1_rpm_is_in_range(uint16_t value)
{
  (void)value;

  return (true);
}

uint8_t edl_engine1_speed_mph_encode(double value)
{
  return (uint8_t)(value);
}

double edl_engine1_speed_mph_decode(uint8_t value)
{
  return ((double)value);
}

bool edl_engine1_speed_mph_is_in_range(uint8_t value)
{
  (void)value;

  return (true);
}

uint8_t edl_engine1_tps_encode(double value)
{
  return (uint8_t)(value);
}

double edl_engine1_tps_decode(uint8_t value)
{
  return ((double)value);
}

bool edl_engine1_tps_is_in_range(uint8_t value)
{
  return (value <= 100u);
}

uint16_t edl_engine1_oil_temp_encode(double value)
{
  return (uint16_t)(value - -50.0);
}

double edl_engine1_oil_temp_decode(uint16_t value)
{
  return ((double)value + -50.0);
}

bool edl_engine1_oil_temp_is_in_range(uint16_t value)
{
  return (value <= 500u);
}

uint8_t edl_engine1_oil_pres_encode(double value)
{
  return (uint8_t)(value);
}

double edl_engine1_oil_pres_decode(uint8_t value)
{
  return ((double)value);
}

bool edl_engine1_oil_pres_is_in_range(uint8_t value)
{
  (void)value;

  return (true);
}

uint8_t edl_engine1_iap_encode(double value)
{
  return (uint8_t)(value);
}

double edl_engine1_iap_decode(uint8_t value)
{
  return ((double)value);
}

bool edl_engine1_iap_is_in_range(uint8_t value)
{
  (void)value;

  return (true);
}

uint8_t edl_engine1_voltage_encode(double value)
{
  return (uint8_t)(value / 0.1);
}

double edl_engine1_voltage_decode(uint8_t value)
{
  return ((double)value * 0.1);
}

bool edl_engine1_voltage_is_in_range(uint8_t value)
{
  (void)value;

  return (true);
}

int edl_engine2_pack(
  uint8_t *dst_p,
  const struct edl_engine2_t *src_p,
  size_t size)
{
  if (size < 8u) {
    return (-EINVAL);
  }

  memset(&dst_p[0], 0, 8);

  dst_p[0] |= pack_left_shift_u32(src_p->odo, 0u, 0xffu);
  dst_p[1] |= pack_right_shift_u32(src_p->odo, 8u, 0xffu);
  dst_p[2] |= pack_right_shift_u32(src_p->odo, 16u, 0xffu);
  dst_p[3] |= pack_left_shift_u32(src_p->trip, 0u, 0xffu);
  dst_p[4] |= pack_right_shift_u32(src_p->trip, 8u, 0xffu);
  dst_p[5] |= pack_right_shift_u32(src_p->trip, 16u, 0x0fu);
  dst_p[5] |= pack_left_shift_u16(src_p->lambda, 4u, 0xf0u);
  dst_p[6] |= pack_right_shift_u16(src_p->lambda, 4u, 0x7fu);
  dst_p[7] |= pack_left_shift_u8(src_p->gear, 0u, 0xffu);

  return (8);
}

int edl_engine2_unpack(
  struct edl_engine2_t *dst_p,
  const uint8_t *src_p,
  size_t size)
{
  if (size < 8u) {
    return (-EINVAL);
  }

  dst_p->odo = unpack_right_shift_u32(src_p[0], 0u, 0xffu);
  dst_p->odo |= unpack_left_shift_u32(src_p[1], 8u, 0xffu);
  dst_p->odo |= unpack_left_shift_u32(src_p[2], 16u, 0xffu);
  dst_p->trip = unpack_right_shift_u32(src_p[3], 0u, 0xffu);
  dst_p->trip |= unpack_left_shift_u32(src_p[4], 8u, 0xffu);
  dst_p->trip |= unpack_left_shift_u32(src_p[5], 16u, 0x0fu);
  dst_p->lambda = unpack_right_shift_u16(src_p[5], 4u, 0xf0u);
  dst_p->lambda |= unpack_left_shift_u16(src_p[6], 4u, 0x7fu);
  dst_p->gear = unpack_right_shift_u8(src_p[7], 0u, 0xffu);

  return (0);
}

int edl_engine2_init(struct edl_engine2_t *msg_p)
{
  if (msg_p == NULL) return -1;

  memset(msg_p, 0, sizeof(struct edl_engine2_t));

  return 0;
}

uint32_t edl_engine2_odo_encode(double value)
{
  return (uint32_t)(value / 0.1);
}

double edl_engine2_odo_decode(uint32_t value)
{
  return ((double)value * 0.1);
}

bool edl_engine2_odo_is_in_range(uint32_t value)
{
  return (value <= 16777210u);
}

uint32_t edl_engine2_trip_encode(double value)
{
  return (uint32_t)(value / 0.1);
}

double edl_engine2_trip_decode(uint32_t value)
{
  return ((double)value * 0.1);
}

bool edl_engine2_trip_is_in_range(uint32_t value)
{
  return (value <= 1048570u);
}

uint16_t edl_engine2_lambda_encode(double value)
{
  return (uint16_t)(value / 0.001);
}

double edl_engine2_lambda_decode(uint16_t value)
{
  return ((double)value * 0.001);
}

bool edl_engine2_lambda_is_in_range(uint16_t value)
{
  return (value <= 2048u);
}

uint8_t edl_engine2_gear_encode(double value)
{
  return (uint8_t)(value);
}

double edl_engine2_gear_decode(uint8_t value)
{
  return ((double)value);
}

bool edl_engine2_gear_is_in_range(uint8_t value)
{
  (void)value;

  return (true);
}

int edl_engine3_pack(
  uint8_t *dst_p,
  const struct edl_engine3_t *src_p,
  size_t size)
{
  if (size < 8u) {
    return (-EINVAL);
  }

  memset(&dst_p[0], 0, 8);

  dst_p[0] |= pack_left_shift_u8(src_p->adv, 0u, 0xffu);
  dst_p[1] |= pack_left_shift_u16(src_p->roll_angle, 0u, 0xffu);
  dst_p[2] |= pack_right_shift_u16(src_p->roll_angle, 8u, 0x0fu);
  dst_p[2] |= pack_left_shift_u16(src_p->pitch_angle, 4u, 0xf0u);
  dst_p[3] |= pack_right_shift_u16(src_p->pitch_angle, 4u, 0xffu);
  dst_p[4] |= pack_left_shift_u16(src_p->yaw_angle, 0u, 0xffu);
  dst_p[5] |= pack_right_shift_u16(src_p->yaw_angle, 8u, 0x1fu);

  return (8);
}

int edl_engine3_unpack(
  struct edl_engine3_t *dst_p,
  const uint8_t *src_p,
  size_t size)
{
  if (size < 8u) {
    return (-EINVAL);
  }

  dst_p->adv = unpack_right_shift_u8(src_p[0], 0u, 0xffu);
  dst_p->roll_angle = unpack_right_shift_u16(src_p[1], 0u, 0xffu);
  dst_p->roll_angle |= unpack_left_shift_u16(src_p[2], 8u, 0x0fu);
  dst_p->pitch_angle = unpack_right_shift_u16(src_p[2], 4u, 0xf0u);
  dst_p->pitch_angle |= unpack_left_shift_u16(src_p[3], 4u, 0xffu);
  dst_p->yaw_angle = unpack_right_shift_u16(src_p[4], 0u, 0xffu);
  dst_p->yaw_angle |= unpack_left_shift_u16(src_p[5], 8u, 0x1fu);

  return (0);
}

int edl_engine3_init(struct edl_engine3_t *msg_p)
{
  if (msg_p == NULL) return -1;

  memset(msg_p, 0, sizeof(struct edl_engine3_t));

  return 0;
}

uint8_t edl_engine3_adv_encode(double value)
{
  return (uint8_t)(value);
}

double edl_engine3_adv_decode(uint8_t value)
{
  return ((double)value);
}

bool edl_engine3_adv_is_in_range(uint8_t value)
{
  (void)value;

  return (true);
}

uint16_t edl_engine3_roll_angle_encode(double value)
{
  return (uint16_t)((value - -180.0) / 0.1);
}

double edl_engine3_roll_angle_decode(uint16_t value)
{
  return (((double)value * 0.1) + -180.0);
}

bool edl_engine3_roll_angle_is_in_range(uint16_t value)
{
  return (value <= 3600u);
}

uint16_t edl_engine3_pitch_angle_encode(double value)
{
  return (uint16_t)((value - -180.0) / 0.1);
}

double edl_engine3_pitch_angle_decode(uint16_t value)
{
  return (((double)value * 0.1) + -180.0);
}

bool edl_engine3_pitch_angle_is_in_range(uint16_t value)
{
  return (value <= 3600u);
}

uint16_t edl_engine3_yaw_angle_encode(double value)
{
  return (uint16_t)((value - -360.0) / 0.1);
}

double edl_engine3_yaw_angle_decode(uint16_t value)
{
  return (((double)value * 0.1) + -360.0);
}

bool edl_engine3_yaw_angle_is_in_range(uint16_t value)
{
  return (value <= 7200u);
}