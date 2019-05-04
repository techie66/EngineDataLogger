/*
 * EDL Main program
 * READ THE README!!!!!!
 */

#include <unistd.h>			//Needed for I2C port
#include <fcntl.h>			//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <time.h>
#include <sys/time.h>
#include <getopt.h>
#include "I2Cdev.h"
#include "serial.h"
#include "definitions.h"
#include "error_handling.h"
#include "front_controls.h"
#include "bluetooth.h"

volatile sig_atomic_t 	time_to_quit = false;
// Set default Debug Level Here NONE,ERROR,WARN,INFO,DEBUG
e_lvl			LEVEL_DEBUG = WARN;

int fc_open(const char *filepath) {
	int fd;
	fd = open (filepath, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0) {
		error_message (WARN,"error %d opening %s: %s", errno, filepath, strerror (errno));
		return -1;
	}

	// Set interface parameters for front controls
	set_interface_attribs (fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
	set_blocking (fd, 0);                // set no blocking
	return fd;
}

int main(int argc, char *argv[])
{
	// TODO: move variable declarations to more sensible spots
	// keep constants up here
	char const	*front_controls_port = "/dev/ttyUSB0",
	//char const	*front_controls_port = "/tmp/ttyV0",
	     		*i2c_device = "/dev/i2c-1",
			*log_file = "system_log.csv";
	fc_data		fcData;
	engine_data	enData;
	System_CMD db_from_cmd = NO_CMD;
	char		en_to_cmd = 0;
	bool		engineRunning = false;
	int		select_result = 0;
	int 		fd_front_controls,
			fd_i2c,
			length,
			max_fd = 0;
	FILE		*fd_log;
	fd_set		readset,
			writeset;
	struct timeval	timeout,
			currtime;
	char	      	time_buf[100];
	time_t		my_time;
	EDL_Bluetooth	dashboard;


	// register signal SIGINT and signal handler  
	signal(SIGINT, signalHandler);  

	// Process options
	// put ':' in the starting of the
	// opt string so that program can
	// distinguish between '?' and ':'
	while(true)
	{
		static struct option long_options[] =
		{
			/* These options don't set a flag. */
			{"verbose",	required_argument,	0, 'v'},
			{"quiet",	no_argument,		0, 'q'},
			{"help",	no_argument,		0, 'h'},
			{0,0,0,0}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;
		int opt = getopt_long (argc, argv, ":qv:h",
				long_options, &option_index);
		/* Detect the end of the options. */
		if (opt == -1)
			break;
		switch(opt)
		{
		case 'v':
			printf("Option Debug: %s\n", optarg);
			if (strcmp("DEBUG",optarg)==0) {
				LEVEL_DEBUG = DEBUG;
				break;
			}
			else if (strcmp("INFO",optarg)==0) {
				LEVEL_DEBUG = INFO;
				break;
			}
			else if (strcmp("WARN",optarg)==0) {
				LEVEL_DEBUG = WARN;
				break;
			}
			else if (strcmp("ERROR",optarg)==0) {
				LEVEL_DEBUG = ERROR;
				break;
			}
			else if (strcmp("NONE",optarg)==0) {
				LEVEL_DEBUG = NONE;
				break;
			} 
			break;
		case 'q':
			LEVEL_DEBUG = NONE;
			break;
		case '?':
		case 'h':
			printf("USAGE: data_logger [OPTIONS]\n");
			printf("Log engine data.\n\n");
			printf("Options:\n");
			printf("  -v,  --verbose=LEVEL\t\tSet verbose level.\n");
			printf("\t\t\t\tLEVEL is one of: NONE,ERROR,WARN,INFO,DEBUG\n");
			printf("  -q\t\t\t\tSame as --verbose = NONE\n");
			printf("  -h, --help\t\t\tPrint this help message.\n");
			return 0;
		}
	}

	// optind is for the extra arguments
	// which are not parsed
	for(; optind < argc; optind++){
		printf("extra arguments: %s\n", argv[optind]);
	}


	// Open Front controls
	fd_front_controls = fc_open(front_controls_port);
	
	// Initialize I2C Interface
	//I2Cdev::initialize();
	//----- OPEN THE I2C BUS -----
	if ((fd_i2c = open(i2c_device, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		error_message (ERROR,"Failed to open the i2c bus. err:%d - %s",errno,strerror(errno));
		//return -1;
	}
	
	// Open log file
	if ((fd_log = fopen(log_file,"a")) < 0) {
		error_message (ERROR,"Failed to open the log file err:%d - %s",errno,strerror(errno));
		//return -1;
	}
	fprintf(fd_log,"\n");

	// Main Loop
	while (!time_to_quit) {
		// Setup read sets
		FD_ZERO(&readset);
		FD_SET(dashboard.getListener(),&readset);
		max_fd = (max_fd>dashboard.getListener())?max_fd:dashboard.getListener();
		if (fd_front_controls > 0) {
			error_message(DEBUG,"Adding front controls to select");
			FD_SET(fd_front_controls,&readset);
			max_fd = (max_fd>fd_front_controls)?max_fd:fd_front_controls;
		}
		else {
			fd_front_controls = fc_open(front_controls_port);
		}
		if (dashboard.getClient() > 0) {
			error_message(DEBUG,"Adding dashboard client to select");
			FD_SET(dashboard.getClient(),&readset);
			max_fd = (max_fd>dashboard.getClient())?max_fd:dashboard.getClient();
		}
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;

		// Read all inputs
		
		//----- READ ENGINE DATA I2C -----
		// TODO: Make class for engine_data just like the other components
		// Set I2C addr
		if (ioctl(fd_i2c, I2C_SLAVE, engine_data_addr) < 0)
		{
			//ERROR HANDLING; you can check errno to see what went wrong
			error_message (ERROR,"Failed to acquire bus access and/or talk to slave. err:%d - %s",errno,strerror(errno));
			//return -1;
		}

		// length is calculated by fixed size of data stream from Sleepy Pi
		length = sizeof(enData.rpm) + sizeof(enData.speed) + sizeof(enData.temp_oil) + sizeof(enData.batteryVoltage)
			+ sizeof(enData.odometer);			//<<< Number of bytes to read
		
		unsigned char 	buffer[60] = {0};
		unsigned char	*buffer_ptr = buffer;
		if (read(fd_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)KE
		{
			//ERROR HANDLING: i2c transaction failed
			error_message (WARN,"Failed to read from the i2c bus.");
		}
		else
		{
			// Good I2C read, Sort out the various packed variables
			memcpy((void*)&enData.rpm,(void*)buffer_ptr,sizeof(enData.rpm));
			buffer_ptr += sizeof(enData.rpm);
			uint16_t tmp = 0;
			memcpy((void*)&tmp,(void*)buffer_ptr,sizeof(tmp));
			enData.temp_oil = tmp / 100.0;
			buffer_ptr += sizeof(tmp);
			memcpy((void*)&tmp,(void*)buffer_ptr,sizeof(tmp));
			enData.speed = tmp / 100.0;
			buffer_ptr += sizeof(tmp);
			memcpy((void*)&enData.batteryVoltage,(void*)buffer_ptr,sizeof(enData.batteryVoltage));
			buffer_ptr += sizeof(enData.batteryVoltage);
			memcpy((void*)&enData.odometer,(void*)buffer_ptr,sizeof(enData.odometer));
			buffer_ptr += sizeof(enData.odometer);
			memcpy((void*)&enData.trip,(void*)buffer_ptr,sizeof(enData.trip));
			buffer_ptr = buffer;
			error_message (INFO,"ODO: %d RPM: %d Speed: %f Oil temp: %f BatV: %f",enData.odometer,enData.rpm,enData.speed,enData.temp_oil, enData.batteryVoltage);
		}

		// Do Select()
		select_result = select(max_fd+1, &readset, &writeset, NULL, &timeout);
		if (select_result < 0) {
			error_message (WARN, "error %d Port: %s: %s", errno, front_controls_port, strerror (errno));
		}
		else if (select_result == 0){
			// Timeout
			// #Dontcare
		}
		else if (select_result > 0) {
			error_message(DEBUG,"Select something");

			if (FD_ISSET(fd_front_controls,&readset)) {
				error_message (DEBUG,"Select read front controls");
				readFC(fd_front_controls,fcData);
			}
			if (FD_ISSET(dashboard.getListener(),&readset)) {
				error_message(DEBUG,"Select new dashboard client");
				dashboard.Accept();
			}
			if (FD_ISSET(dashboard.getClient(),&readset)) {
				error_message(DEBUG,"Select read dashboard");
				db_from_cmd = dashboard.Read();
			}
		}

		// calculate stuff / make decisions
		if (enData.rpm > RUNNING_RPM) {
			engineRunning = true;
			fcData.serialCmdA |= ENGINE_RUNNING;
			error_message (DEBUG,"Running. %s",exCmd_bin(fcData.serialCmdA));
		}
		else {
			engineRunning = false;
			fcData.serialCmdA &= ~ENGINE_RUNNING;
			error_message (DEBUG,"Not Running. %s",exCmd_bin(fcData.serialCmdA));
		}

		if(db_from_cmd == TRPRST) {
			error_message(WARN,"Resetting Trip");
			en_to_cmd = 'T';
			db_from_cmd = NO_CMD;
		}

		// Create Flatbuffer
		// Sent to dashboard and 
		// TODO: data logfile
		EDL::AppBuffer::BikeT bikeobj;
		flatbuffers::FlatBufferBuilder fbb;

		// TODO: get rid of fcData and enData, use BikeT for everything
		// define bikeobj with main scope
		bikeobj.rpm = enData.rpm;
		bikeobj.speed = enData.speed;
		bikeobj.odometer = enData.odometer;
		bikeobj.oil_temp = enData.temp_oil;
		bikeobj.batteryvoltage = enData.batteryVoltage;
		bikeobj.blink_left = fcData.left_on;
		bikeobj.blink_right = fcData.right_on;
		bikeobj.trip = enData.trip;
		// Serialize into new flatbuffer.
		fbb.Finish(EDL::AppBuffer::Bike::Pack(fbb, &bikeobj));

		// Send commands
		FD_ZERO(&writeset);
		max_fd = (max_fd>dashboard.getListener())?max_fd:dashboard.getListener();
		if (fd_front_controls > 0) {
			FD_SET(fd_front_controls,&writeset);
			max_fd = (max_fd>fd_front_controls)?max_fd:fd_front_controls;
		}
		if (dashboard.getClient() > 0) {
			FD_SET(dashboard.getClient(),&writeset);
			max_fd = (max_fd>dashboard.getClient())?max_fd:dashboard.getClient();
		}
		if (en_to_cmd != 0) {
			// write to en
			write(fd_i2c,(const void*)&en_to_cmd,sizeof(en_to_cmd));
			en_to_cmd = 0;
		}
		// Do Select()
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		select_result = select(max_fd + 1, NULL, &writeset, NULL, &timeout);
		if (select_result < 0) {
			error_message (WARN, "error %d : %s", errno, strerror (errno));
		}
		else if (select_result == 0){
			// Timeout
			// #Dontcare
		}
		else if (select_result > 0) {
			if (FD_ISSET(fd_front_controls,&writeset)) {
				// Front controls accepts commands
				writeFC(fd_front_controls,fcData);
			}
			if (FD_ISSET(dashboard.getClient(),&writeset)) {
				error_message(DEBUG,"Select write dashboard");
				dashboard.Send(fbb);
			}
		}
		
		// Log data
		gettimeofday(&currtime, NULL);
		my_time = currtime.tv_sec;
		strftime(time_buf, 100, "%D %T", localtime(&my_time));
		// RPM, Speed, sysvoltage, batVoltage, oil temp, running, Time
		fprintf(fd_log,"%d,%.2f,%.2f,%.2f,%.2f,%d,%s.%ld\n",enData.rpm,enData.speed,fcData.systemVoltage,enData.batteryVoltage,enData.temp_oil,engineRunning,time_buf,currtime.tv_usec);
		fflush(fd_log);

		usleep(50000);
	}

	// Return 0 for clean exit
	return 0;
}

