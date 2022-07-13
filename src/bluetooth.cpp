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
#include "bluetooth.h"
#include "error_handling.h"

EDL_Bluetooth::EDL_Bluetooth()
{
  Init(1);
}

EDL_Bluetooth::EDL_Bluetooth(int bt_port)
{
  Init(bt_port);
}

void EDL_Bluetooth::Init(int bt_port)
{
  // allocate socket
  s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
  // bind socket to port <bt_port> of the first available
  // local bluetooth adapter
  loc_addr.rc_family = AF_BLUETOOTH;
  loc_addr.rc_bdaddr = loc_BDADDR_ANY;
  loc_addr.rc_channel = (uint8_t) bt_port;
  bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

  // put socket into listening mode
  listen(s, 1);
  fcntl(s, F_SETFL, O_NONBLOCK);
  client = -1;
}
int EDL_Bluetooth::Accept()
{
  // accept one connection
  client = accept(s, (struct sockaddr *)&rem_addr, &opt);

  ba2str( &rem_addr.rc_bdaddr, buf );
  error_message(WARN, "Accepted connection from [%s]\n", buf);
  memset(buf, 0, sizeof(buf));
  fcntl(client, F_SETFL, O_NONBLOCK);
  return client;
}

void EDL_Bluetooth::Send(const flatbuffers::FlatBufferBuilder &fbb)
{
  uint8_t *buf = fbb.GetBufferPointer();
  int size = fbb.GetSize();
  write(client, buf, size);
}
int EDL_Bluetooth::Close()
{
  // close connection
  close(client);
  client = -1;
  //close(s);
  return 0;
}

System_CMD EDL_Bluetooth::Read()
{
  // read data from the client
  // TODO do something with it
  bytes_read = read(client, buf, sizeof(buf));
  error_message(INFO, "Bluetooth Bytes Read= %d", bytes_read);
  if ( bytes_read > 0 ) {
    buf[bytes_read] = 0;
    error_message(INFO, "received [%s]\n", buf);
    if (strcmp(buf, "TRPRST") == 0) {
      return TRPRST;
    }
    if (strcmp(buf, "O2MANON") == 0) {
      return O2MANON;
    }
    if (strcmp(buf, "O2MANOFF") == 0) {
      return O2MANOFF;
    }
  } else if (bytes_read <= 0 ) {
    Close();
  }
  return NO_CMD;
}

int EDL_Bluetooth::getClient()
{
  return client;
}

int EDL_Bluetooth::getListener()
{
  return s;
}

