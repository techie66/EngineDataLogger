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
	client = -1;
}
int EDL_Bluetooth::Accept(){
	// accept one connection
	client = accept(s, (struct sockaddr *)&rem_addr, &opt);

	ba2str( &rem_addr.rc_bdaddr, buf );
	error_message(WARN,"Accepted connection from [%s]\n", buf);
	memset(buf, 0, sizeof(buf));
	fcntl(client, F_SETFL, O_NONBLOCK);
	return client;
}

void EDL_Bluetooth::Send(const flatbuffers::FlatBufferBuilder& fbb) {
	uint8_t *buf = fbb.GetBufferPointer();
	int size = fbb.GetSize();	
	write(client,buf,size);
}
int EDL_Bluetooth::Close() {
	// close connection
	close(client);
	client = -1;
	//close(s);
	return 0;
}

System_CMD EDL_Bluetooth::Read() {
	// read data from the client
	// TODO do something with it
	bytes_read = read(client, buf, sizeof(buf));
	error_message(INFO,"Bluetooth Bytes Read= %d",bytes_read);
	if( bytes_read > 0 ) {
		buf[bytes_read] = 0;
		error_message(INFO,"received [%s]\n", buf);
		if(strcmp(buf,"TRPRST") == 0) {
			return TRPRST;
		}
		if(strcmp(buf,"O2MANON") == 0) {
			return O2MANON;
		}
		if(strcmp(buf,"O2MANOFF") == 0) {
			return O2MANOFF;
		}
	}
	else if (bytes_read <= 0 ) {
		Close();
	}
	return NO_CMD;
}

int EDL_Bluetooth::getClient() {
	return client;
}

int EDL_Bluetooth::getListener() {
	return s;
}

