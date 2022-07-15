/*
    EngineDataLogger
    Copyright (C) 2018-2021  Jacob Geigle

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include "can.h"

bool can_sock_connect( int can_s, char const *can_arg )
{
  struct sockaddr_can addr;
  struct ifreq ifr;
  strcpy(ifr.ifr_name, can_arg);
  if ( ioctl(can_s, SIOCGIFINDEX, &ifr) < 0 ) {
    error_message(WARN, "Warning:CAN: ioctl failed : %s, Retrying...", strerror(errno));
    return false;
  }

  else {
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if ( bind(can_s, (struct sockaddr *)&addr, sizeof(addr)) < 0 ) {
      error_message(WARN, "Warning:CAN: bind failed : %s, Retrying...", strerror(errno));
      return false;
    }

    else {
      return true;
    }
  }
}

void can_parse(const can_frame &frame, bike_data &log_data, const int can_s)
{
  int _status = EXIT_FAILURE;
  switch (frame.can_id & 0x7FFFFFFF) {
    case IMU_CAN_BODY_POSITION_FRAME_ID: {
      _status = EXIT_SUCCESS;
      struct imu_can_body_position_t st_body_pos;
      int status_unpack = imu_can_body_position_unpack(&st_body_pos, frame.data, sizeof(frame.data));
      // Physical mounting may require changing up pitch and roll.
      log_data.yaw = imu_can_body_position_yaw_angle_decode(st_body_pos.yaw_angle);
      log_data.pitch = imu_can_body_position_pitch_angle_decode(st_body_pos.pitch_angle);
      log_data.roll = imu_can_body_position_roll_angle_decode(st_body_pos.roll_angle);
      error_message(INFO, "CAN:Body Roll: %f", log_data.roll);
    }
    break;

    case IMU_CAN_BODY_ACCEL_FRAME_ID: {
      _status = EXIT_SUCCESS;
      struct imu_can_body_accel_t st_body_acc;
      int status_unpack = imu_can_body_accel_unpack(&st_body_acc, frame.data, sizeof(frame.data));
      // Physical mounting may require changing up x and y
      log_data.acc_forward = imu_can_body_accel_acc_x_decode(st_body_acc.acc_x);
      log_data.acc_side = imu_can_body_accel_acc_y_decode(st_body_acc.acc_y);
      log_data.acc_vert = imu_can_body_accel_acc_z_decode(st_body_acc.acc_z);
      error_message(INFO, "CAN:IMU X Accel: %f", log_data.acc_forward);
    }
    break;

    case IGNITECH_CAN_IGNITECH_WB_2_FRAME_ID: {
      _status = EXIT_SUCCESS;
      struct ignitech_can_ignitech_wb_2_t st_wb2;
      int status_unpack = ignitech_can_ignitech_wb_2_unpack(&st_wb2, frame.data, sizeof(frame.data));
      log_data.lambda = st_wb2.lambda; // Both scaled by 0.01, no conversion necessary
      error_message(DEBUG, "CAN:ignitech lambda: %f", ignitech_can_ignitech_wb_2_lambda_decode(st_wb2.lambda));
    }
    break;

    case OBD2_OBD2_REQUEST_FRAME_ID: {
      _status = obd2_process(frame, log_data, can_s);
      error_message(DEBUG, "CAN:OBD2 Request");
    }
    break;

    default: /// Default if can_id is not in the switch print out message details
      error_message(WARN, "WARN:Unknown CAN frame%: %X", frame.can_id);
  }
  if (_status == EXIT_FAILURE) {
    // TODO do something if it wasn't decoded?
    //printf("%8X %d\n", frame.can_id, frame.can_dlc);
  }
}

int obd2_process(const can_frame &frame, bike_data &log_data, const int can_s)
{
  int _status = EXIT_FAILURE;
  struct obd2_obd2_request_t obd2Request;
  obd2_obd2_request_unpack(&obd2Request, frame.data, frame.can_dlc);
  // Check Valid Mode and PIDs
  if ( !obd2_obd2_request_mode_is_in_range(obd2Request.mode) )
    return EXIT_FAILURE;
  if ( !obd2_obd2_request_pid_is_in_range(obd2Request.pid) )
    return EXIT_FAILURE;

  // Send responses for each supported mode/pid
  if ( obd2Request.mode == OBD2_OBD2_SERVICE_SHOW_CURRENT_DATA__CHOICE ) {
    switch (obd2Request.pid) {
      case OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_00_PI_DS_SUPPORTED_01_20_CHOICE: {
        _status = EXIT_SUCCESS;
        error_message(DEBUG, "OBD2: Supported PIDS");
      }
      break;

      case OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_0_C_ENGINE_RPM_CHOICE: {
        struct can_frame _response;
        struct obd2_obd2_t _obd2_response;
        _obd2_response.s1_pid_0_c_engine_rpm = obd2_obd2_s1_pid_0_c_engine_rpm_encode(log_data.rpm);
        _obd2_response.length = 4u; // MODE(1) + PID(1) + Data Bytes (2)
        _obd2_response.response = 4u;  // Every response packet is 4
        _obd2_response.service = OBD2_OBD2_SERVICE_SHOW_CURRENT_DATA__CHOICE;
        _obd2_response.parameter_id_service01 = OBD2_OBD2_PARAMETER_ID_SERVICE01_S1_PID_0_C_ENGINE_RPM_CHOICE;
        obd2_obd2_pack(_response.data, &_obd2_response, 8);
        _response.can_dlc = 8;
        _response.can_id = OBD2_OBD2_FRAME_ID;
        if (write(can_s, &_response, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
          error_message(ERROR, "OBD2: Response Write failed");
          return EXIT_FAILURE;
        }
      }
      break;

      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }

  else if ( obd2Request.mode == OBD2_OBD2_SERVICE_SHOW_FREEZE_FRAME_DATA__CHOICE ) {
    switch (obd2Request.pid) {
      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }

  else if ( obd2Request.mode == OBD2_OBD2_SERVICE_SHOW_STORED_DT_CS__CHOICE ) {
    switch (obd2Request.pid) {
      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }

  else if ( obd2Request.mode == OBD2_OBD2_SERVICE_CLEAR_DT_CS_AND_STORED_VALUES_CHOICE ) {
    switch (obd2Request.pid) {
      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }

  else if ( obd2Request.mode == OBD2_OBD2_SERVICE_OXYGEN_SENSOR_MONITORING__CHOICE ) {
    switch (obd2Request.pid) {
      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }

  else if ( obd2Request.mode == OBD2_OBD2_SERVICE_OTHER_SYSTEM_MONITORING__CHOICE ) {
    switch (obd2Request.pid) {
      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }

  else if ( obd2Request.mode == OBD2_OBD2_SERVICE_SHOW_PENDING_DT_CS__CHOICE ) {
    switch (obd2Request.pid) {
      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }

  else if ( obd2Request.mode == OBD2_OBD2_SERVICE_CONTROL_ON_BOARD_SYSTEM__CHOICE ) {
    switch (obd2Request.pid) {
      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }

  else if ( obd2Request.mode == OBD2_OBD2_SERVICE_REQUEST_VEHICLE_INFORMATION__CHOICE ) {
    switch (obd2Request.pid) {
      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }

  else if ( obd2Request.mode == OBD2_OBD2_SERVICE_PERMANENT_DT_CS___CLEARED_DT_CS___CHOICE ) {
    switch (obd2Request.pid) {
      default:
        error_message(WARN, "WARN:Unknown OBD2 MODE:PID: %X:%X", obd2Request.mode, obd2Request.pid);
    }
  }
  return _status;
}

