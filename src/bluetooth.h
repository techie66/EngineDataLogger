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
*/#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include "cb1100f-app_generated.h"
#include "definitions.h"

class EDL_Bluetooth
{
 private:
  struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
  char buf[1024] = { 0 };
  int s, client, bytes_read;
  socklen_t opt = sizeof(rem_addr);
  bdaddr_t loc_BDADDR_ANY = {};
  void Init(int);

 public:
  EDL_Bluetooth();
  EDL_Bluetooth(int bt_port);
  int Accept();
  void Send(const flatbuffers::FlatBufferBuilder &fbb);
  int Close();
  System_CMD Read();
  int getListener();
  int getClient();
};

#endif
