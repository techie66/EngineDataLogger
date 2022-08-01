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
 * This file was generated by cantools version 37.0.7 Fri Jul 15 19:55:12 2022.
 */

#ifndef IMU_CAN_H
#define IMU_CAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifndef EINVAL
#    define EINVAL 22
#endif

/* Frame ids. */
#define IMU_CAN_BODY_POSITION_FRAME_ID (0x1cecff80u)
#define IMU_CAN_BODY_ACCEL_FRAME_ID (0x1cecff81u)

/* Frame lengths in bytes. */
#define IMU_CAN_BODY_POSITION_LENGTH (5u)
#define IMU_CAN_BODY_ACCEL_LENGTH (6u)

/* Extended or standard frame types. */
#define IMU_CAN_BODY_POSITION_IS_EXTENDED (1)
#define IMU_CAN_BODY_ACCEL_IS_EXTENDED (1)

/* Frame cycle times in milliseconds. */


/* Signal choices. */


/**
 * Signals in message BODY_POSITION.
 *
 * All signal values are as on the CAN bus.
 */
struct imu_can_body_position_t {
  /**
   * Range: 0..3600 (-180..180 deg)
   * Scale: 0.1
   * Offset: -180
   */
  uint16_t roll_angle;

  /**
   * Range: 0..3600 (-180..180 deg)
   * Scale: 0.1
   * Offset: -180
   */
  uint16_t pitch_angle;

  /**
   * Range: 0..7200 (-360..360 deg)
   * Scale: 0.1
   * Offset: -360
   */
  uint16_t yaw_angle;
};

/**
 * Signals in message BODY_ACCEL.
 *
 * All signal values are as on the CAN bus.
 */
struct imu_can_body_accel_t {
  /**
   * Range: 0..40000 (-20..20 m/s2)
   * Scale: 0.001
   * Offset: -20
   */
  uint16_t acc_x;

  /**
   * Range: 0..40000 (-20..20 m/s2)
   * Scale: 0.001
   * Offset: -20
   */
  uint16_t acc_y;

  /**
   * Range: 0..40000 (-20..20 m/s2)
   * Scale: 0.001
   * Offset: -20
   */
  uint16_t acc_z;
};

/**
 * Pack message BODY_POSITION.
 *
 * @param[out] dst_p Buffer to pack the message into.
 * @param[in] src_p Data to pack.
 * @param[in] size Size of dst_p.
 *
 * @return Size of packed data, or negative error code.
 */
int imu_can_body_position_pack(
  uint8_t *dst_p,
  const struct imu_can_body_position_t *src_p,
  size_t size);

/**
 * Unpack message BODY_POSITION.
 *
 * @param[out] dst_p Object to unpack the message into.
 * @param[in] src_p Message to unpack.
 * @param[in] size Size of src_p.
 *
 * @return zero(0) or negative error code.
 */
int imu_can_body_position_unpack(
  struct imu_can_body_position_t *dst_p,
  const uint8_t *src_p,
  size_t size);

/**
 * Encode given signal by applying scaling and offset.
 *
 * @param[in] value Signal to encode.
 *
 * @return Encoded signal.
 */
uint16_t imu_can_body_position_roll_angle_encode(double value);

/**
 * Decode given signal by applying scaling and offset.
 *
 * @param[in] value Signal to decode.
 *
 * @return Decoded signal.
 */
double imu_can_body_position_roll_angle_decode(uint16_t value);

/**
 * Check that given signal is in allowed range.
 *
 * @param[in] value Signal to check.
 *
 * @return true if in range, false otherwise.
 */
bool imu_can_body_position_roll_angle_is_in_range(uint16_t value);

/**
 * Encode given signal by applying scaling and offset.
 *
 * @param[in] value Signal to encode.
 *
 * @return Encoded signal.
 */
uint16_t imu_can_body_position_pitch_angle_encode(double value);

/**
 * Decode given signal by applying scaling and offset.
 *
 * @param[in] value Signal to decode.
 *
 * @return Decoded signal.
 */
double imu_can_body_position_pitch_angle_decode(uint16_t value);

/**
 * Check that given signal is in allowed range.
 *
 * @param[in] value Signal to check.
 *
 * @return true if in range, false otherwise.
 */
bool imu_can_body_position_pitch_angle_is_in_range(uint16_t value);

/**
 * Encode given signal by applying scaling and offset.
 *
 * @param[in] value Signal to encode.
 *
 * @return Encoded signal.
 */
uint16_t imu_can_body_position_yaw_angle_encode(double value);

/**
 * Decode given signal by applying scaling and offset.
 *
 * @param[in] value Signal to decode.
 *
 * @return Decoded signal.
 */
double imu_can_body_position_yaw_angle_decode(uint16_t value);

/**
 * Check that given signal is in allowed range.
 *
 * @param[in] value Signal to check.
 *
 * @return true if in range, false otherwise.
 */
bool imu_can_body_position_yaw_angle_is_in_range(uint16_t value);

/**
 * Pack message BODY_ACCEL.
 *
 * @param[out] dst_p Buffer to pack the message into.
 * @param[in] src_p Data to pack.
 * @param[in] size Size of dst_p.
 *
 * @return Size of packed data, or negative error code.
 */
int imu_can_body_accel_pack(
  uint8_t *dst_p,
  const struct imu_can_body_accel_t *src_p,
  size_t size);

/**
 * Unpack message BODY_ACCEL.
 *
 * @param[out] dst_p Object to unpack the message into.
 * @param[in] src_p Message to unpack.
 * @param[in] size Size of src_p.
 *
 * @return zero(0) or negative error code.
 */
int imu_can_body_accel_unpack(
  struct imu_can_body_accel_t *dst_p,
  const uint8_t *src_p,
  size_t size);

/**
 * Encode given signal by applying scaling and offset.
 *
 * @param[in] value Signal to encode.
 *
 * @return Encoded signal.
 */
uint16_t imu_can_body_accel_acc_x_encode(double value);

/**
 * Decode given signal by applying scaling and offset.
 *
 * @param[in] value Signal to decode.
 *
 * @return Decoded signal.
 */
double imu_can_body_accel_acc_x_decode(uint16_t value);

/**
 * Check that given signal is in allowed range.
 *
 * @param[in] value Signal to check.
 *
 * @return true if in range, false otherwise.
 */
bool imu_can_body_accel_acc_x_is_in_range(uint16_t value);

/**
 * Encode given signal by applying scaling and offset.
 *
 * @param[in] value Signal to encode.
 *
 * @return Encoded signal.
 */
uint16_t imu_can_body_accel_acc_y_encode(double value);

/**
 * Decode given signal by applying scaling and offset.
 *
 * @param[in] value Signal to decode.
 *
 * @return Decoded signal.
 */
double imu_can_body_accel_acc_y_decode(uint16_t value);

/**
 * Check that given signal is in allowed range.
 *
 * @param[in] value Signal to check.
 *
 * @return true if in range, false otherwise.
 */
bool imu_can_body_accel_acc_y_is_in_range(uint16_t value);

/**
 * Encode given signal by applying scaling and offset.
 *
 * @param[in] value Signal to encode.
 *
 * @return Encoded signal.
 */
uint16_t imu_can_body_accel_acc_z_encode(double value);

/**
 * Decode given signal by applying scaling and offset.
 *
 * @param[in] value Signal to decode.
 *
 * @return Decoded signal.
 */
double imu_can_body_accel_acc_z_decode(uint16_t value);

/**
 * Check that given signal is in allowed range.
 *
 * @param[in] value Signal to check.
 *
 * @return true if in range, false otherwise.
 */
bool imu_can_body_accel_acc_z_is_in_range(uint16_t value);


#ifdef __cplusplus
}
#endif

#endif
