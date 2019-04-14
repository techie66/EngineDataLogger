#include "bluetooth.h"
#include "error_handling.h"

EDL_Bluetooth::EDL_Bluetooth(){
	Init(1);
}

EDL_Bluetooth::EDL_Bluetooth(int bt_port) {
	Init(bt_port);
}

void EDL_Bluetooth::Init(int bt_port) {
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
}
int EDL_Bluetooth::Accept(){
	// accept one connection
	client = accept(s, (struct sockaddr *)&rem_addr, &opt);

	ba2str( &rem_addr.rc_bdaddr, buf );
	fprintf(stderr, "accepted connection from %s\n", buf);
	memset(buf, 0, sizeof(buf));
	fcntl(client, F_SETFL, O_NONBLOCK);
	return client;
}

void EDL_Bluetooth::Send() {
	// Send data
	// TODO Actually send real data
	sprintf(output,"%d",rpm);
	rpm = bswap_32(rpm);
	write(client,&rpm,sizeof(rpm));
	rpm = bswap_32(rpm);
	//write(client,output,sizeof(output));
	rpm += 100;
	rpm %= 12000;
}
int EDL_Bluetooth::Close() {
	// close connection
	close(client);
	client = -1;
	//close(s);
	return 0;
}

void EDL_Bluetooth::Read() {
	// read data from the client
	// TODO do something with it
	bytes_read = read(client, buf, sizeof(buf));
	error_message(DEBUG,"Bytes Read= %d",bytes_read);
	if( bytes_read > 0 ) {
		error_message(DEBUG,"received [%s]\n", buf);
	}
	else if (bytes_read <= 0 ) {
		Close();
	}
}

int EDL_Bluetooth::getClient() {
	return client;
}

int EDL_Bluetooth::getListener() {
	return s;
}

