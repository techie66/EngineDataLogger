/*
    EngineDataLogger
    Copyright (C) 2018-2020  Jacob Geigle

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

/*
 * EDL Main program
 * READ THE README!!!!!!
 */

#include "config.h"
#include <unistd.h>			//Needed for I2C port
#include <fcntl.h>			//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <time.h>
#include <string.h>
#include <libgen.h>
#include <sys/time.h>
#include <getopt.h>
#include <isp2.h>

#ifdef FEAT_GPIO
#include <bcm2835.h>
#endif /* FEAT_GPIO */

#ifdef FEAT_I2C
#include "I2Cdev.h"
#endif /* FEAT_I2C */

#include <ignitech.h>
#include "serial.h"
#include "definitions.h"
#include "error_handling.h"
#include "front_controls.h"
#include "bluetooth.h"
#include "cmdline.h"

// TODO option-ify
#define O2_PIN 26
#define LC2_PORT "/dev/serial0"
#define LC2_POWER_DELAY 15 // delay in seconds
#define FC_PORT "/dev/front_controls"
#define TIME_BUF_LEN 256
#define LOG_FILE_LEN 4096

volatile sig_atomic_t 	time_to_quit = false;
// Set default Debug Level Here NONE,ERROR,WARN,INFO,DEBUG
e_lvl			LEVEL_DEBUG = ERROR;

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
	// register signal handlers
	signal(SIGINT, signalHandler);  
	signal(SIGTERM, signalHandler);  
	signal(SIGHUP, signalHandler);  
	
	// Get string for current time
	char time_buf[TIME_BUF_LEN] = {0};
	time_t rawtime = time(NULL);
	if ( rawtime == -1 ) {
		error_message(ERROR,"time() function failed");
		return -1;
	}
	struct tm *ptm = localtime(&rawtime);
	if ( ptm == NULL ) {
		error_message(ERROR,"localtime() failed");
		return -1;
	}
	strftime(time_buf,TIME_BUF_LEN,"%Y%m%d%H%M%S",ptm);

	// TODO option-ify (part way there)
	char const	*i2c_device = "/dev/i2c-1";
	int 		fd_front_controls,
			fd_lc2,
			fd_i2c;
	FILE		*fd_log;
	EDL_Bluetooth	dashboard;

	gengetopt_args_info args_info;
	struct cmdline_parser_params *params;

	/* initialize the parameters structure */
	params = cmdline_parser_params_create();

	/* let's call our cmdline parser */
	if (cmdline_parser (argc, argv, &args_info) != 0)
		exit(1) ;

	if ( args_info.config_file_given ) {
		// call config file parser
		params->initialize = 0;
		if (cmdline_parser_config_file
		(args_info.config_file_arg, &args_info, params) != 0)
			exit(1);
	}

	if ( args_info.v_given )
		LEVEL_DEBUG = e_lvl(args_info.v_given + 1);
	if ( args_info.verbose_given )
		LEVEL_DEBUG = e_lvl(args_info.verbose_arg);
	if ( args_info.quiet_given )
		LEVEL_DEBUG = NONE;

	char const *ignitech_device = NULL;
	IGNITECH* ignition;
	int ignition_read_status = -1;
	if ( args_info.ignitech_given ) {
		ignitech_device = args_info.ignitech_arg;
		ignition = new IGNITECH(ignitech_device);
	}
	if ( args_info.ignitech_dump_file_given ) {
		ignition->enable_raw_dump(args_info.ignitech_dump_file_arg);
	}

	char const *log_file = NULL;
	if ( args_info.output_file_given ) {
		if ( args_info.output_file_date_given ) {
			char log_file_tmp[LOG_FILE_LEN] = {0};
			char *tmp = strdup(args_info.output_file_arg);
			char *dir = strdup(dirname(tmp));
			free(tmp);
			tmp = strdup(args_info.output_file_arg);
			char *base = strdup(basename(tmp));
			free(tmp);
			tmp = strchr(base,'.');
			char *ext = NULL;
			char *file = NULL;
			if (tmp) {
				ext = strdup(tmp);
				file = strndup(base,(strlen(base)-strlen(ext)));
				free(base);
			}
			else {
				file = strdup(base);
				free(base);
			}
			
			if (dir) 
				strcat(log_file_tmp,dir);
			strcat(log_file_tmp,"/");
			if (file) {
				strcat(log_file_tmp,file);
				free(file);
			}
			strcat(log_file_tmp,"-");
			if (time_buf)
				strcat(log_file_tmp,time_buf);
			if (ext) {
				strcat(log_file_tmp,ext);
				free(ext);
			}

			log_file = log_file_tmp;
		}
		else {
			log_file = args_info.output_file_arg;
		}

		// Open log file
		if ((fd_log = fopen(log_file,"a")) < 0) {
			error_message (ERROR,"Failed to open the log file err:%d - %s",errno,strerror(errno));
			//return -1;
		}
		fprintf(fd_log,"\n");
	}

	// End processing options/config

	#ifdef FEAT_GPIO
	// Initialize GPIO output
	if (!bcm2835_init())
		return 1;
	// Set pin mode
	bcm2835_gpio_fsel(O2_PIN, BCM2835_GPIO_FSEL_OUTP);
	#endif /* FEAT_GPIO */
	
	// Open Front controls
	fd_front_controls = fc_open();
	
	// Open LC-2
	fd_lc2 = lc2_open();
	
	// Initialize I2C Interface
	//I2Cdev::initialize();
	//----- OPEN THE I2C BUS -----
	if ((fd_i2c = open(i2c_device, O_RDWR)) < 0) {
		//ERROR HANDLING: you can check errno to see what went wrong
		error_message (ERROR,"Failed to open the i2c bus. err:%d - %s",errno,strerror(errno));
		//return -1;
	}
	

	// Main Loop
	engine_data	enData;
	bike_data	log_data;
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
		// TODO run-time and build-time optional
		if (ioctl(fd_i2c, I2C_SLAVE, engine_data_addr) < 0) {
			//ERROR HANDLING; you can check errno to see what went wrong
			error_message (ERROR,"Failed to acquire bus access and/or talk to slave. err:%d - %s",errno,strerror(errno));
			//return -1;
		}

		// length is calculated by fixed size of data stream from Sleepy Pi
		length = sizeof(enData.rpm) + sizeof(enData.speed) + sizeof(enData.temp_oil) + sizeof(enData.batteryVoltage)
			+ sizeof(enData.odometer) + 2;		//<<< Number of bytes to read
		
		unsigned char 	buffer[60] = {0};
		unsigned char	*buffer_ptr = buffer;
		if (read(fd_i2c, buffer, length) != length) {		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)KE
			//ERROR HANDLING: i2c transaction failed
			error_message (WARN,"Failed to read from the i2c bus.");
		}
		else {
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
		if ( args_info.ignitech_given ) {
			static int num_failures = 0;
			// Read Ignition
			ignition_read_status = ignition->read_async();
			if (ignition_read_status != IGN_SUC ) {
				if (ignition_read_status < IGN_SUC )
					error_message (INFO,"Failed to Read Ignitech err:%d - %s",errno,strerror(errno));
				num_failures++;
				if ( num_failures > IGNITECH_MAX_RESETS ) {
					num_failures = 0;
					log_data.ig_rpm = 0;
					log_data.batteryvoltage = 0;
					log_data.map_kpa = 0;
				}
			}
			else if (ignition_read_status == IGN_SUC) {
				num_failures = 0;
				error_message (DEBUG,"Read Ignitech, RPM: %d, Battery: %d\n", ignition->get_rpm(),ignition->get_battery_mV());
				log_data.ig_rpm = ignition->get_rpm();
				log_data.batteryvoltage = ignition->get_battery_mV()/float(1000);
				log_data.map_kpa = ignition->get_sensor_value();
			}
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
		// TODO LC-2 runtime and build-time optional (libISP)
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

		int my_rpm = enData.rpm;
		if ( args_info.ignitech_given ) {
			if ( ignition->get_status() == IGN_SUC ) {
				my_rpm = ignition->get_rpm();
			}
		}
		if (my_rpm > RUNNING_RPM) {
			engineRunning = true;
			fcData.serialCmdA |= ENGINE_RUNNING;
			gettimeofday(&currtime, NULL);
			my_time = currtime.tv_sec;
			if (start_running_time == 0 ) {
				start_running_time = currtime.tv_sec;
			}
			#ifdef FEAT_GPIO
			if (my_time - start_running_time > LC2_POWER_DELAY) {
				bcm2835_gpio_write(O2_PIN, HIGH);
			}
			#endif /* FEAT_GPIO */
			error_message (DEBUG,"Running. %s",exCmd_bin(fcData.serialCmdA));
		}
		else if ( my_rpm <= STOPPED_RPM ) {
			engineRunning = false;
			fcData.serialCmdA &= ~ENGINE_RUNNING;
			start_running_time = 0;
			#ifdef FEAT_GPIO
			if (!o2_manual) {
				bcm2835_gpio_write(O2_PIN, LOW);
			}
			#endif /* FEAT_GPIO */
			error_message (DEBUG,"Not Running. %s",exCmd_bin(fcData.serialCmdA));
		}

		if(db_from_cmd == TRPRST) {
			error_message(WARN,"Resetting Trip");
			en_to_cmd = 'T';
			db_from_cmd = NO_CMD;
		}

		#ifdef FEAT_GPIO
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
		#endif /* FEAT_GPIO */

		// Create Flatbuffer
		// Sent to dashboard and 
		// TODO: option to write flatbuffer to datafile
		// TODO runtime and build-time optional dashboard
		EDL::AppBuffer::BikeT bikeobj;
		flatbuffers::FlatBufferBuilder fbb;

		bikeobj.alt_rpm = enData.rpm;
		bikeobj.rpm = my_rpm;
		bikeobj.speed = enData.speed;
		bikeobj.odometer = enData.odometer;
		bikeobj.oil_temp = enData.temp_oil;
		bikeobj.oil_pres = enData.pres_oil;
		bikeobj.batteryvoltage = enData.batteryVoltage;
		bikeobj.systemvoltage = fcData.systemVoltage;
		bikeobj.blink_left = fcData.left_on;
		bikeobj.blink_right = fcData.right_on;
		bikeobj.trip = enData.trip;
		if (lc2_data.status == ISP2_NORMAL || o2_manual) {
			bikeobj.lambda = lc2_data.lambda;
			log_data.lambda = lc2_data.lambda;
		}
		bikeobj.gear = "?";
		if (fcData.in_neutral) {
			bikeobj.gear = "N";
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

		log_data.oil_temp = enData.temp_oil;
		log_data.oil_pres = enData.pres_oil;
		log_data.alt_rpm = enData.rpm;
		log_data.speed = enData.speed;
		log_data.systemvoltage = enData.batteryVoltage;
		if ( args_info.output_file_given ) {
			// TODO remove flatbuffers dependency for the log
			// TODO log advance when available
			fprintf(fd_log,"%d,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%s.%06ld,%.2f,%d\n",
				log_data.ig_rpm,
				log_data.alt_rpm,
				log_data.speed,
				log_data.systemvoltage,
				log_data.batteryvoltage,
				log_data.oil_temp,
				log_data.oil_pres,
				engineRunning,
				time_buf,
				currtime.tv_usec,
				log_data.lambda/1000.0, 
				log_data.map_kpa
			);
			fflush(fd_log);
		}

		usleep(50000);
	}

	// TODO Make run-time optional
	#ifdef FEAT_GPIO
	bcm2835_gpio_write(O2_PIN, LOW);
	bcm2835_close();
	#endif /* FEAT_GPIO */
	// Return 0 for clean exit
	return 0;
}

