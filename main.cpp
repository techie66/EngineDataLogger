#include <unistd.h>			//Needed for I2C port
#include <fcntl.h>			//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include "I2Cdev.h"
#include "serial.h"
#include "definitions.h"
#include "error_handling.h"
#include "front_controls.h"
#include <time.h>
#include <sys/time.h>
#include <getopt.h>

volatile sig_atomic_t 	time_to_quit = false;
// Set Debug Level Here NONE,ERROR,WARN,INFO,DEBUG
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
	int opt;
	char const	*front_controls_port = "/dev/ttyUSB0",
	//char const	*front_controls_port = "/tmp/ttyV0",
	     		*i2c_device = "/dev/i2c-1",
			*log_file = "system_log.csv";
	fc_data		fcData;
	engine_data	enData;
	bool		engineRunning = false;
	int		select_result = 0,
			result = 0;
	int 		fd_front_controls,
			fd_i2c,
			length;
	FILE		*fd_log;
	fd_set		readset,
			writeset;
	struct timeval	timeout,
			currtime;
	unsigned char 	buffer[60] = {0},
			*buffer_ptr = buffer;
	char	      	time_buf[100];
	time_t		my_time;


	// register signal SIGINT and signal handler  
	signal(SIGINT, signalHandler);  

	// Process options
	// put ':' in the starting of the
	// string so that program can
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
		opt = getopt_long (argc, argv, ":qv:",
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
	
		

	// Listen for bluetooth client and connect any clients
	// TODO
	
	// Open log file
	if ((fd_log = fopen(log_file,"a")) < 0) {
		error_message (ERROR,"Failed to open the i2c bus. err:%d - %s",errno,strerror(errno));
		//return -1;
	}
	fprintf(fd_log,"\n");

	// Main Loop
	while (!time_to_quit) {
		// Do some stuff
		FD_ZERO(&readset);
		if (fd_front_controls > 0) {
			FD_SET(fd_front_controls,&readset);
		}
		else {
			fd_front_controls = fc_open(front_controls_port);
		}
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;

		// Read all inputs
		
		//----- READ ENGINE DATA I2C -----
		// Set I2C addr
		if (ioctl(fd_i2c, I2C_SLAVE, engine_data_addr) < 0)
		{
			//ERROR HANDLING; you can check errno to see what went wrong
			error_message (ERROR,"Failed to acquire bus access and/or talk to slave. err:%d - %s",errno,strerror(errno));
			//return -1;
		}

		// length is calculated by fixed size of data stream from Sleepy Pi
		length = sizeof(enData.rpm) + sizeof(enData.speed) + sizeof(enData.temp_oil) + sizeof(enData.batteryVoltage);			//<<< Number of bytes to read
		if (read(fd_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)KE
		{
			//ERROR HANDLING: i2c transaction failed
			error_message (WARN,"Failed to read from the i2c bus.\n");
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
			buffer_ptr = buffer;
			error_message (INFO,"RPM: %d Speed: %f Oil temp: %f BatV: %f",enData.rpm,enData.speed,enData.temp_oil, enData.batteryVoltage);
		}
		select_result = select(fd_front_controls + 1, &readset, NULL, NULL, &timeout);
		if (select_result < 0) {
			error_message (WARN, "error %d Port: %s: %s", errno, front_controls_port, strerror (errno));
		}
		else if (select_result == 0){
			// Timeout
		}
		else if (select_result > 0) {
			// Front Controls readable
			if (FD_ISSET(fd_front_controls,&readset)) {
				// Front controls has data to be read
				readFC(fd_front_controls,fcData);
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
		// Send commands
		FD_ZERO(&writeset);
		FD_SET(fd_front_controls,&writeset);
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		
		select_result = select(fd_front_controls + 1, NULL, &writeset, NULL, &timeout);
		if (select_result < 0) {
			error_message (WARN, "error %d : %s", errno, strerror (errno));
		}
		else if (select_result == 0){
			// Timeout
		}
		else if (select_result > 0) {
			// Send Commands
			// Front Controls
			if (FD_ISSET(fd_front_controls,&writeset)) {
				// Front controls accepts commands
				writeFC(fd_front_controls,fcData);
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

