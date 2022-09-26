/*
    EngineDataLogger
    Copyright (C) 2018-2020  Jacob Geigle

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

// TODO
// Convert to class
// fold fc_open into here

#include "front_controls.h"

int readFC(int &fd_front_controls, bike_data &log_data)
{
  int	n = 0,
      i = 0,
      i_serial_in = 0;
  char	buf [100],
        serial_in[100];

  n = read (fd_front_controls, buf, sizeof(buf));
  //n = read (fd_front_controls, buf, FC_CMD_SIZE);
  error_message (DEBUG, "Reading Serial: %d Bytes", n);
  // for front_controls 8 bytes are needed, copy all bytes read
  if (n >= FC_CMD_SIZE) {
    error_message(DEBUG, "Got enough, processing");
    for (i = 0; i < n; i++) {
      serial_in[i_serial_in] = buf[i];
      i_serial_in++;
    }
    // process copied bytes, only really care about last valid input
    // loop backwards and find '\n' or beginning of buf
    while (serial_in[i_serial_in] != '\n' && i_serial_in > 0) {
      char tmp[2];
      tmp[1] = 0;
      tmp[0] = serial_in[i_serial_in];
      error_message(DEBUG, "Index = %d, Character = %s", i_serial_in, tmp);
      i_serial_in--;
    }
    char tmp[2];
    tmp[1] = 0;
    tmp[0] = serial_in[i_serial_in];
    error_message(DEBUG, "Index = %d, Character = %s", i_serial_in, tmp);
    if (serial_in[i_serial_in] == '\n' && i_serial_in >= FC_CMD_SIZE - 1) {
      error_message(DEBUG, "Found a newline");
      // Process FC_CMD_SIZE bytes and convert to usable variables
      i_serial_in = i_serial_in - FC_CMD_SIZE + 1;
      log_data.inputCmdD = serial_in[i_serial_in];
      log_data.inputCmdC = serial_in[i_serial_in + 1];
      i_serial_in += 2;
      memcpy((void *)&log_data.systemvoltage, (void *)&serial_in[i_serial_in], 4);

      char 	CmdC[9],
            CmdD[9];
      strcpy(CmdC, exCmd_bin(log_data.inputCmdC));
      strcpy(CmdD, exCmd_bin(log_data.inputCmdD));
      error_message (INFO, "inputCmdD: %s inputCmdC: %s Voltage: %f", CmdD, CmdC, log_data.systemvoltage);
      log_data.brake_on = log_data.inputCmdD & BRAKE_ON;
      log_data.horn_on = log_data.inputCmdD & HORN_ON;
      log_data.blink_left = log_data.inputCmdD & LEFT_ON;
      log_data.blink_right = log_data.inputCmdD & RIGHT_ON;
      log_data.high_on = log_data.inputCmdD & HIGH_BEAMS_ON;
      log_data.kill_on = log_data.inputCmdD & KILL_ON;
      log_data.clutch_disengaged = log_data.inputCmdC & CLUTCH_DISENGAGED;
      log_data.kickstand_up = log_data.inputCmdC & KICKSTAND_UP;
      log_data.in_neutral = log_data.inputCmdC & IN_NEUTRAL;
    }
  } else if (n == 0) {
    error_message(WARN, "FC zero data length. closing");
    close(fd_front_controls);
    fd_front_controls = -1;
  }
  return 0;
}

int writeFC(int fd_front_controls, bike_data &log_data)
{
  int result;

  error_message (INFO, "Sending 6 bytes to front controls");
  char 	CmdA[9],
        CmdB[9],
        CmdC[9],
        CmdD[9];
  strcpy(CmdA, exCmd_bin(log_data.serialCmdA));
  strcpy(CmdB, exCmd_bin(log_data.serialCmdB));
  strcpy(CmdC, exCmd_bin(log_data.serialCmdC));
  strcpy(CmdD, exCmd_bin(log_data.serialCmdD));
  error_message (DEBUG, "A: %s B: %s C: %s D: %s", CmdA, CmdB, CmdC, CmdD);
  unsigned char	buf[6];
  buf[0] = 'A';
  buf[1] = log_data.serialCmdD;
  buf[2] = log_data.serialCmdB;
  buf[3] = log_data.serialCmdC;
  buf[4] = log_data.serialCmdA;
  buf[5] = 'Z';
  result = write(fd_front_controls, buf, 6);
  if (result < 0 ) {
    error_message (ERROR, "Write error: %d - %s", errno, strerror(errno));
    return -1;
  } else {
    return 0;
  }
}

/**
 * exCmd_bin - Create a string representation of an 8-bit binary value
 * @param@ uint8_t the 8 bits of data to be represented
 * @return@ pointer to char array containing string of '1's and '0's
 *
 */
char *exCmd_bin(uint8_t cmd)
{
  static char buf[9];
  int i;
  for (i = 0; i < 8; i++) {
    buf[7 - i] = '0' + ((cmd >> i) & 0x01);
  }
  buf[8] = 0;
  return buf;
}

