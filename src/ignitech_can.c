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
 * This file was generated by cantools version 37.0.7 Sun Jun  5 21:40:10 2022.
 */

#include <string.h>

#include "ignitech_can.h"

static inline uint8_t pack_left_shift_u16(
  uint16_t value,
  uint8_t shift,
  uint8_t mask)
{
  return (uint8_t)((uint8_t)(value << shift) & mask);
}

static inline uint8_t pack_right_shift_u16(
  uint16_t value,
  uint8_t shift,
  uint8_t mask)
{
  return (uint8_t)((uint8_t)(value >> shift) & mask);
}

static inline uint16_t unpack_left_shift_u16(
  uint8_t value,
  uint8_t shift,
  uint8_t mask)
{
  return (uint16_t)((uint16_t)(value & mask) << shift);
}

static inline uint16_t unpack_right_shift_u16(
  uint8_t value,
  uint8_t shift,
  uint8_t mask)
{
  return (uint16_t)((uint16_t)(value & mask) >> shift);
}

int ignitech_can_ignitech_wb_2_pack(
  uint8_t *dst_p,
  const struct ignitech_can_ignitech_wb_2_t *src_p,
  size_t size)
{
  if (size < 8u) {
    return (-EINVAL);
  }

  memset(&dst_p[0], 0, 8);

  dst_p[2] |= pack_left_shift_u16(src_p->lambda, 0u, 0xffu);
  dst_p[3] |= pack_right_shift_u16(src_p->lambda, 8u, 0xffu);

  return (8);
}

int ignitech_can_ignitech_wb_2_unpack(
  struct ignitech_can_ignitech_wb_2_t *dst_p,
  const uint8_t *src_p,
  size_t size)
{
  if (size < 8u) {
    return (-EINVAL);
  }

  dst_p->lambda = unpack_right_shift_u16(src_p[2], 0u, 0xffu);
  dst_p->lambda |= unpack_left_shift_u16(src_p[3], 8u, 0xffu);

  return (0);
}

uint16_t ignitech_can_ignitech_wb_2_lambda_encode(double value)
{
  return (uint16_t)(value / 0.01);
}

double ignitech_can_ignitech_wb_2_lambda_decode(uint16_t value)
{
  return ((double)value * 0.01);
}

bool ignitech_can_ignitech_wb_2_lambda_is_in_range(uint16_t value)
{
  return (value <= 10000u);
}
