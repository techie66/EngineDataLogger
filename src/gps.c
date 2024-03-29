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
 * This file was generated by cantools version 37.0.7 Sun Jun  5 19:13:16 2022.
 */

#include <string.h>

#include "gps.h"

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

int gps_gps_time_pack(
  uint8_t *dst_p,
  const struct gps_gps_time_t *src_p,
  size_t size)
{
  if (size < 8u) {
    return (-EINVAL);
  }

  memset(&dst_p[0], 0, 8);

  dst_p[0] |= pack_left_shift_u8(src_p->day, 0u, 0xffu);
  dst_p[1] |= pack_left_shift_u8(src_p->month, 0u, 0xffu);
  dst_p[2] |= pack_left_shift_u16(src_p->year, 0u, 0xffu);
  dst_p[3] |= pack_right_shift_u16(src_p->year, 8u, 0xffu);
  dst_p[4] |= pack_left_shift_u8(src_p->hour, 0u, 0xffu);
  dst_p[5] |= pack_left_shift_u8(src_p->minute, 0u, 0xffu);
  dst_p[6] |= pack_left_shift_u8(src_p->second, 0u, 0xffu);

  return (8);
}

int gps_gps_time_unpack(
  struct gps_gps_time_t *dst_p,
  const uint8_t *src_p,
  size_t size)
{
  if (size < 8u) {
    return (-EINVAL);
  }

  dst_p->day = unpack_right_shift_u8(src_p[0], 0u, 0xffu);
  dst_p->month = unpack_right_shift_u8(src_p[1], 0u, 0xffu);
  dst_p->year = unpack_right_shift_u16(src_p[2], 0u, 0xffu);
  dst_p->year |= unpack_left_shift_u16(src_p[3], 8u, 0xffu);
  dst_p->hour = unpack_right_shift_u8(src_p[4], 0u, 0xffu);
  dst_p->minute = unpack_right_shift_u8(src_p[5], 0u, 0xffu);
  dst_p->second = unpack_right_shift_u8(src_p[6], 0u, 0xffu);

  return (0);
}

uint8_t gps_gps_time_day_encode(double value)
{
  return (uint8_t)(value);
}

double gps_gps_time_day_decode(uint8_t value)
{
  return ((double)value);
}

bool gps_gps_time_day_is_in_range(uint8_t value)
{
  return ((value >= 1u) && (value <= 31u));
}

uint8_t gps_gps_time_month_encode(double value)
{
  return (uint8_t)(value);
}

double gps_gps_time_month_decode(uint8_t value)
{
  return ((double)value);
}

bool gps_gps_time_month_is_in_range(uint8_t value)
{
  return ((value >= 1u) && (value <= 12u));
}

uint16_t gps_gps_time_year_encode(double value)
{
  return (uint16_t)(value);
}

double gps_gps_time_year_decode(uint16_t value)
{
  return ((double)value);
}

bool gps_gps_time_year_is_in_range(uint16_t value)
{
  return (value >= 1u);
}

uint8_t gps_gps_time_hour_encode(double value)
{
  return (uint8_t)(value);
}

double gps_gps_time_hour_decode(uint8_t value)
{
  return ((double)value);
}

bool gps_gps_time_hour_is_in_range(uint8_t value)
{
  return (value <= 23u);
}

uint8_t gps_gps_time_minute_encode(double value)
{
  return (uint8_t)(value);
}

double gps_gps_time_minute_decode(uint8_t value)
{
  return ((double)value);
}

bool gps_gps_time_minute_is_in_range(uint8_t value)
{
  return (value <= 59u);
}

uint8_t gps_gps_time_second_encode(double value)
{
  return (uint8_t)(value);
}

double gps_gps_time_second_decode(uint8_t value)
{
  return ((double)value);
}

bool gps_gps_time_second_is_in_range(uint8_t value)
{
  return (value <= 59u);
}

int gps_gps_loc_pack(
  uint8_t *dst_p,
  const struct gps_gps_loc_t *src_p,
  size_t size)
{
  if (size < 8u) {
    return (-EINVAL);
  }

  memset(&dst_p[0], 0, 8);

  dst_p[0] |= pack_left_shift_u32(src_p->lat_decimal_degrees, 0u, 0xffu);
  dst_p[1] |= pack_right_shift_u32(src_p->lat_decimal_degrees, 8u, 0xffu);
  dst_p[2] |= pack_right_shift_u32(src_p->lat_decimal_degrees, 16u, 0xffu);
  dst_p[3] |= pack_right_shift_u32(src_p->lat_decimal_degrees, 24u, 0xffu);
  dst_p[4] |= pack_left_shift_u32(src_p->long_decimal_degrees, 0u, 0xffu);
  dst_p[5] |= pack_right_shift_u32(src_p->long_decimal_degrees, 8u, 0xffu);
  dst_p[6] |= pack_right_shift_u32(src_p->long_decimal_degrees, 16u, 0xffu);
  dst_p[7] |= pack_right_shift_u32(src_p->long_decimal_degrees, 24u, 0xffu);

  return (8);
}

int gps_gps_loc_unpack(
  struct gps_gps_loc_t *dst_p,
  const uint8_t *src_p,
  size_t size)
{
  if (size < 8u) {
    return (-EINVAL);
  }

  dst_p->lat_decimal_degrees = unpack_right_shift_u32(src_p[0], 0u, 0xffu);
  dst_p->lat_decimal_degrees |= unpack_left_shift_u32(src_p[1], 8u, 0xffu);
  dst_p->lat_decimal_degrees |= unpack_left_shift_u32(src_p[2], 16u, 0xffu);
  dst_p->lat_decimal_degrees |= unpack_left_shift_u32(src_p[3], 24u, 0xffu);
  dst_p->long_decimal_degrees = unpack_right_shift_u32(src_p[4], 0u, 0xffu);
  dst_p->long_decimal_degrees |= unpack_left_shift_u32(src_p[5], 8u, 0xffu);
  dst_p->long_decimal_degrees |= unpack_left_shift_u32(src_p[6], 16u, 0xffu);
  dst_p->long_decimal_degrees |= unpack_left_shift_u32(src_p[7], 24u, 0xffu);

  return (0);
}

uint32_t gps_gps_loc_lat_decimal_degrees_encode(double value)
{
  return (uint32_t)((value - -90.0) / 0.000001);
}

double gps_gps_loc_lat_decimal_degrees_decode(uint32_t value)
{
  return (((double)value * 0.000001) + -90.0);
}

bool gps_gps_loc_lat_decimal_degrees_is_in_range(uint32_t value)
{
  return (value <= 180000000u);
}

uint32_t gps_gps_loc_long_decimal_degrees_encode(double value)
{
  return (uint32_t)((value - -180.0) / 0.000001);
}

double gps_gps_loc_long_decimal_degrees_decode(uint32_t value)
{
  return (((double)value * 0.000001) + -180.0);
}

bool gps_gps_loc_long_decimal_degrees_is_in_range(uint32_t value)
{
  return (value <= 360000000u);
}

int gps_gps_nav_pack(
  uint8_t *dst_p,
  const struct gps_gps_nav_t *src_p,
  size_t size)
{
  if (size < 8u) {
    return (-EINVAL);
  }

  memset(&dst_p[0], 0, 8);

  dst_p[0] |= pack_left_shift_u16(src_p->speed, 0u, 0xffu);
  dst_p[1] |= pack_right_shift_u16(src_p->speed, 8u, 0xffu);
  dst_p[2] |= pack_left_shift_u16(src_p->heading, 0u, 0xffu);
  dst_p[3] |= pack_right_shift_u16(src_p->heading, 8u, 0xffu);
  dst_p[4] |= pack_left_shift_u16(src_p->altitude, 0u, 0xffu);
  dst_p[5] |= pack_right_shift_u16(src_p->altitude, 8u, 0xffu);

  return (8);
}

int gps_gps_nav_unpack(
  struct gps_gps_nav_t *dst_p,
  const uint8_t *src_p,
  size_t size)
{
  if (size < 8u) {
    return (-EINVAL);
  }

  dst_p->speed = unpack_right_shift_u16(src_p[0], 0u, 0xffu);
  dst_p->speed |= unpack_left_shift_u16(src_p[1], 8u, 0xffu);
  dst_p->heading = unpack_right_shift_u16(src_p[2], 0u, 0xffu);
  dst_p->heading |= unpack_left_shift_u16(src_p[3], 8u, 0xffu);
  dst_p->altitude = unpack_right_shift_u16(src_p[4], 0u, 0xffu);
  dst_p->altitude |= unpack_left_shift_u16(src_p[5], 8u, 0xffu);

  return (0);
}

uint16_t gps_gps_nav_speed_encode(double value)
{
  return (uint16_t)(value / 0.1);
}

double gps_gps_nav_speed_decode(uint16_t value)
{
  return ((double)value * 0.1);
}

bool gps_gps_nav_speed_is_in_range(uint16_t value)
{
  (void)value;

  return (true);
}

uint16_t gps_gps_nav_heading_encode(double value)
{
  return (uint16_t)(value / 0.1);
}

double gps_gps_nav_heading_decode(uint16_t value)
{
  return ((double)value * 0.1);
}

bool gps_gps_nav_heading_is_in_range(uint16_t value)
{
  return (value <= 35990u);
}

uint16_t gps_gps_nav_altitude_encode(double value)
{
  return (uint16_t)(value);
}

double gps_gps_nav_altitude_decode(uint16_t value)
{
  return ((double)value);
}

bool gps_gps_nav_altitude_is_in_range(uint16_t value)
{
  (void)value;

  return (true);
}

int gps_gps_stat_pack(
  uint8_t *dst_p,
  const struct gps_gps_stat_t *src_p,
  size_t size)
{
  if (size < 8u) {
    return (-EINVAL);
  }

  memset(&dst_p[0], 0, 8);

  dst_p[0] |= pack_left_shift_u8(src_p->active_satellites, 0u, 0x3fu);
  dst_p[0] |= pack_left_shift_u8(src_p->type, 6u, 0xc0u);
  dst_p[1] |= pack_right_shift_u8(src_p->type, 2u, 0x03u);
  dst_p[1] |= pack_left_shift_u8(src_p->visible_satellites, 2u, 0xfcu);
  dst_p[2] |= pack_left_shift_u16(src_p->pdop, 0u, 0xffu);
  dst_p[3] |= pack_right_shift_u16(src_p->pdop, 8u, 0xffu);
  dst_p[4] |= pack_left_shift_u16(src_p->hdop, 0u, 0xffu);
  dst_p[5] |= pack_right_shift_u16(src_p->hdop, 8u, 0xffu);
  dst_p[6] |= pack_left_shift_u16(src_p->vdop, 0u, 0xffu);
  dst_p[7] |= pack_right_shift_u16(src_p->vdop, 8u, 0xffu);

  return (8);
}

int gps_gps_stat_unpack(
  struct gps_gps_stat_t *dst_p,
  const uint8_t *src_p,
  size_t size)
{
  if (size < 8u) {
    return (-EINVAL);
  }

  dst_p->active_satellites = unpack_right_shift_u8(src_p[0], 0u, 0x3fu);
  dst_p->type = unpack_right_shift_u8(src_p[0], 6u, 0xc0u);
  dst_p->type |= unpack_left_shift_u8(src_p[1], 2u, 0x03u);
  dst_p->visible_satellites = unpack_right_shift_u8(src_p[1], 2u, 0xfcu);
  dst_p->pdop = unpack_right_shift_u16(src_p[2], 0u, 0xffu);
  dst_p->pdop |= unpack_left_shift_u16(src_p[3], 8u, 0xffu);
  dst_p->hdop = unpack_right_shift_u16(src_p[4], 0u, 0xffu);
  dst_p->hdop |= unpack_left_shift_u16(src_p[5], 8u, 0xffu);
  dst_p->vdop = unpack_right_shift_u16(src_p[6], 0u, 0xffu);
  dst_p->vdop |= unpack_left_shift_u16(src_p[7], 8u, 0xffu);

  return (0);
}

uint8_t gps_gps_stat_active_satellites_encode(double value)
{
  return (uint8_t)(value);
}

double gps_gps_stat_active_satellites_decode(uint8_t value)
{
  return ((double)value);
}

bool gps_gps_stat_active_satellites_is_in_range(uint8_t value)
{
  return (value <= 63u);
}

uint8_t gps_gps_stat_type_encode(double value)
{
  return (uint8_t)(value);
}

double gps_gps_stat_type_decode(uint8_t value)
{
  return ((double)value);
}

bool gps_gps_stat_type_is_in_range(uint8_t value)
{
  return (value <= 15u);
}

uint8_t gps_gps_stat_visible_satellites_encode(double value)
{
  return (uint8_t)(value);
}

double gps_gps_stat_visible_satellites_decode(uint8_t value)
{
  return ((double)value);
}

bool gps_gps_stat_visible_satellites_is_in_range(uint8_t value)
{
  return (value <= 63u);
}

uint16_t gps_gps_stat_pdop_encode(double value)
{
  return (uint16_t)(value / 0.1);
}

double gps_gps_stat_pdop_decode(uint16_t value)
{
  return ((double)value * 0.1);
}

bool gps_gps_stat_pdop_is_in_range(uint16_t value)
{
  (void)value;

  return (true);
}

uint16_t gps_gps_stat_hdop_encode(double value)
{
  return (uint16_t)(value / 0.1);
}

double gps_gps_stat_hdop_decode(uint16_t value)
{
  return ((double)value * 0.1);
}

bool gps_gps_stat_hdop_is_in_range(uint16_t value)
{
  (void)value;

  return (true);
}

uint16_t gps_gps_stat_vdop_encode(double value)
{
  return (uint16_t)(value / 0.1);
}

double gps_gps_stat_vdop_decode(uint16_t value)
{
  return ((double)value * 0.1);
}

bool gps_gps_stat_vdop_is_in_range(uint16_t value)
{
  (void)value;

  return (true);
}
