#include "bluetooth.h"
#include "error_handling.h"

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

	public:
	EDL_Bluetooth(){
		EDL_Bluetooth(1);
	}

	EDL_Bluetooth(int bt_port) {
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
	}

	int Accept(){
		// accept one connection
		client = accept(s, (struct sockaddr *)&rem_addr, &opt);

		ba2str( &rem_addr.rc_bdaddr, buf );
		fprintf(stderr, "accepted connection from %s\n", buf);
		memset(buf, 0, sizeof(buf));
		return client;
	}

	void Send() {
		// Send data
		for (int i=0;i<50;i++) {
			sprintf(output,"%d",rpm);
			write(client,output,sizeof(output));
			sleep(1);
			rpm += 100;
		}
	}
	int Close() {
		// close connection
		close(client);
		close(s);
		return 0;
	}

	void Read() {
		// read data from the client
		bytes_read = read(client, buf, sizeof(buf));
		if( bytes_read > 0 ) {
			printf("received [%s]\n", buf);
		}
	}
};
