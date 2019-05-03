// TODO
// Convert to class
// fold fc_open into here

#include "front_controls.h"

int readFC(int& fd_front_controls, fc_data& fcData) {
	int	n = 0,
		i = 0,
		i_serial_in = 0;
	char	buf [100],
		serial_in[100];

	n = read (fd_front_controls, buf, sizeof(buf));
	//n = read (fd_front_controls, buf, FC_CMD_SIZE);
	error_message (DEBUG,"Reading Serial: %d Bytes",n);
	// for front_controls 8 bytes are needed, copy all bytes read
	if (n >= FC_CMD_SIZE) {
		error_message(DEBUG,"Got enough, processing");
		for(i=0;i<n;i++) {
			serial_in[i_serial_in] = buf[i];
			i_serial_in++;
		}
		// process copied bytes, only really care about last valid input
		// loop backwards and find '\n' or beginning of buf
		while (serial_in[i_serial_in] != '\n' && i_serial_in > 0) {
			char tmp[2];
			tmp[1] = 0;
			tmp[0] = serial_in[i_serial_in];
			error_message(DEBUG,"Index = %d, Character = %s",i_serial_in,tmp);
			i_serial_in--;
		}
		char tmp[2];
		tmp[1] = 0;
		tmp[0] = serial_in[i_serial_in];
		error_message(DEBUG,"Index = %d, Character = %s",i_serial_in,tmp);
		if (serial_in[i_serial_in] == '\n' && i_serial_in >= FC_CMD_SIZE-1){
			error_message(DEBUG,"Found a newline");
			// Process FC_CMD_SIZE bytes and convert to usable variables
			i_serial_in = i_serial_in - FC_CMD_SIZE + 1;
			fcData.inputCmdD = serial_in[i_serial_in];
			fcData.inputCmdC = serial_in[i_serial_in + 1];
			i_serial_in += 2;
			memcpy((void*)&fcData.systemVoltage,(void*)&serial_in[i_serial_in],4);

			char 	CmdC[9],
				CmdD[9];
			strcpy(CmdC,exCmd_bin(fcData.inputCmdC));
			strcpy(CmdD, exCmd_bin(fcData.inputCmdD));
			error_message (INFO,"inputCmdD: %s inputCmdC: %s Voltage: %f",CmdD,CmdC,fcData.systemVoltage);
			fcData.brake_on = fcData.inputCmdD & BRAKE_ON;
			fcData.horn_on = fcData.inputCmdD & HORN_ON;
			fcData.left_on = fcData.inputCmdD & LEFT_ON;
			fcData.right_on = fcData.inputCmdD & RIGHT_ON;
			fcData.high_on = fcData.inputCmdD & HIGH_BEAMS_ON;
			fcData.kill_on = fcData.inputCmdD & KILL_ON;
			fcData.clutch_disengaged = fcData.inputCmdC & CLUTCH_DISENGAGED;
			fcData.kickstand_up = fcData.inputCmdC & KICKSTAND_UP;
			fcData.in_neutral = fcData.inputCmdC & IN_NEUTRAL;
		}
	}
	else if (n == 0) {
		error_message(WARN,"FC zero data length. closing");
		close(fd_front_controls);
		fd_front_controls = -1;
	}
	return 0;
}

int writeFC(int fd_front_controls, fc_data& fcData) {
	int result;

	error_message (INFO,"Sending 6 bytes to front controls");
	char 	CmdA[9],
		CmdB[9],
		CmdC[9],
		CmdD[9];
	strcpy(CmdA,exCmd_bin(fcData.serialCmdA));
	strcpy(CmdB, exCmd_bin(fcData.serialCmdB));
	strcpy(CmdC, exCmd_bin(fcData.serialCmdC));
	strcpy(CmdD, exCmd_bin(fcData.serialCmdD));
	error_message (DEBUG,"A: %s B: %s C: %s D: %s",CmdA,CmdB,CmdC,CmdD);
	unsigned char	buf[6];
	buf[0] = 'A';
	buf[1] = fcData.serialCmdD;
	buf[2] = fcData.serialCmdB;
	buf[3] = fcData.serialCmdC;
	buf[4] = fcData.serialCmdA;
	buf[5] = 'Z';
	result = write(fd_front_controls,buf,6);
	if (result < 0 ){
		error_message (ERROR,"Write error: %d - %s",errno,strerror(errno));
		return -1;
	}
	else {
		return 0;
	}
}

char *exCmd_bin(uint8_t cmd) {
	static char buf[9];
	int i;
	for(i=0;i<8;i++) {
		buf[7-i] = '0'+((cmd >> i) & 0x01);
	}
	return buf;
}

