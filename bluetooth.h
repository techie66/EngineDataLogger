#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>

class EDL_Bluetooth
{
	private:
		struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
		char buf[1024] = { 0 };
		char output[30] = {0};
		int rpm = 2000;
		int s, client, bytes_read;
		socklen_t opt = sizeof(rem_addr);
		bdaddr_t loc_BDADDR_ANY = {};
		void Init(int);

	public:
	EDL_Bluetooth();
	EDL_Bluetooth(int bt_port);
	int Accept();
	void Send();
	int Close();
	void Read();
	int getListener();
	int getClient();
};

#endif
