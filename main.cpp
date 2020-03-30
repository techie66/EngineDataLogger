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
#include <isp2.h>
#include "I2Cdev.h"
#include "serial.h"
#include "definitions.h"
#include "error_handling.h"
#include "front_controls.h"
#include "bluetooth.h"
#include <bcm2835.h>
#include <ignitech.h>

#define O2_PIN 26
#define LC2_PORT "/dev/serial0"
#define LC2_POWER_DELAY 15 // delay in seconds

#define FC_PORT "/dev/front_controls"

volatile sig_atomic_t 	time_to_quit = false;
// Set default Debug Level Here NONE,ERROR,WARN,INFO,DEBUG
e_lvl			LEVEL_DEBUG = WARN;

int fc_open() {
	int fd;
	fd = open (FC_PORT, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0) {
		error_message (WARN,"error %d opening %s: %s", errno, FC_PORT, strerror (errno));
		return -1;
	}

	// Set interface parameters for front controls
	set_interface_attribs (fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
	set_blocking (fd, 0);                // set no blocking
	return fd;
}

int lc2_open() {
	int fd;
	fd = open (LC2_PORT, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0) {
		error_message (WARN,"error %d opening %s: %s", errno, LC2_PORT, strerror (errno));
		return -1;
	}

	// Set interface parameters for front controls
	set_interface_attribs (fd, B19200, 0);  // set speed to 19,200 bps, 8n1 (no parity)
	set_blocking (fd, 0);                // set no blocking
	return fd;
}

int main(int argc, char *argv[])
{
	// TODO: move variable declarations to more sensible spots
	// keep constants up here
	char const	*i2c_device = "/dev/i2c-1",
			*log_file = "system_log.csv";
	char		empty[4] = "";
	char		*ignitech_device = empty;
	int 		fd_front_controls,
			fd_lc2,
			fd_i2c;
	FILE		*fd_log;
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
			{"ignitech",	required_argument,	0, 'i'},
			{0,0,0,0}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;
		int opt = getopt_long (argc, argv, ":qi:v:h",
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
		case 'i':
			// TODO seems pretty dumb to blindly trust user input
			ignitech_device = strdup(optarg);
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
			printf("\t\t\t\tLEVEL is one of: NONE,ERROR,WARN,INFO,DEBUG\n\n");
			printf("  -q\t\t\t\tSame as --verbose = NONE\n");
			printf("  -h, --help\t\t\tPrint this help message.\n");
			printf("  -i, --ignitech\t\tDevice path for Ignitech\n");
			return 0;
		}
	}

	// optind is for the extra arguments
	// which are not parsed
	for(; optind < argc; optind++){
		printf("extra arguments: %s\n", argv[optind]);
	}

	// Initialize GPIO output
	if (!bcm2835_init())
		return 1;
	// Set pin mode
	bcm2835_gpio_fsel(O2_PIN, BCM2835_GPIO_FSEL_OUTP);

	// Open Front controls
	fd_front_controls = fc_open();
	
	// Open LC-2
	fd_lc2 = lc2_open();
	
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

	// Open Ignitech
	IGNITECH ignition (ignitech_device);
	int ignition_read_status = -1;

	// Main Loop
	engine_data	enData;
	isp2_t		lc2_data = isp2_t();
	fc_data		fcData;
	System_CMD	db_from_cmd = NO_CMD;
	char		en_to_cmd = 0;
	bool		engineRunning = false;
	bool		o2_manual = false;
	while (!time_to_quit) {
		int	length;
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
			+ sizeof(enData.odometer) + 2;		//<<< Number of bytes to read
		
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
			buffer_ptr += sizeof(enData.trip);
			memcpy((void*)&tmp,(void*)buffer_ptr,sizeof(tmp));
			enData.pres_oil = tmp / 100.0;
			buffer_ptr = buffer;
			error_message (INFO,"ODO: %d RPM: %d Speed: %f Oil temp: %f pres: %f BatV: %f",enData.odometer,enData.rpm,enData.speed,enData.temp_oil, enData.pres_oil, enData.batteryVoltage);
		}

		// Read Ignition
		ignition_read_status = ignition.read_async();
		if (ignition_read_status < IGN_SUC ) {
			error_message (ERROR,"Failed to Read Ignitech err:%d - %s",errno,strerror(errno));
		}
		else if (ignition_read_status == IGN_SUC) {
			enData.rpm = ignition.get_rpm();
			error_message (INFO,"Read Ignitech, RPM: %d, Battery: %d\n", enData.rpm,ignition.get_battery_mV());
			// TODO read the other stuff
		}

		// Setup read sets
		int	select_result = 0,
			max_fd = 0;
		fd_set	readset,
			writeset;
		FD_ZERO(&readset);
		FD_SET(dashboard.getListener(),&readset);
		max_fd = (max_fd>dashboard.getListener())?max_fd:dashboard.getListener();
		if (fd_front_controls > 0) {
			error_message(DEBUG,"Adding front controls to select");
			FD_SET(fd_front_controls,&readset);
			max_fd = (max_fd>fd_front_controls)?max_fd:fd_front_controls;
		}
		else {
			fd_front_controls = fc_open();
		}
		if (fd_lc2 > 0) {
			error_message(DEBUG,"Adding LC-2 to select");
			FD_SET(fd_lc2,&readset);
			max_fd = (max_fd>fd_lc2)?max_fd:fd_lc2;
		}
		else {
			fd_lc2 = fc_open();
		}
		if (dashboard.getClient() > 0) {
			error_message(DEBUG,"Adding dashboard client to select");
			FD_SET(dashboard.getClient(),&readset);
			max_fd = (max_fd>dashboard.getClient())?max_fd:dashboard.getClient();
		}

		struct timeval	timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		
		// Do Select()
		select_result = select(max_fd+1, &readset, &writeset, NULL, &timeout);
		if (select_result < 0) {
			error_message (WARN, "error %d Port: %s: %s", errno, FC_PORT, strerror (errno));
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
			if (FD_ISSET(fd_lc2,&readset)) {
				error_message (DEBUG,"Select read LC-2");
				ISP2::isp2_read(fd_lc2,lc2_data);
				error_message(ERROR,"Status: %d Lambda: %d\n",lc2_data.status,lc2_data.lambda);
			}
		}

		// calculate stuff / make decisions
		static time_t start_running_time;
		struct timeval	currtime;
		time_t my_time;
		if (enData.rpm > RUNNING_RPM) {
			engineRunning = true;
			fcData.serialCmdA |= ENGINE_RUNNING;
			gettimeofday(&currtime, NULL);
			my_time = currtime.tv_sec;
			if (start_running_time == 0 ) {
				start_running_time = currtime.tv_sec;
			}
			if (my_time - start_running_time > LC2_POWER_DELAY) {
				bcm2835_gpio_write(O2_PIN, HIGH);
			}
			error_message (DEBUG,"Running. %s",exCmd_bin(fcData.serialCmdA));
		}
		else {
			engineRunning = false;
			fcData.serialCmdA &= ~ENGINE_RUNNING;
			start_running_time = 0;
			if (!o2_manual) {
				bcm2835_gpio_write(O2_PIN, LOW);
			}
			error_message (DEBUG,"Not Running. %s",exCmd_bin(fcData.serialCmdA));
		}

		if(db_from_cmd == TRPRST) {
			error_message(WARN,"Resetting Trip");
			en_to_cmd = 'T';
			db_from_cmd = NO_CMD;
		}

		if(db_from_cmd == O2MANON) {
			error_message(INFO,"Manually turning on O2 sensor");
			bcm2835_gpio_write(O2_PIN, HIGH);
			o2_manual = true;
			db_from_cmd = NO_CMD;
		}

		if(db_from_cmd == O2MANOFF) {
			error_message(INFO,"Manually turning off O2 sensor");
			bcm2835_gpio_write(O2_PIN, LOW);
			o2_manual = false;
			db_from_cmd = NO_CMD;
		}

		// Create Flatbuffer
		// Sent to dashboard and 
		// TODO: data logfile
		EDL::AppBuffer::BikeT bikeobj;
		flatbuffers::FlatBufferBuilder fbb;

		bikeobj.rpm = enData.rpm;
		bikeobj.speed = enData.speed;
		bikeobj.odometer = enData.odometer;
		bikeobj.oil_temp = enData.temp_oil;
		bikeobj.batteryvoltage = enData.batteryVoltage;
		bikeobj.blink_left = fcData.left_on;
		bikeobj.blink_right = fcData.right_on;
		bikeobj.trip = enData.trip;
		if (lc2_data.status == ISP2_NORMAL || o2_manual) {
			bikeobj.lambda = lc2_data.lambda;
		}
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
		char time_buf[100];
		strftime(time_buf, 100, "%D %T", localtime(&my_time));
		// RPM, Speed, sysvoltage, batVoltage, oil temp, oil pressure, running, Time, lambda, IAP(kpa)
		fprintf(fd_log,"%d,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%s.%06ld,%.2f,%d\n",enData.rpm,enData.speed,fcData.systemVoltage,enData.batteryVoltage,enData.temp_oil,enData.pres_oil,engineRunning,time_buf,currtime.tv_usec,lc2_data.lambda/1000.0, ignition.get_map_kpa());
		fflush(fd_log);

		usleep(50000);
	}

	bcm2835_gpio_write(O2_PIN, LOW);
	bcm2835_close();
	// Return 0 for clean exit
	return 0;
}

