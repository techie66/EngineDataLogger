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

#include "serial.h"

int
set_interface_attribs (int fd, int speed, int parity)
{
  struct termios tty;
  memset (&tty, 0, sizeof tty);
  if (tcgetattr (fd, &tty) != 0) {
    error_message (ERROR, "error %d from tcgetattr", errno);
    //return -1;
  }

  cfsetospeed (&tty, speed);
  cfsetispeed (&tty, speed);
  /*
  	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
  	// disable IGNBRK for mismatched speed tests; otherwise receive break
  	// as \000 chars
  	tty.c_iflag &= ~IGNBRK;         // disable break processing
  	tty.c_lflag = 0;                // no signaling chars, no echo,
  									// no canonical processing
  	tty.c_oflag = 0;                // no remapping, no delays

  	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
  */
  cfmakeraw(&tty);
  tty.c_cc[VMIN]  = 0;            // read doesn't block
  tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CRTSCTS;
  tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
  // enable reading
  tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
  tty.c_cflag |= parity;

  if (tcsetattr (fd, TCSANOW, &tty) != 0) {
    error_message (ERROR, "error %d from tcsetattr", errno);
    //return -1;
  }
  return 0;
}

void
set_blocking (int fd, int should_block)
{
  struct termios tty;
  memset (&tty, 0, sizeof tty);
  if (tcgetattr (fd, &tty) != 0) {
    error_message (ERROR, "error %d from tggetattr", errno);
    //return;
  }

  tty.c_cc[VMIN]  = should_block ? 1 : 0;
  tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

  if (tcsetattr (fd, TCSANOW, &tty) != 0)
    error_message (ERROR, "error %d setting term attributes", errno);
}


